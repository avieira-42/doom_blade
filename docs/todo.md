<!-- 1. Change collisions in raycast to have last bit be the collision bit
2. Integrate RNG algorithm, initialize with seed, add damage and health values to enemy, make hitreg deduct health, implement respawn system with RNG
3. Gates might have two rays per column
I think gates might be a system where, when its a gate, you calculate two rayhits: 
	When its open, you draw the background then the foreground, otherwise just draw the foreground.
	This however assumes that behind the gate there won't be another gate. If it does, it's actually N rayhits
	For deferred rendering, this is cancer -->

<!-- 1. Polish draw sprites function to take y offset parameter -->

TO REMEMBER:
- Map width and height must be greater than 3x3
- Screen width and height must be greater than render height/width
- 