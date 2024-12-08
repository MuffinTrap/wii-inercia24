# wii-inercia24
Wii space demo for Inercia 2024
'Marmot Spacelines 2024'

## Links etc

### Video captures
- [Final version : the compo entry](https://youtu.be/RJLuqgfDlUE)
- [Alpha version](https://youtu.be/SGji5DPvcKo)
- [Scene durations cut](https://youtu.be/HF8GMk6vles)
- [Rough first cut](https://youtu.be/ZYgxOfbzJiw)


### Music
Is too big for github as a wav: [Get it from here](https://drive.google.com/file/d/1VkxwGh8k3hsZa7Vxt1_ewXTYOgXAhrci/view?usp=sharing)


### References
- [Reference images from Elite Dangerous](https://imgur.com/a/inercia-space-reference-klIk9j2)
- [Storyboard](https://imgur.com/a/Z9nxCJp)
- [Storyboard palette](https://lospec.com/palette-list/ink)
- [Dot font](https://www.1001fonts.com/led-counter-7-font.html)
- [Spaceship reference from Gundam Wing](https://imgur.com/a/9OB9rev)
- [Reference for camera shake](https://www.youtube.com/watch?v=Qedlb82Ru6w)
- [Earth image from NASA](https://images.nasa.gov/details/PIA00122)


## Development
- [Trello](https://trello.com/b/iNhn7PfS/wii-inercia24)
Use mtek-gdl branch **ufbx**

**external libraries**
- glm

### Rocket track notes

default / **scene**
This controls what is rendered and what the camera targets.

0. Spaceport, terrain and ship. Target: ship
1. Departures hall and display. Target display
2. Terrain and ship. Target: ship
3. Earth bg image and ship: Target: ship
4. Hyperspace gate and ship: Target: ship

**camera**
- x, y, and z are offsets to the target.
- posX, posY and posZ are the camera's position if orbitOn is 0
- orbitOn:  0: camera stays still. 1: Camera keeps distance to the target
- orbitX and orbitY: Angle around target on X and Y axiis, in degrees
- speed: How fast camera moves to new position.
- interpolateOn: 0: Camera changes position instantly, 1: camera interpolates to new position
- wiggleSpeed: How fast the camera shakes to simulate hand held camera. 0: no wiggle. > 0: how often new offset is randomized

**door**
- hangarX and elevatorX: moves both doors on X axis to same direction
- hangar and elevator: moves both doors on X axis, but to opposite directions