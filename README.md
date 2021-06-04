# rt_indi

## MacOS - ![Build](https://github.com/dolovnyak/rt_indi/actions/workflows/c-cpp.yml/badge.svg?branch=master)

RT is a rendering project of school 21(Ecole 42).
------------

Implements:
---------
- Lighting models: Phong, Pathtracing.
- Intersection methods: 
  - Raytracing (sphere, cone, cylinder, plane).
  - Raymarching (torus, hyperboloid, parallelogram, mudelbulb fractal).
- GPU acceleration via OpenCL.
- Supersample anti-aliasing
- Post processing effects: gauss blure, anaglyph, monochrome, ordered dithering
- Textures.
- Pathtracing effects: specularity, transparency, refraction, glow.

Build and run on `MacOS`
------------
```
make
./RT 'path_to_map' 'path_to_texture'  

# Examples of maps and textures are stored in a folder 'json'.
```

## Controls
`ESC` - Close the program.  
`W`/`A`/`S`/`D` - Movement in space.    
`Hold the right mouse button and move it` - Movement in space.   
`UP`/`DOWN`/`RIGHT`/`LEFT` - Camera rotation.  
`Hold the left mouse button and move it` - Camera rotation.   
`Mouse wheel scroll` - zoom into / zoom out.   
`Z`/`X`/`C`/`V`/ - effects on / off.   
`2`-`4` - fsaa on(mouse event off it).   
`L` - path trace on / off (mouse event off it).   
`0` - screen shot.   
`ESC` - quit programm. 

Screenshots of scenes:
-------------
### Scene with all objects (Phong shading via Raytrace and Raymarching algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/raw/master/images/12.10.2019%2016.17.47.png)
### beautiful scene (Path trace algorithm via Raytrace algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/blob/master/images/14.10.2019%2018.56.08.png)
### 4_speheres_1 scene (Path trace algorithm via Raytrace algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/blob/master/images/29.10.2019%2015.13.58.png)
### 4_speheres_2 scene (Path trace algorithm via Raytrace algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/blob/master/images/29.10.2019%2015.13.18.png)
### Glass scene (Path trace algorithm via Raytrace algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/raw/master/images/12.10.2019%2016.19.50.png)
### Stained glass scene (Path trace algorithm via Raytrace algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/raw/master/images/12.10.2019%2016.22.34.png)
### Student hostel scene (Path trace algorithm via Raytrace algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/raw/master/images/12.10.2019%2016.27.14.png)
### Mandelbulb scene (Phong shading via Raymarching algorithm)
![Image alt](https://github.com/dolovnyak/rt_indi/raw/master/images/12.10.2019%2016.29.09.png)

## Authors
IlyaSviridov - https://github.com/IlyaSviridov   
dolovnyak - https://github.com/dolovnyak   
godflight - https://github.com/GodFlight   
pgilly   
