Introduction
============

This lab is designed to introduce you to the basics of multithreading.
By the end of this lab, you will be expected to understand how to create
threads, handle critical sections using semaphores, and create thread
pools.

In this lab you are expected to perform the basics of cloning your Lab 9
repository from the GitLab course group. A link to the course group can
be found
[here](https://git.cs.dal.ca/courses/2022-winter/csci-2122)
and your repository can be found in the Lab9 subgroup. See the Lab
Technical Document for more information on using git. You will notice
that your repository has a file in the **Lab9** directory named
**delete\_this\_file**. Due to the limitations of GitLab, we are not
able to push completely empty directories. Before you push your work to
your repository (which should be in the **Lab9** directory anyway), make
sure to first use the **git rm** command to remove the extra file. If
you do not, your pipeline could fail.

**Be sure to read this entire document before starting!**

Table of Contents
===============================

[[_TOC_]]

Multithreading
==============

Multithreading is a common practice in performance computing and is the
primary way to get the most out of your modern CPUs. As the size of
components gets smaller and smaller, they are now very close to the
smallest component we can pass electricity through without causing
interference with other components. Due to this limitation,
manufacturers are now (instead of making things smaller) putting
multiple processing units on their CPUs, called **cores**. These cores
can act as independent processing units from the point of view of the
operating system (and thus your software), allowing you to create
lightweight processes called **threads**, which can be processed in
parallel.

Often you will hear about limitations in software design which keep CPUs
from reaching their maximum potential on a single process. Most
software, and especially as you go back in time, tends to be
single-threaded from the point of view of the operating system, meaning
very little of the software is capable of utilizing more than one core
of a CPU at a time, which is a leftover practice from times when CPUs
weren't capable of the kind of multiprocessing they're able to achieve
now. Video games are a huge culprit, as many of the popular engines
still run on a single \"catch all\" frame calculation cycle which acts
as a single monolithic main loop for all of the game's logic and
graphics handling.

In this lab we will discuss some basic principles for handling
multithreaded software, then code some simple examples before moving on
to more heavy-duty applications.

Heavyweight vs. Lightweight Processes
-------------------------------------

The first thing to understand is the difference between heavyweight and
lightweight processes, which is a very straightforward distinction:
programs are heavyweight processes and threads are lightweight
processes.

When it comes to heavyweight processes, the operating system treats them
as distinct entities in the memory space. They're cloned from a previous
heavyweight process, and then the cloned data is overwritten with the
new program so that it is distinct relative to the code which it was
loaded from. Once the heavyweight process is creating by the operating
system, it has its own memory, it's own address space, and its memory is
protected so that no other programs are able to access it without
permission of the system or the process itself. These are the programs
you see if you run your Task Manager in Windows, or **htop** on
Timberlea. This is also how every one of your C programs starts when you
execute them.

A lightweight process **is a member of** (belongs to) a heavyweight
process. In general, they're viewed through the lens of a **thread**.
Threads are sub-programs of a program and do not have their own memory
assigned by the operating system. Instead, they share the existing
memory already allocated to the program they belong to. This means that
threads are able to see any global variables, functions, and definitions
held within a program and are able to communicate through those
mechanisms freely, as long as the code designer has allowed that kind of
interaction. Even though threads are considered a part of a heavyweight
process, they are still scheduled independently to ensure maximum
resource utilization. You will learn more about process scheduling in
the Operating Systems class.

When a heavyweight program is allowed to run by the operating system,
the system also decides which of the program's threads it will place on
the CPU to execute. Threads will run freely until their subroutine is
completed, at which point their return value is stored and the thread is
destroyed. Since it is up to the operating system which threads are
allowed to execute on a CPU at any given time, there's no guarantee
which order the threads will be executed in, or for how long, which is
important to note when we're trying to work with shared memory later.

The POSIX Thread Library
------------------------

When working with threads in this course, we will be using the POSIX
Thread Library, which is normally referred to as **pthreads**. The
pthreads library is available on Timberlea and will supply you with all
of the functionality you need to take advantage of multiprocessing in C.
You can view the man page for the pthreads library by entering **man
pthreads** into the terminal on Timberlea. You will also find additional
man pages for all of the commands specific to the pthreads library by
entering them into the man program accordingly. Note that we will not be
using any of the advanced features of pthreads, mostly because it's not
necessary under most use cases, but also because we don't want to make
the concept of threads too complicated.

Importing and Compiling POSIX Threads
-------------------------------------

In order to import the pthreads library into your code, there are a few
things to consider. The first thing to understand is that pthreads do
not like to be compiled without also being linked. This means that you
will not be able to easily apply the **-c** option in your gcc commands
in your Makefile. When you compile a C source file (.c) which includes
pthreads, you will have to do it directly. For this reason, in this lab,
you will not be required to make any object files (.o) for any source
file which imports the pthreads library.

To import the pthreads library into your code, you will need to use
**\#include <pthread.h>**. This will give you access to all of the
pthread functions outlined below. You will also need to include the
library import option on your gcc command, which is **-lpthread**. This
should be the last thing attached to any gcc command which requires it
to ensure maximum compatibility with your files and other gcc options.

Creating a POSIX Thread
-----------------------

The basics of a pthread revolve around creating functions which your
threads will execute to completion. These functions need to be
constructed in a very particular fashion before being passed to the
pthread library to have the thread created and executed.\
In order to create and execute a pthread, you will need to use
**pthread\_create** function. The **pthread\_create** function takes
four parameters, which are outlined as follows:

1.  A pointer to a thread ID, represented by the **pthread\_t** data type.
2.  A struct of attributes which you wish to modify, represented by the
    **pthread\_attr\_t** data type. For our purposes, this will always
    be set to NULL.
3.  A function pointer to a function which the thread will execute when
    it is created.
4.  A void\* to any data you wish to pass to the function held by this
    thread when it begins execution. In this lab, we will create structs
    for this purpose.

When the pthread is created successfully, it is also immediately
executed. The pthread itself is held internally, in the systems created
by the pthread library, and thus you do not have direct access to it.
However, when your pthread\_create function ends, it assigns an integer
value to the provided **pthread\_t**, which can be used later to
designate which thread you would like to reference in further function
calls to the pthread library. In practice, the pthread\_t type is an int
value, although its exact size is implementation-specific, and thus it
is not appropriate to use integer types directly. You can see an example
of how to create a few simple pthreads here:

```c
// Compile with: gcc --std=c18 fileName.c -lpthread
#include <stdio.h>
#include <pthread.h>

void* example(void* args)
{
    pthread_t me = pthread_self();
    printf("This is inside thread %ld.\n", me);
}

int main(int argc, char** argv)
{
    pthread_t thread;

    pthread_create(&thread, NULL, example, NULL);
    pthread_create(&thread, NULL, example, NULL);
    pthread_create(&thread, NULL, example, NULL);
    pthread_create(&thread, NULL, example, NULL);
    pthread_create(&thread, NULL, example, NULL);

    return 0;
}
```

When this program executes, it will create five threads, each executing
a single print statement which prints its thread ID. Each thread in this
case is being executed using a single pthread\_t variable, so each time
a thread is executed its ID is lost. For the purposes of the example,
this is sufficient. In a real execution scenario, creating multiple
threads is better done with an array of pthread\_t values. Creating an
array of pthread\_t values is no different than creating any other
array. It can be iterated through with pthread\_create calls to
initialize and execute all of your threads.

You may notice that we used the pthread\_self() function. This function,
when used inside a thread, will return you the ID it has been assigned.
This can be useful for determining which thread is which in situations
where it may be important to have their execution monitored or
synchronized with other threads.

### Setting up a Thread Function

You may notice in the examples throughout this lab that the **example**
functions are written with a very specific signature: they accept a
single void\*, and return a void\*. When you are setting up a function
which will be executed by a thread, you need to consider the following
things:

1.  Once a thread starts, it acts as a mini-program which executes in
    parallel to the main thread of your program, which is usually your
    main function. The thread will run the function you give it until
    that function is done, and then the thread will stop and store that
    function's return value so you can retrieve it. The return value is
    stored as a void\*, but since you know what the thread's function
    returns, you should have no problem converting it to the correct
    pointer type and using it however you wish. You can retrieve this
    data by using a pthread\_join, which is explained below.
2.  The function accepts a single void\* argument, which is the same
    void\* pointer passed in as the fourth argument to pthread\_create.
    When pthread\_create is executing, it creates a new thread (which
    runs outside of the standard execution flow of the function that
    created it), and once the thread is created it calls the function
    stored in the function pointer and passes the arguments pointer into
    it. This is how your function is given access to the arguments
    structs we will create over the course of the lab. Once your
    function starts, since you know what kind of argument pointer you
    passed it, you can convert the void\* to the correct data type and
    use it however you wish.

Passing Arguments to a POSIX Thread
-----------------------------------

To pass arguments to a pthread, you can convert any type of data into a
void\* and pass it into the pthread\_create function as the final
argument. This can be any data you choose, although we recommend using
structs for anything beyond a simple value, as they're the easiest way
to manage a variety of different data types that would normally be
associated with a single function.\
An example of passing data to a pthread can be seen here:

```c
#include <stdio.h>
#include <pthread.h>

typedef struct _Args
{
    char* this;
    int that;
    float other;
} Args;

void* example(void* args)
{
    Args* arg = args;
    printf("My arguments are: %s %d %f\n", arg->this, arg->that, arg->other);
}

int main(int argc, char** argv)
{
    pthread_t thread;
    Args arg;
    arg.this  = "Hello!";
    arg.that  = 13;
    arg.other = 815.0f;
    pthread_create(&thread, NULL, example, &arg);
    return 0;
}
```

As you can see, if you have a firm understanding of how void pointers
work (and you should by now, after all of the lists and collections
you've had to set up with void pointers!), it should be fairly easy to
pass arguments into your thread's function during creation. We can
simply cast the incoming void pointer to whatever struct type we're
using and have access to all of the fields, assuming it was properly
allocated. In the above example you may notice that I did not manually
allocate the Args struct. You can allocate it manually if you so desire,
but I did not for this example.

When you run this code, you may notice that sometimes it doesn't print
anything at all. What's going on? It turns out that, by default, your
program will not wait for the individual threads to finish. If the
program creates a thread and then exits too quickly, the thread may not
have time to properly execute and will be cancelled by the operating
system when the heavyweight process ends. How can we stop that from
happening?

Joining a POSIX Thread
----------------------

To ensure your threads finish their execution, you can perform a
**pthread\_join** on them. Joining a thread to your program has two
benefits. First, joining a thread stops the main program logic from
continuing until the thread in question stops. If you have multiple
threads currently executing and you want them to be guaranteed to
finish, you can join each one in your code, one after the other. This
can be done manually, with a series of individual lines, or via a loop
if you have to iterate through an array of thread IDs.

The second benefit of using a join is that you're able to receive a
return value from the function the thread is running. You may have
noticed in the previous code snippets that the example function has a
very specific signature: it must **return a void\***, and it must also
**accept a void\* as a function parameter**. We saw in the previous
example that we can pass a void\* into the function via the
pthread\_create function. In order to retrieve data from the function
via a return statement, we must do so with a pthread\_join function
call.

A pthread\_join takes in two parameters:

1.  A thread ID value, represented by the **pthread\_t** data type. Note
    that unlike pthread\_create, this is not a pointer.

2.  A void\*\* value for holding the returning value after the join has
    completed.

The second parameter can be a little strange at first. The reason it is
a void\*\* and not a void\* is because the pthread\_join function has to
be able to give you the **original pointer**, which means you need to
store the pointer itself, not just the data inside. If you only have it
a void\*, it would only be able to affect the data the pointer is
pointing to. What the void\*\* allows the function to do is not just
change the data in the pointed memory location, but it can change the
whole void\* to a **totally different pointer location**.

The key here is that because C passes by copy, when you pass in a
pointer (which is technically just an unsigned long), C makes a copy of
that pointer and makes it point to whatever the original pointer was
pointing to. That means if you only pass in a void\*, C makes a copy of
that void\* inside the function. Since the function is working with a
copy, any changes it makes to that copied void\* will not affect the
original pointer it was copied from. In order for it to be able to
return a value to you, you therefore have to give it a pointer to the
void\*. That way it receives the address to the void\* you want
modified, then modifies its data, and your original pointer has now been
altered.

While this seems complicated, all you really need to do is create a
pointer for the data type you'd like to store the returned value in,
then pass in the address of that variable. You can see an example of a
return value with a join here:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct _Args
{
        char* this;
        int that;
        float other;
} Args;

void* example(void* args)
{
    Args* arg = args;
    printf("My arguments are: %s %d %f\n", arg->this, arg->that, arg->other);

    int* value = malloc(sizeof(int));
    *value = 15;

    return value;
}

int main(int argc, char** argv)
{
        pthread_t thread;

        Args arg;
        arg.this  = "Hello!";
        arg.that  = 13;
        arg.other = 815.0f;

        int* result = NULL;

        pthread_create(&thread, NULL, example, &arg);

        pthread_join(thread, (void**) &result);

        printf("Returned Value = %d\n", *result);

        return 0;
}
```

You will notice a few things in this code. First, we don't allocate the
**result** variable. This is not necessary, as the thing being returned
needs to be allocated inside the function. It's important to allocate
the data you plan on returning before you return it. Failure to do so
could lead to your values being deallocated when your function ends.
Always remember that if you don't allocate something yourself inside a
function, C will automatically deallocate it when the function ends.
Normally this isn't a problem because C will pass-by-copy, but there are
situations where you can run into bad copies. For example, if you try to
create **int value = 15** and then return **&value**, C will inform you
that you will lose the value of 15 because **int value** is local to the
function and we be freed automatically when the function ends.

You should also notice that we specifically have to convert the result's
address (being passed into the join function) to a void\*\* in order for
this to work. If you don't include that type cast, C will complain that
the address types do not match.

Returning Values without Join
-----------------------------

While the above section says you can receive values from the thread by
calling a pthread\_join and giving it a correct double pointer to store
the return value in, it's also possible to return values in other (less
clean) ways. Since you are handing in a pointer to an argument struct,
there's nothing stopping you from creating a field in that struct which
is capable of storing an output value (or a value for determining
ongoing status). Since you still have access to that argument struct on
the outside of the thread, having the thread update that struct while
you periodically check it from outside the thread could prove useful,
easier, and more convenient (depending on the situation) than using a
join. Remember that using a join forces your code to stop and wait for
the thread, and you may not necessarily want to do that to see what's
happening inside!

Critical Sections and Race Conditions
-------------------------------------

Since your threads simply execute and are not directly controlled by you
after they're created, you can run into problems with certain types of
code where it's possible for two threads to operate on the same piece of
data simultaneously, possibly creating instability in your data
structures. Consider the following example:

You create an array list and decide that you want to add 100,000,000
integers values to it. Since it would take a single thread a very long
time to read in and add all of those values to the array list, you
decide to create ten threads to split the job up. That way each thread
can add 10,000,000 values for you, and since they're in parallel they
should take about 1/10 of the time.

However, because the operating system doesn't know what the threads are
doing and is likely to want to let every thread have at least some
execution time, it will let each thread run for 10 seconds. Your first
thread starts running (along with a few others) and everything seems
fine, until it gets very close to the moment when your thread will be
moved off of the CPU to give another thread some time to execute. Your
thread gets the value 27 and tries to add it to the end of your array
list. It manages to get the memory allocated, stores the 27 inside it,
and *just before* it manages to increase the size of your array list by
1, the operating system swaps it for another thread on the CPU.

That new thread then tries to add something to the array list, but
because the first thread wasn't able to increase the size in time, this
new thread adds something to the end of the array list, which it sees as
the same index as the last thread. It allocates new memory to the last
index, overwriting the value 27 and leaking that memory (since we no
longer have a pointer to it) and then increments the size of the array.
It eventually is switched out by the operating system and the original
thread is returned to running from the same place it left, where it
increments the size and moves on long nothing happened.

So from this situation, we've lost one of our values (27), and the array
list thinks that it has one more element inside it than it actually
does, meaning the stability of the array list is now broken. It's
possible that this situation could happen multiple times and you could
end up with some serious errors down the road.

This is referred to as a **race condition**, where each thread is
attempting to change some shared data before the others are able to do
so. The place where this fight for shared data control takes place is
referred to as a **critical section** in your code, and it is important
to protect your critical sections from the impact of multiple threads
fighting for data control.

To avoid this problem, we will implement a type of code locking
structure called a **semaphore**. A semaphore is a simple piece of code
which acts as a check-in or waiting area for your threads. In practice,
a semaphore is a very simple piece of code which acts as a number. The
number is initially set equal to the number of threads you want to allow
access to your critical section. In this lab, that number will be 1, to
ensure that the critical section is entirely **mutually exclusive**,
sometimes shorted to **mutex**. Mutually exclusive things, by
definition, cannot happen together, so when you see people talk about
something mutually exclusive, in means that only one thing can happen at
a time. In this case, threads in the critical section will be considered
mutually exclusive (only one thread can access the critical section at a
time) if the semaphore is working correctly.

Every time a thread reaches a **semaphore wait** point, it checks to see
if the semaphore is greater than 0. If it is, it will automatically
reduce the value of the semaphore by 1 and then proceed past the wait
point. If the semaphore is 0 or less, the thread will **block**. A block
is what occurs when the operating system is waiting for some kind of
feedback from the user, but it can also be used to temporarily put a
process to sleep. This forces a new thread to be loaded while the
previous thread waits for the semaphore to go back to positive. This can
happen to multiple threads, making them all stop and wait at the
semaphore wait point. It's possible to have multiple semaphores to
protect multiple parts of your code, with threads waiting at each.

When a thread enters a critical section, it is able to perform any
calculations on the critical section it desires. When it is done, it
will move through a **semaphore post** point. A post point is where the
thread lets the semaphore know that it has completed its work inside the
critical section and thus the next thread is free to move inside. When
it reaches the post point, it tells the semaphore belonging to the
current critical section to increase its value by 1. If this makes the
semaphore positive, the next thread will proceed past the wait point
(decreasing the semaphore value by 1) and the process will repeat.

We can create a semaphore with the pthread library. This is done by
importing the <semaphore.h> library (which is included in the
pthread library). This gives us access to the **sem\_t** data type, the
**sem\_init** function, the **sem\_wait** function, and the
**sem\_post** function.

Using POSIX Semaphores
----------------------

Similar to pthreads, semaphores are created using their own data type,
**sem\_t**. For Lab 9 and 10, we will store semaphores in a global scope
(outside of any function) and can be declared below your includes and
defines. Once a semaphore is created, you will need to initialize it
before you start creating threads. This can be done with the sem\_init
function. This function accepts three parameters:

1.  A pointer to a sem\_t value. The value of a semaphore is assigned by
    the operating system, but is generally a semaphore value plus a
    waiting queue.
2.  An integer flag for determining whether or not this semaphore should
    be shared by sub-processes. Leave this set to 0.
3.  An integer for setting the initial semaphore value. In this lab,
    setting this to 1 should suffice.

Once a semaphore is initialized, it can be freely used in your code.
Once you have identified a critical section, you can place a sem\_wait
function call before it. The sem\_wait function accepts a pointer to a
sem\_t type, which determines which semaphore the threads should be
waiting in. If you have more than one critical section, you should also
have more than one semaphore, as each should be filtering threads into
different blocks of code.

At the end of your critical section, you should include a sem\_post
function call, which accepts a single pointer to a sem\_t value. The
pointer passed in should match the pointer passed into the original
sem\_wait call. Don't mix these up, and if you have multiple semaphores
nested together, make sure you are posting them in the correct order.\
If you don't post your sempahores in the correct order, you could end up
in a situation where none of your threads are able to proceed into the
critical section. This often happens when you nest semaphores. A good
rule of thumb is that you should post your semaphores in the reverse
order that you wait them. That means if a thread has to wait at
semaphore 1 and then wait at semaphore 2 before proceeding into the
critical section, the thread inside the critical section should post
semaphore 2, then post semaphore 1 as it is leaving the critical
section.

An example of a semaphore can be seen here:

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wait_here;

void* example(void* args)
{
    sem_wait(&wait_here);

    printf("Sleeping for 2 seconds...\n");
    sleep(2);
    printf("Woke up! Leaving the critical section.\n");

    sem_post(&wait_here);
}

int main(int argc, char** argv)
{
    sem_init(&wait_here, 0, 1);

    pthread_t threads[5];

    for(int i=0; i < 5; i++)
        pthread_create(&threads[i], NULL, example, NULL);

    for(int i=0; i < 5; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
```

When you run this program, you should find that a thread sleeps, and
then wakes up, always in that order. Each thread waits its turn to
center the critical section and thus there should never be a mixing of
sleeps or a mixing of wakes. Every thread should sleep, then wake, and
thus do it five times in sequence. If you comment out the sem\_wait
call, you might find a different behaviour.

Thread Pools
============

A thread pool is a specific type of program which allocates a specific
number of threads to a given data task. Normally thread pools are
created to ensure that only a certain number of threads are created and
running at any given time. This is especially useful in shared resource
systems (like Timberlea) or systems where stability is incredibly
important. It turns out that creating too many threads in rapid
succession has the possibility of overwhelming *any* system, and thus
enforcing some restraint gives you the benefit of increasing the speed
at which tasks are performed without sacrificing system stability.

There are many ways to create thread pools, but we will perform a very
simple pool where we create a queue of Operations and as each thread
finishes execution, we will dequeue another Operation and create a new
thread in place of the old one. This is a very simple model which still
suffers from overhead of creating many threads, but still provides us
the ability to manage the number of concurrent threads very easily.
Other types of thread pools can be more efficient by never finishing
execution of a thread while waiting for more tasks to be given. This has
the additional benefit of not having to constantly recreate threads at
the cost of being more complicated to implement.

The thread pool requires a queue and an array. You will be given an
array size for managing a certain number of threads. You should never
have more threads running than the given integer value. Since you have
no direct means of knowing whether or not a thread has completed
processing, you will need to create an argument struct capable of
reporting when a thread is complete. Under normal circumstances you
could join the thread immediately, but in the case of a thread pool it
would be inefficient to do so. Your goal is to loop through your
currently running threads and any time you find one which has completed
processing, *then* you join it and retrieve its return value before
dequeuing the next Operation and creating a new thread. Every time a
thread completes and its value is returned, you must store the value in
an array list to accumulate all of your data. When all of your
Operations have completed, you will return the array list.

Since the threads are not managed and the order of thread execution is
outside of our control (controlled by the operating system), the array
list's values will be in a somewhat random order. You will need to sort
these values and print them. You should already have the programs
necessary to sort these values. We recommend looking back through
previous lab pipeline results to find a means by which you can sort the
values in your array list. Heaps are a good data structure for sorting,
and binary trees are potentially a useful alternative. We suggest
checking your results from previous pipelines to come up with a way to
sort your integers.

Lab 9 Function Contracts
========================

In this lab you will be responsible for fulfilling two lab contracts:
the **Threads** contract and the **Pool** contract. Each contract is
designed to test you on some of the things you've learned throughout the
instruction portion of this lab.

All contracts must be completed exactly as the requirements are laid
out. Be very careful to thoroughly read the contract instructions before
proceeding. This does not, however, preclude you from writing **more**
functions than you are asked for. You may write as many additional
functions as you wish in your C source files.

All contracts are designed to be submitted without a main function, but
that does not mean you cannot write a main function in order to test
your code yourself. It may be more convenient for you to write a C
source file with a main function by itself and take advantage of the
compiler's ability to link files together by accepting multiple source
files as inputs. When you push your code to Gitlab, you don't need to
**git add** any of your extra main function source files.

For those of you who are concerned, when deciding which naming
conventions you want to use in your code, favour consistency in style,
not dedication to a style that doesn't work.

The contracts in this document are worth the following points values,
for a total of 10.

| Contract  | Points |
| ------    | ------ |
| Threads   | 6      |
| Pool      | 4      |
| **Total** | 10     |

Threads
-------

### Problem

You will create three programs for testing various types of thread
features.

### Preconditions

You are required to write three programs for creating and testing
threads:

1.  **threads**: You will create a program which accepts an array and
    squares each value in the array using threads.
2.  **unsafe**: You will create a program which attempts to increment
    and print a variable without the use of semaphores.
3.  **safe**: You will create a program which attempts to increment and
    print a variable by protecting your critical section with
    semaphores.

Each program must include a relevant .c file, which should contain all
of your function implementations, and a relevant .h file, which should
contain your structure definitions, any necessary typedefs, and all of
your forward function declarations. When you compile, you will need to
include the source file in your command in order to ensure the functions
exist during the linking process. You may include any additional helper
functions as you see fit. Since you are dealing with pointers, you will
need to check all of your pointers to ensure they are not null. Trying
to perform operations on null will lead to segmentation faults or other
program crashes at run-time.

Details on threads and semaphores can be found in the **Multithreading**
section of this document. The **bool** type referenced in this contract
is found in **<stdbool.h>**. You are expected to do basic error checking
(such as checking for null pointers and correct index boundaries).

Your **threads** program must include the following structs (typedef-ed
appropriately):

|Structure Name         |Fields      |Functionality                                                   |
|-----------------------|------------|----------------------------------------------------------------|
|\_Args (typedef Args)  |int\* arr    <br>int start<br>int en|A pointer to an array that you will recalculate the values of.  <br>The starting index to perform calculations from.<br>The ending index to perform calculations to (non-inclusive).|

Your **threads** program must include the following functions:

|Requirement       |Conditions                                                                                       |
|------------------|-------------------------------------------------------------------------------------------------|
|Function          |void\* fill(void\*)                                                                              |
|Input Parameters  |A void pointer to an Args struct.                                                                |
|Return Value      |A void pointer (to fit the thread function requirement). This will be NULL in practice.          |
|Notes             |This should be passed into the thread creation function. This should return NULL. When           <br>executed, this function should iterate through indexes [start, end) in the provided array<br>and square each value before returning null.|

|Requirement       |Conditions                                                                                       |
|------------------|-------------------------------------------------------------------------------------------------|
|Function          |void fill\_memory(int\*, int)                                                                    |
|Input Parameters  |An int pointer to an integer array, and an integer representing the number of threads to make.   |
|Return Value      |None.                                                                                            |
|Notes             |This function should create the correct number of threads N, create an Args struct for every     <br>thread, then divide the array into N equal ranges and execute a thread on each of those ranges.<br>The provided integer array is guaranteed to have 10,000,000 values.<br><br>Be careful when calculating your ranges, as not every value of N will create perfectly even<br>ranges. You may find it necessary to account for the indexes lost to fractions by assigning<br>the final *end* index a value equal to the last index in the list.|

Your **unsafe** program must include the following structs (typedef-ed
appropriately):

|Structure Name           |Fields       |Functionality                         |
|-------------------------|-------------|--------------------------------------|
|\_Count (typedef Count)  |int counter  |A value for counting upward to 1000.  |

Your **unsafe** program must include the following functions:

|Requirement       |Conditions                                                                                   |
|------------------|---------------------------------------------------------------------------------------------|
|Function          |void\* counting(void\*)                                                                      |
|Input Parameters  |A void pointer to a Count struct.                                                            |
|Return Value      |A void pointer (to fit the thread function requirement). NULL in practice.                   |
|Notes             |This should be passed into the thread creation function. This function should                <br>continue to process as long as the counter value is < 1000. If it is < 1000,<br>increment the counter value by 1, then print it to the screen on its own line.|

|Requirement       |Conditions                                                                                   |
|------------------|---------------------------------------------------------------------------------------------|
|Function          |void count\_variable(int)                                                                    |
|Input Parameters  |An integer representing the number of threads to create.                                     |
|Return Value      |None.                                                                                        |
|Notes             |This function should create a single Count struct and create the correct number of threads,  <br>providing the same Count struct as an argument to each thread.|

Your **safe** program must include the following structs (typedef-ed
appropriately):

|Structure Name           |Fields       |Functionality                         |
|-------------------------|-------------|--------------------------------------|
|\_Count (typedef Count)  |int counter  |A value for counting upward to 1000.  |

Your **safe** program must include the following functions:

|Requirement       |Conditions                                                                           |
|------------------|-------------------------------------------------------------------------------------|
|Function          |void\* counting(void\*)                                                              |
|Input Parameters  |A void pointer to a Count struct.                                                    |
|Return Value      |A void pointer (to fit the thread function requirement). NULL in practice.           |
|Notes             |This should be passed into the thread creation function. This function should        <br>continue to process as long as the counter value is < 1000. If it is < 1000,<br>increment the counter value by 1, then print it to the screen on its own line.<br>Unlike the **unsafe** program, you should create a semaphore to stop your<br>threads from printing the counter out of order.|

|Requirement       |Conditions                                                                           |
|------------------|-------------------------------------------------------------------------------------|
|Function          |void count\_variable(int)                                                            |
|Input Parameters  |An integer representing the number of threads to create.                             |
|Return Value      |None.                                                                                |
|Notes             |This function should create a single Count struct, initialize a semaphore for the    <br>counter to be protected by, and create the correct number of threads, providing the<br>same Count struct as an argument to each thread.|

### Postconditions

Your programs should be capable of creating and executing threads. All
of the functions should be capable of executing without crashing.

### Restrictions

None.

### File Requirements

This contract requires you to provide three C source files and three C
header files, named as per the submission instructions. Your header
files should contain your forward declarations, struct definitions and
typedefs, as well as any library imports (includes) you may need. Always
protect your header with a define guard. Your source files must not
contain any main functions, or your program will fail during marking.

In addition to the C files, you will also be required to make a Makefile
capable of producing **threads**, **unsafe**, and **safe** executable
files. Your program will be compiled by executing **make**. Your
Makefile should produce those executables by compiling a combination of
your C files and their related main object files, **threadsM.o**,
**unsafeM.o**, and , which are located in **CI/objects/threads**.

Your source, header, and make files should be placed in the
**Lab9/threads/** directory in your GitLab repository.

### Testing

To test your code, you can compile your source files with the
**threadsM.o**, **unsafeM.o**, and **safeM.o** object files found in
**CI/objects/threads**. Your program can then be executed as normal.
Each object file contains a main function, so you do not need to provide
your own when you submit to GitLab. Using a Makefile for compiling these
files together can save you a lot of time.

The **threads** executable will test your code against a static list of
integers, which you must square using a series of threads. The
**unsafe** and **safe** executables will test your code to determine if
it is outputting the correct order of 1000 integer values. It's
recommended that you test your code thoroughly using the provided main
function objects.

### Sample Inputs and Outputs

A sample output is provided, and is used against the **safe** output in
the pipeline. Your code should minimally be able to complete the test
main functions in the object files, but you may find it more convenient
to test your functions with your own main function first.

Pool
----

### Problem

You are required to design and implement a thread pool.

### Preconditions

You are required to write a program for creating a simple thread pool.
This consists of **pool.c**, which should contain all of your function
implementations, and , which should contain your structure definitions,
any necessary typedefs, and all of your forward function declarations.
When you compile, you will need to include the source file in your
command in order to ensure the functions exist during the linking
process. You may include any additional helper functions as you see fit.
Since you are dealing with pointers, you will need to check all of your
pointers to ensure they are not null. Trying to perform operations on
null will lead to segmentation faults or other program crashes at
run-time.

To create a thread pool, you will follow a similar method as presented
in the calculator contract of Lab 4. To start, you must create an
**Operation** struct which will contain a function pointer to a math
function, an integer value a, and an integer value b. You will then
create an **Args** struct which contains a pointer to an Operation and a
boolean for determining if the thread has completed execution.

In order for your Operation to successfully process, **you** must implement
the functions **add**, **sub**, and **mul**. Each of those functions
accepts two integer values **(a, b)** and returns an integer value. The
operations are always performed **a op b**.

The structure of the **operation\_list** file is such that there are
10,000 operations contained in the file, each on their own line. The
lines are each represented as three integers, where the first integer is
the operation code (op), the second number is the first operand (a), and
the third number is the second operand (b) for that calculation. The op
codes are mapped such that 0=add, 1=sub, and 2=mul. Thus an operation
`0 42 15` would perform 42 + 15, while another operation `2 10 12`
would perform 10 x 12.

The main point of execution in this program is the
**execute\_thread\_pool** function. This function will create all
necessary data structures, initialize an array of thread ID values (your
thread pool), and then read in an operations file. Reading in operations
is performed by the **read\_operations** function and will read the
operations into a queue data structure. Once the Operations have been
stored in the queue, you can repeatedly dequeue the next Operation from
the queue, then create an Args struct. Utilizing your current data
structures in any way you choose, you should create threads for
executing operations until the thread pool is full.

Once the thread pool is full, constantly iterate through the threads and
check if any have completed processing their operation. If they have,
you can retrieve the return value from the thread and store it in an
array list. Once that is done, set up an Args struct, dequeue the next
Operation, and start a new thread in the current thread pool position.
If you create an Args struct for each thread index, you can reuse the
Args struct from a completed thread, as long as you only use the same
struct with the thread in the same index every time. If the queue no
longer has any Operations in it, you can stop iterating through the
pool. Wait for the final return values from each remaining thread,
retrieve them, then return the array list.

Your **compute** function should accept an Args struct, extract the
Operation stored inside, then run the operation function on the two
values stored in the Operation itself. Set any necessary completion
flags and return the calculated value.

The **print\_sorted** function should accept a pointer to an ArrayList,
sort the values stored in the ArrayList, then print them in order from
smallest to largest. You may find it beneficial to use a data structure
from a previous lab (heaps are a good choice). The main function will
expect compareInt and printInt functions to be implemented, even if you
don't use them.

Your **pool** program must include the following structs (typedef-ed
appropriately):

|Structure Name                   |Fields                 |Functionality                                            |
|---------------------------------|-----------------------|---------------------------------------------------------|
|\_Operation (typedef Operation)  |int (\*op)(int, int)    <br>int a<br>int |A function for performing a math operation on a and b.   <br>The first value passed to the op function.<br>The second value passed to the op function.|
|\_Args (typedef Args)            |Operation\* operation   <br>bool is\_complet|A pointer to a valid Operation structure.                <br>A boolean showing if the operation has been calculated.|

Your **pool** program must include the following functions:

|Requirement       |Conditions                                                                                    |
|------------------|----------------------------------------------------------------------------------------------|
|Function          |void\* compute(void\*)                                                                        |
|Input Parameters  |A void pointer to an Args struct.                                                             |
|Return Value      |A void pointer to the integer result of the Operation performed.                              |
|Notes             |None.                                                                                         |

|Requirement       |Conditions                                                                                    |
|------------------|----------------------------------------------------------------------------------------------|
|Function          |bool read\_operations(char\*, Queue\*)                                                        |
|Input Parameters  |A char\* (string) representing a file name, and a pointer to a Queue.                         |
|Return Value      |True if the operations in the file were successfully read and stored in the Queue. Otherwise  <br>false.|
|Notes             |None.                                                                                         <br>|

|Requirement       |Conditions                                                                                    |
|------------------|----------------------------------------------------------------------------------------------|
|Function          |ArrayList\* execute\_thread\_pool(char\*, int)                                                |
|Input Parameters  |A char\* (string) representing a file name, and an integer representing the thread pool size. |
|Return Value      |A pointer to an array list of calculated Operation values.                                    |
|Notes             |None.                                                                                         |

|Requirement       |Conditions                                                                                    |
|------------------|----------------------------------------------------------------------------------------------|
|Function          |void print\_sorted(ArrayList\*)                                                               |
|Input Parameters  |A pointer to an array list of integers.                                                       |
|Return Value      |None.                                                                                         |
|Notes             |This function should sort the values in the array list and print them to stdout.              |               

### Postconditions

Your program should be capable of producing all required structures. All
of the functions should be capable of executing without crashing.
Failure states should be handled by return values. If a function with a
void return type fails, it does not need to be reported.

### Restrictions

None.

### File Requirements

This contract requires you to provide a C source file named **pool.c**
and a C header file named **pool.h**. Your header files should contain
your forward declarations, struct definitions and typedefs, as well as
any library imports (includes) you may need. Always protect your header
with a define guard. Your source files must not contain any main
functions, or your program will fail during marking.

In addition to the C files, you will also be required to make a Makefile
for the **pool** program. Your program will be compiled by executing
**make**. Your Makefile should produce a **pool** executable file by
compiling your code with the **poolM.o** file located in
**CI/objects/pool**.

Your source, header, and make files should be placed in the
**Lab9/pool/** directory in your GitLab repository.

### Testing

To test your code, you can compile your source files with the
**poolM.o** object file found in **CI/objects/pool**. Your program can
then be executed as normal. The object file contains a main function, so
you do not need to provide your own when you submit to GitLab. Using a
Makefile for compiling these files together can save you a lot of time.

### Sample Inputs and Outputs

A sample output is provided and is compared when executing your
pipeline. The main object file for this program will execute your thread
pool and determine if the values you output are correct. Your code
should minimally be able to complete the test main function in the
object file, but you may find it more convenient to test your functions
with your own main function first.

Submission
==========

Required Files
--------------

Each file must be contained in the directory listed in the structure
requirement diagram below. These files include:

1.  pool.c
2.  pool.h
3.  threads.c
4.  threads.h
5.  safe.c
6.  safe.h
7.  unsafe.c
8.  unsafe.h
9.  Makefile (for pool)
10. Makefile (for threads)

You may submit other files that your Makefile needs to function
correctly. **Note that the above files are simply the minimum
requirements to pass the pipeline.** Any additional files will not count
against you. Be careful with what you upload, as it could cause issues
with the pipeline (such as hiding changes from your Makefile). You
should try your best to only push the files your program needs to
compile and execute.

Submission Procedure and Expectations
-------------------------------------

Your code will be submitted to your Lab 9 GitLab repository using the
same method as outlined in the Lab Technical Document. Refer to that
document if you do not remember how to submit files via the GitLab
service. A link to your repository can be found in the **Lab9** subgroup
of the CSCI 2122 GitLab group
[here](https://git.cs.dal.ca/courses/2022-winter/csci-2122).

As mentioned in the Lab Technical Document, we will provide you with a
CI/CD script file which will help you test your submissions. The .yml
file containing the CI/CD test script logic, and any other necessary
script files, are available in your repository at all times. You are
free to view any of the script files to help you understand how our
marking scripts will function. We make extensive use of relative path
structures for testing purposes, which is why strict adherence to
directory structure and file contents is such a necessity. Also remember
to check your pipeline job outputs on the GitLab web interface for your
repository to see where your jobs might be failing.

Remember to follow the instruction guidelines as exactly as possible.
Sometimes the pipeline scripts will not test every detail of your
submission. **Do not rely on us to perfectly test your code before
submission.** The CI/CD pipeline is a great tool for helping you debug
major parts of your submissions, but you are still expected to follow
all rules as they have been laid out.

Submission Structure
--------------------

In order for a submission to be considered valid, and thus gradable,
your git repository must contain directories and files in the following
structure:

```
Lab9/
 ┣ pool/
 ┃  ┣ pool.c
 ┃  ┣ pool.h
 ┃  ┗ Makefile
 ┗ threads/
    ┣ threads.c
    ┣ threads.h
    ┣ safe.c
    ┣ safe.h
    ┣ unsafe.c
    ┣ unsafe.h
    ┗ Makefile
```

As with all labs, accuracy is incredibly important. When submitting any
code for labs in this class, you *must* adhere to the directory
structure and naming requirements in the above diagram. Failure to do so
will yield a mark of 0. That said, in this lab, your directory structure
requirement is to *minimally* have these files, but you may have more as
you require.

Remember to remove **Lab9/delete\_this\_file** from your repository
using **git rm** to avoid any pipeline failures.

Marks
-----

This lab is marked out of 10 points. All of the marks in this lab are
based on the successful execution of each contract. Any marking pipeline
failures of a given contract will result in a mark of 0 for that
contract. Successful completion of the various contracts will award
points based on the following table:

| Contract  | Points |
| ------    | ------ |
| Threads   | 6      |
| Pool      | 4      |
| **Total** | 10     |
