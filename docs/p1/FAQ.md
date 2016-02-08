CS 4411 Practicum in Operating Systems Spring 2016
___

#Project 1: Non-Preemptive Multithreading (FAQ)
##Managing Stacks
###Stack Allocation
>How does stack allocation work?

Each new thread you create needs its own stack to store function arguments and return addresses on. You can obtain a stack by caling
`minithread_allocate_stack`, and initialize it by calling `minithread_initialize_stack`. Initializing the stack requires two
functions: the function the new thread should run, and a "final" function, which runs after the main function has returned. The final function need not be unique for each thread. Both functions have arguments, so that the order
of invocation looks like:
```c
main_proc(main_arg);
...
finally_proc(finally_arg);
```
The exception is the first thread you create, which gets the original stack for the process, and runs inside
`minithread_system_initialize`. It does not need a thread control block, however, and will be able to be context switched back to using `minithread_switch`. This first thread cannot terminate in the same way as the others: since it's running on the host OS stack, when it returns from `main`, it will destroy the whole process.

###Freeing Stacks
>Why can't a thread free it's own stack?

When a thread announces it wants to terminate, by calling the thread destroy
routine, it is still running on its own stack. In order to transfer control
to another thread in a context switch, it pushes some data onto its stack.
So if it freed its stack and then context-switched, it would end up writing
to an invalid stack. The solution is to have the terminating thread "notify
the system" (set a flag, put itself on a queue, etc.) so that a _dedicated_ cleanup thread (with its own TCB, stack, etc)
knows to deallocate the terminated thread's stack.


##Managing Threads
###What to do with the initial context
>How does a `minithread` represent the thread that NT/Linux/OSX gives us?
>Where do we get a thread's stack base and stack top to initialize our `minithread` structure?
>What is the exact purpose of representing the kernel thread as a `minithread`?

Bootstrapping is a problem for any OS. In `minithread`s, just like in a real
system, you have the problem of going from the initial bootloader context to
switching between threads that you have created. In our case, the initial
stack assigned to us by NT/Linux/OSX is the bootloader stack, and you would like to
then start context switching between `minithread`s.

The best approach is to use the initial stack provided to you by the
bootloader (your host OS) as if it were a `minithread` stack. The problem is
that you don't know the base or the top of that stack.
But why do you need to know the base or the top ? You need the base if you
want to ever free the stack, and you need the top when you need to
initialize the stack. But the boot stack is already initialized, and if you
turn the initial boot context into the idle thread or the stack cleaner
thread, then it will never terminate. Hence, you'll never need to free its
stack.

_Hint: There's no need to allocate a special TCB for the initial context, nor should it ever be put in the run queue. The idle thread should only be context switched back to when there is no other work to be done._

###`final_proc` return
>Why can't `final_proc` return? Don't all functions return?

`final_proc` cannot return because otherwise you will pop the last stack
frame from the stack.  By definition of "last stack frame", there is no
other frame to return to; instead, we would leave the process in an
undefined state, possibly executing undefined code (it's more likely that
we'd just crash).
`final_proc` should do all necessary bookkeeping to let itself be cleaned up later by another thread.  Then it should perform a context switch out of the now-terminating context.

###Thread Stop and Start
>What are `minithread_start` and `minithread_stop` for?

They're really only useful for implementing semaphores, it's unlikely that
your "user programs" will need to use these low-level calls.

###Thread Identifiers
>What are `minithread_self` and `minithread_id` for?

A thread needs to be able to get a pointer to its TCB in case it wants to
add it to some data structure, for instance: then another thread could call
`minithread_start` on it. Of course, this is only useful outside
`minithread.c`. Thread IDs are really only helpful for debugging.

###System Termination
>What do I do if all the threads terminate? What if there's no thread avaible to
run when a thread does a stop or yield?

Some students have the tendency to exit from the system when there are no
threads to schedule. ***This is a mistake.*** Keep in mind that you are writing an
operating system, a long-lived process that is never supposed to terminate.
If, for some reason, there are no more threads to schedule, your system
should just loop in the idle loop. A subsequent interrupt might kick off a
previously blocked thread, and cause a thread to start up at an arbitrary
time in the future.

##Semaphores
###Reading Semaphore Internals
>Is it ok to read (not write) the value of a semaphore without executing a `P` on it?

No, you should stick to the interface in `synch.h`.  Semaphores do not
provide an interface by which the internal semaphore count can be read.
There are two good reasons for this:
- Reading internals of a semaphore breaks the semaphore abstraction and encapsulation
- That value will be out of date the moment it is read, and therefore code
that depends on reading the internal count of a semaphore cannot be correct.

###Semaphore Synchronization
>How should threads wait for semaphores?

If a thread cannot complete `semaphore_P` on a semaphore (because it is 0), it must wait for another thread to call
`semaphore_V`. However, you should not make this thread poll on the semaphore state, instead it should be stopped,
put on wait queue, and only be started if another thread calls `semaphore_V`.

>How should we implement concurrent access to semaphores?

For Project 1 you can assume that there is no preemption (in fact there cannot be any as we haven't implemented it yet).
So unless you call `minithread_yield` or `minithread_stop` in a semaphore primitive, no two thread will execute them concurrently.
This means, for Project 1, you don't need to acquire any locks on the semaphore.

###Semaphore Behavior
>How do semaphores work?

Please see the [CS4410 lecture notes](https://www.cs.cornell.edu/courses/cs4410/2016sp/lectures.php) for an explanation of semaphores.

##Misc
###Debugging
>I can't find where my code is `SEGFAULT`-ing, what should I do?

We recommend familiarizing yourself with [`gdb`](https://www.gnu.org/software/gdb/), which allows you to step through your code to find the exact instruction that causes the error.

###Extra Functions
>What are all these extra functions in `machineprimitives.h`?

Remember, we said to treat this as a legacy codebase, so some code and functionality will be deprecated. This includes
`minithread_trampoline`, any function regarding test-and-set locks, etc.

###Other Questions
>I have another question that this document doesn't answer!

Please, feel free to ask us on [Piazza](https://piazza.com/class/ijzyrk21o9re6) or during [office hours](https://www.cs.cornell.edu/courses/cs4410/2016sp/staff.php)!
