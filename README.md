# MoonPhase
A simple astronomical simulator that attempts to predict sunrise, sunset, moonrise and moonset for two locations, pausing when the moon is visible to both so the time can be checked.

The model here is simple, orbits are considered exactly elliptical with the angular velocity being calculated in terms of the polar angle around the orbit, from Kepler's third law. This could be viewed as a differential equation and then solved with standard techniques, but high accuracy isn't a priority here as the timesteps used are tiny comparatively. The method here is equivalent to the explicit Euler method. 

There is also the terrestrial part of the code, which calculates how a location on the Earth moves over time as the Earth rotates. This is also simple, the intial spherical polar position is coded in, and then the axis of rotation and rotation period is too. This lets the spherical polar position to be calculated at any time.

To check if an object is visible from a location, the location's spherical position is first converted to cartesian. Then, a vector is drawn from the Earth to the object. A dot product between these two is taken, and if it lies within certain bounds (i.e the angle is less than 90 degrees) then it is considered visible. In reality this isn't quite 90 degrees as refraction is taken into account too, but the different is minor.

I've randomised the locations being used so I'm not certain where they point to currently. When I had it pointed to my home address, it was accurate to around half an hour at some times of the year but would be a few hours off at others. I think even this simple idea should be more accurate than that, so I expect there are errors in the implementation somewhere.

## Dependencies
Depends on CImg 2.9.2 or later. 

## Compiling
To compile, clone the repository and set it to the current working directory. Then run
```sh
g++ -O3 moon.cpp -lX11 -pthread -o moon
```
to compile. It can also be compiled for Windows by switching `g++` with MinGW, and by switching `-lX11` with `-lgdi32`

## Usage
There aren't any configurable options currently, but ideally it would be possible to enter a location on Earth and a time/date and then let it run. These are all hard coded currently and are a huge pain to look up and convert to the correct format.

## Screenshots
![](https://github.com/Asa-Hopkins/MoonPhase/blob/main/screenshots/Screenshot.png)
Clearly, not to scale.
