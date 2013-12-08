# Black Shades
By David Rosen

## Building

### OS X

Use [homebrew](http://mxcl.github.com/homebrew/) to install `alut` and `sdl_image`, and then just type `make -f Makefile.osx`.

### Linux

Make sure to install necessary libraries `libalut` and `libsdl-image` and then run `make -f Makefile.linux`.

## Controls
* WASD = walk
* shift = run
* mouse = look
* control = crouch/zoom
* click = fire (while aiming) or smash (while running) or pick up gun (while crouching over a body and not aiming) or disarm (while not aiming)
* q = aim or un-aim (important for picking up guns)
* r = reload
* e = psychic aim
* z = toggle soul release
* space = dive (while running forwards)

## Debug mode

* tab = 3rdperson toggle
* f = Force Push :)
* shift-x = switch weapons

That's it!

## Instructions
Try to keep your VIP (the guy in white) alive as long as possible. The assassins will all try to shoot or stab him to death. You must do all you can to prevent this. Your reputation has preceded you, so the VIP has absolute confidence in your abilities and will completely ignore all the assassins. When an assassin is aiming at your VIP with a gun, you will psychicly see a line of sight extending from him to your VIP. This line will narrow and redden until it disappears and the assassin fires. Depending on the situation it may be best just to shoot the assassin(s), or to dive and tackle the VIP to the ground to avoid the bullet. Unfortunately your psychic powers do not show the line of sight of knife-wielding assassins.

If you are feeling overwhelmed you can use psychic aiming to temporarily speed up your thought processes and aim your shots better. If there are no visible enemies you may want to release your soul and look for nearby enemies. When your soul is released your VIP pulsates between blue and red, civilians between black and red, and assassins are solid red.

If you die or your VIP is killed, the level restarts. If you are are interested in the theory behind this: you are a psychic bodyguard, so you can see small distances into the future. The failure only occurred in a possible future which you are now going to try and avoid.

The scoring system consists of: 

* 150 points for a successful disarm
* 300 points for destroying a zombie (by blowing its head off)
* 100+50x points for completing a mission where x is the mission number (i.e. 450 points for completing mission 3)
* 75 points for incapacitating an assassin
*     +50 if he had a knife
* -300 points for hurting a civilian
* -200 points for allowing the VIP to die
The penalty for failing to protect the VIP is halved if you kill the assassin.

You can edit levels by setting "Custom levels" to 1 in the config and editing the customlevels.txt file in the data folder.

## Weapons
* **Bare Hands**: Smack people with them. Or if you want to be nice, walk or stand (don't run) near somebody with a gun and take it away.

* **Knife**: Like bare hands, but deadly and with longer range. Look out for knife-wielding assassins, they are the most dangerous.

* **Handgun**: One shot with this should be enough to incapacitate any human target, but they may remain conscious for a second (this can be bad if they are about to stab your VIP, so aim for the head).

* **Magnum**:  Not as much ammo as the vanilla handgun, but one shot is an instantaneous kill.

* **Assault Rifle**: This weapon has quite a kick and is bigger and more unwieldy than the handguns, but it has a large magazine and can fire quickly if necessary.

* **Sniper Rifle**: A bit more powerful than the magnum, with a scope. Very difficult to aim unless you are zoomed in (zoom by holding the control key).

* **Shotgun**: Aim and shoot. Very powerful but somewhat inaccurate.

* **Grenade**: Hold down the mouse button to take out the pin, release the button to throw it. Crouch to change your mind and put the pin back in. You can knock people out if you hit them in the head, or of course you can just blow them to pieces.

## Known Bugs

* It doesn't work too well under classic emulation
* Occasional collision detection issues
* Turning virtual memory off can cause speed issues

## History
* 12-8-2013 removed compiler warnings and added dual support for Linux and OS X
* 11-6 to 11-9 main menu, score, etc.
* 10-9 to 11-6 better ai, gibbing, added zombies, weather, knife, shotgun, misc. stuff
* 10-8 shooting delay/ effect
* 10-6 added rich vip guy
* 10-5 bug fixes, gibbing, slomo psychicness
* 10-4 bug fixes, blood toggle, environments, falling damage
* 10-1 worked on menu, pathfinding
* 9-27 added black shades, bug fixes, added some IDG sounds, better collision, config.txt
* 9-22 bug fixes
* 9-21 mouse smoothing, better aiming, different blood
* 9-20 fixed street clipping, grenades working, more blood
* 9-19 fixed grenade faces, fixed some assassin AI, fixed penetration bugs
* 9-18 started assassin AI, some more assassin AI, some work on grenades
* 9-17 faster aiming/crouching, VBL sync, slightly improved control sensitivity stuff
* 9-16 glock added, penetrating sniper rounds, better scope, different zooming
* 9-15 suicide, improved bashing, npcs can carry guns, reloading, headshot sound, camera positioned better, handgun
* 9-14 rifle bashing, aim modifications, redder headshots, speed
* 9-13 everything in one folder, stationary 3rd person cam toggle, visible bullets, some sound stuff
* 9-12 crouching, duck to snipe, pain animations
* 9-11 more fluid aiming, different costumes, assault rifle, health system, impact reaction, kill counter
* 9-10 sniper rifle scope, bullets, sounds
* 9-9 bug fix
* 9-8 sniper rifle added (fires blanks, can't zoom), ragdoll system improved, ragdoll sounds
* 9-7 smooth transition to pointing arms, walking, better running, skeletal 'ragdoll' system + collisions
* 9-6 fixed sound bug
* 9-5 Added distance people sprites (very far view distance, faster also) Added limit to # of people per block, more even spacing, added pointing arms (pistol aiming without the pistol)
* 9-4 Added random civilian placement system (populated entire city instead of a block)
* 9-3 Too Busy
* 9-2 Too Busy
* 9-1 Too Busy
* 8-31 Started basic civilian AI
* 8-30 Fog fixed on ground and sky, better skeletal animation, pre-calced rotations for joints
* 8-29 Real Life intervened, no time
* 8-28 Air control added, some collision fixed, occluding people, sky plane, ground does not occlude
* 8-27 looking up/down changes animation, editors updated to CW 8, animation editor tweaks, optimised a bit
* 8-26 look at previous soul position when going back into body, sound fx added, movement speed tweaks, can't see body while soul is released
* 8-25 black sunglasses idea(prevent visions) "dark shades" or "black shades" name 
* psychic vision effect added
* better collision added
* 8-24 working under CW 8
* 8-23 collision detection on street (with sidewalk+buildings)
* 8-22 collision detection models + firstperson view
* 8-21 psychic bodyguard idea
* 8-20 basic city engine, skeletal animation from glfighters2 pre-alpha, player+building models, jogger group

Main menu music is included with permission from musician John Graham, Copyright 2002.
Due to legal difficulties all other music is composed by me at the last minute based on some nice loops made by Carlos Camacho, hence the 'programmer music'.
Also many thanks to my other friends and beta testers, and to artist David Drew for modelling the assault rifle, sniper rifle, handgun, shotgun and grenade.
