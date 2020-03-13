measure: pixels (maybe I will make it twice bigger)
array of dots -- 19 x 25
dot 2 x 2
every dot is the center of square cell 12 x 12
this cells intercept with 4x12 rectangulars
blue margin is 4 pixels width
original pacman was 14x14
I want it be 12x12
original field is 164x212
let's say that "pacman is in the cell" if it fills at least 8x12 (>= 2/3) of cell
then at each time pacman will be at least in one cell, and sometimes he will be in two cell at a time (both 8x12 and 8x12)
we can precompute the number of cell
do we really need this concept?
Let pacman eat a dot if it fills a half of the cell
do the direction matter?
if we move from left to right and we hace a cell above, then we will never fill the half of it
we can't fill the half of a cell with our back if we not already have eaten the dot with our mouth
so the direction doesn't matter
so the field is like
first line, number is cell's count
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0 0 0 0 0 0 0 0 0,1 0,1 0,1 0,1 1 1 1 1 1,2 1,2 1,2 1,2 2 2 2 2...
0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,1,3 0,1,3 0,1,3 0,1,3...
0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,1,3 0,1,3 0,1,3 0,1,3...
0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,1,3 0,1,3 0,1,3 0,1,3...
0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,3 0,1,3 0,1,3 0,1,3 0,1,3...
pacman is in the cell ~= pacman's center is in the cell
let's redefine "pacman is in the cell"
"pacman is in the cell" when its center (2 x 2) is in the cell
there should be at least one cell, could be two
if we will increase the size of map, we still can consider 2 x 2 center
should we check all for pixels? Maybe
each pixel could be in two cells too
we can check only top left and bottom right pixel of center
we can have mapping array
how should it be organized?
let's separate 12x12 cell to 9 4x4 cells
each can be a wall, a dot or an empty space
or it will be better to have 2x2 dots cause I think pacman should move to 2 pixels every time
or 1?
ok. let's forget about FPS right now
```
cell(x, y)
	// remove margin from x, y by substracting

```
Let's use 12x12 cell logic only for dot consuming
Wall collision will be processed in pixels mode?
Let's have pixels map with walls
164x212 = 34 kb if 1 byte used
How smooth movement will work?
Current direction and next direction
Five types of movement: STAY, LEFT, RIGHT, UP, DOWN
Initial current, next = STAY
Direction clicked -- it set to next
if current is stay -- it set to next, next set to stay
If current not stay and we can move to next direction -- set current to next, next to stay
If current not stay and we cannot move to next -- decrease next expire (2 or 3 by default), current remains the same (smooth movement)
Move to current. If cannot move, set current to stay.
Do we really need expiration? I think not.
Pacman is descrideb by its x, y, current and next state. I think it could be the movable object class.
Should there be STAY state? Maybe not
methods: 
* set_move_command -- set next if button clicked (we will think about enemies later)
* move -- check if we can move to next. If yes, set current to next. Else current remains the same. Move to current, if possible.
Moving object: x, y position in pixels of top left corner, width and heigth of object in pixels. some getters and setters, maybe texture. move method.
For move object should refer to our map field
Maybe map field should be read from png itself.
No, round boundaries will break the logic.
Three grid states: empty, wall, enemy. We can forget for the third one for now.
Fill this thing by hand? 41x53 -- ok, possible. Also scalable.
Four grid cell state -- teleport. Also later
