This is my attempt solution of the "ISS Longeron Challenge" from
http://www.topcoder.com/iss/ backed by NASA. Unfortunately, I knew too late
about the challenge, just 2 days before finishing. While I knew I had
extremely low chance of competing, just now, 24 hours after starting coding,
I gave up and decidade to release my far from complete attempt solution.

I do so because I belive mine idea to be a promising approach. I have no clue
on how to organize the movement of the panels to have the optimum power
generation, but considering you can precompute the solution of the problem, I
could use genetic algorithm and an bit of simulation to let the computer find a
very good solution for itself.

I consider an array of 92 states, one for each minute, plus a yaw angle, to be
an individual of my genetic algorithm. Running the algorithm for enough
generations would lead to near optimum solution. 

The mutations needed by the genetic algorithm could be accomplished by shifting
the states by a few minutes should be one of the possible mutations and by
incrementing/decrementing the angles by some degrees inside the allowed range.
Other, more sofisticated mutations, can be thought, like rotating a SARJ or BGA
by any ammount, and fitting the previous and subsequent per-minute states to
accommotate the change, resulting in a bigger abrangence mutation. In face, due
to the size of the individual (large count of floating point numbers), mutations
in various levels of abrangence may be desired.

I anticipate no quirks in the crossover procedure, what leaves us with the
ranking step of the genetic algorithm: besides the oficial ranking formula of
the challange, there should be a hint to the evolutive process that the solution
is about to fail due to the longeron constraint. Maybe something like: for each
BGA, if the longest bad-minute streak is greater than 15 (thus, near the maximum
of 20) the difference is accumulated. This difference is then somehow used to
decrease the solution's score. Maybe this is good, maybe it is not, and it is
better to use the standard procedure of abruptly zeroing the score if any streak
for any SAW is greater than 20.

But to use genetic algorithm we must test the individual's rank, and test it
fast! To test one solution in my machine using the provided Java (eek!)
visualizer tool, it takes about 10 seconds. That is too slow, because of
shadowing simulation. That where OpenGL comes in: we shall use the video card
to make the shadowing simulation. Just render ISS as looking from the sun and
use a specific key color for the solar cells. Count the solar cell pixels in the
resulting image, multiply by a constant and you have the power! Longerons can 
be drawn as GL_LINES, each with a specific key color. Count the pixels visible
from the sun and you have if it is shadowed or not. The size each longeron and
solar cell should have if unshadowed, in pixels, is easily estimated by applying
the transformation matrix to lines and rectangles, respectively.

I lack the motivation to continue the task, and I am abandoning it as it is now:
generating one random solution, and animating it in OpenGL (hopefuly, from sun's
perpective)using dubious joints coordinates, calculated crudely from the .stl
file. For some (easily fixable) reason GCC refuses to compile the code, so I am
using Clang.
