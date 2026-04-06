<!-- 1. Change collisions in raycast to have last bit be the collision bit
2. Integrate RNG algorithm, initialize with seed, add damage and health values to enemy, make hitreg deduct health, implement respawn system with RNG
3. Gates might have two rays per column
I think gates might be a system where, when its a gate, you calculate two rayhits: 
	When its open, you draw the background then the foreground, otherwise just draw the foreground.
	This however assumes that behind the gate there won't be another gate. If it does, it's actually N rayhits
	For deferred rendering, this is cancer -->

<!-- 1. Polish draw sprites function to take y offset parameter -->
TO REMEMBER:
- Screen width and height must be greater than render height/width
<!-- - Parse needs to validate map first to see how many textures it will need, to adjust match target -->
<!-- - ^^^ Optionally only fail on mandatory textures not loaded, then always load empty textures (PINK, for example) -->
<!-- - Enemy init copies , enemy has a texture ptr -->

<!-- - Create a read texture function that scales with nearest neighbour to nearest power of two -->
<!-- - Transparency color needs to be only one, and processed in parse (needs to be done for other textures too) -->
- Move draw_radar to draw_overlay
- Clip space function that returns a bounds
- Adhere to one standard of rendering, either state functions choose what texture is loaded (adds t_mat32 to struct) or render functions determine texture from state (more branches)
- Review diagonal movement
- Fix respawn
- Add some random lateral movement
- Add readme
- Add enemy dist drawcull
- DT acceleration
- Add grounded enemies (add vars to enemy struct like zheight, etc etc)
- Radar hands draw on top (bug)
- Enemies deal damage based on enemy dist