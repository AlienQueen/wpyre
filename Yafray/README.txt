******************************************
* Yet Another Free Raytracer For Windows *
* Version 0.0.8, threads and EXR enabled *
******************************************

TABLE OF CONTENTS
-----------------

0 - Introduction
1 - Installation Notes
2 - YafRay Options
3 - How to set YafRay in PATH
4 - Changelog

==================
0 - Introduction
==================

YafRay comes as a command line raytracer and dynamic library for
integration into other applications (for example Blender).
Currently, YafRay does not feature a GUI.
Visit www.yafray.org for more information.

The command line syntax is:

yafray [options] file.xml

==================
1 - Installation Notes
==================

If you're using YafRay since first versions, please be careful with a
possible deprecated yafray.bat in your \windows folder.
It could make YafRay fails. You have to delete it.

To run the latest version, you need MSVCR71.dll because from 0.0.8 on
Yafray uses the dynamic runtime (mainly because of OpenEXR).
MSVCR71.dll is part of the Microsoft .NET Framework 1.1 available from:
http://msdn.microsoft.com/netframework/downloads/framework1_1/


==================
2 - YafRay Options
==================

-c N 	-> Where 'N' is the number of CPU's to use 
	   (for multi-proccesor computers)
-z	-> Use net optimized (under mosix clusters)
-p PATH -> Where 'PATH' is the installing path of YafRay.
            (By default YafRay uses Windows registry to locate grammar file
            and plugins, so this option is not needed (If all runs fine ;-) ).
-v  	-> Shows YafRay's version.	

NEW since 0.0.6: 

-r min_x:max_x:min_y:max_y 	-> Render region. Values between -1 and 1. 
				   Whole Image is -r -1:1-1:1

To display the options, just type yafray and press ENTER


==============================
3 - HOW TO SET yafray.exe IN PATH
==============================

YafRay Installer doesn't add yafray executable to your PATH environment variable.
So if you wish execute YafRay from any directory you need add YafRay's installation
directory to the PATH variable yourself.


I will assume that YAFRAY_DIR contains the YafRay installation directory. 
If example, if you have installed YafRay in c:\Program Files\yafray you must
change YAFRAY_DIR with c:\Program Files\yafray in the lines below.

* Setting the path in windows 95/98 
-----------------------------------

- You need to be able to see hidden folders 
- Click on "My Computer" 
- Select "View" from the menu 
- Select "Folder Options" 
- In the window that pops up: 
	- Select the "View" tab 
	- Select "Show all files option" 
	- Select "Ok" 
- Editing AutoExec.bat 
- Open C:\AutoExec.bat with Notepad, Wordpad or other text editor
Two options:

	1.- Append YAFRAY_DIR to the PATH variable.	

	2.- Add the following line to the bottom of your file: 
	SET PATH="YAFRAY_DIR";"%path%"

- Save and close 
- RESTART Windows 


* Setting the path in windows 2000/XP: 
-----------------------------------

- Right click on my computer. 
- Select "Properties". 
- Click "Environment Variables". 

If you have Administrative privileges then you can Edit the Path in 
the System Variables box. If you do not have Adminitrative privileges
then you can Edit the PATH in the user variables box. 

- Select the variable "PATH" or "Path". 
- Click "Edit...". 
- In the box "Variable Value:" go all the way to the end (as far right 
  as possible) you can do this by clicking in the box and then pressing
  and hold in the right arrow on the keyboard. Once you are at the end
  you will need to type the path to the executable program. You will 
  need to type (without the quotes): ";YAFRAY_DIR". 
 
- Click "Ok". 
- Click "Ok". 
- Click "Ok". 

================================
4 - CHANGELOG 
================================


	- (eeshlo) Bugfix for wings export which doesn't define uv-ccords for all faces.
	
	- (jandro) Thread Support for SMP systems is back

	- (eeshlo) Added rotation matrix option for background 
	  image. Set with parameters m00, m01, m02, m10, m11, etc.. 

	- (eeshlo) Added bumpmap option to the phongnode. It now has a 'bump' shader input 
    and the amount of bumpmapping can be controlled with the 'normal' 
		parameter, 0 is off, can be positive or negative.

	- (eeshlo) Pathlight has a new 'mode' parameter, which can be set to 'occlusion', 
		in which case instead of full Global illumination it will sample occlusion
		only. Both 'hemilight' and 'pathlight' in 'occlusion' mode have a new
	  parameter 'maxdistance', with which the maximum distance to
	  detect occlusion can be set. This means that like Blender's
	  Ambient Occlusion, it can now be used in interior scenes as well.


	- (eeshlo) Major rewrite of the Blender shader, also losing a bit of
	  backward compatibility, more of the current (2.37) Blender
	  shading system is now supported. All texture modulation modes
	  with the exception of 'Amb' and 'Translu' are now supported. All
	  blending modes are supported. All diffuse and specular models are
	  supported.  'Checker' clipmode was added. Most procedural
	  textures are supported, though stucci does not (and cannot)
	  totally work the same as in Blender, and related to that, for
	  similar reasons, the Blender 'nabla' bumpmap parameter is not
	  used either, it will match Blender best if the default setting of
	  0.025 is used.  Ramps are partially supported, as for the what,
	  how and why, as well as more info on the current supported
	  Blender features, see these links to the Blender commit logs:
	  http://projects.blender.org/pipermail/bf-blender-cvs/2005-May/003656.html
	  http://projects.blender.org/pipermail/bf-blender-cvs/2005-May/003665.html
	  http://projects.blender.org/pipermail/bf-blender-cvs/2005-May/003692.html
	  http://projects.blender.org/pipermail/bf-blender-cvs/2005-May/003696.html

	- (eeshlo) The brdf models implemented also includes two anisotropic models
	  as well, the Ward elliptical model (used for Blender 'WardIso' as
	  well with both u & v roughness set to the same value) and the
	  Ashikhmin model. But since sampling could not be implemented in
	  the current architecture  making everything too complex and
	  inefficient than necessary, reflections are not currently
	  possible, only basic shading is implemented, making them a lot
	  less usable anyway. Better saved for the redesign.  Anisotropic
	  specular needs tangent vectors and currently these are derived
	  from either uv or orco cooridinates. in Blender using
	  nurbs/bezier surfaces with 'UV orco' enabled will work best,
	  while regular uv mapping has to be specifically set up for this
	  to work properly.  Depending on the uv mapping from 3d view
	  window, 'cylindrical' and 'sphere' mapping will work best. But
	  since all that is only accesible through xml editing (as far as
	  Blender is concerned) that is probably of no use to most people
	  anyway.  Besides that, there are some more brdf models in there
	  as well currently unused, like Lommel-Seeliger and a general
	  microfacet model with Blinn/Beckmann/TorranceSparrow &
	  TrowbridgeReitz distributions, but these might not really be
	  useful anyway at this point.	All that is almost a direct
	  copy&paste of my renderer own code, and still a bit unoptimized
	  probably, some leftovers around that could also be removed for
	  current usage.

	- (eeshlo) Some changes to the OpenEXR output, an extra flag has been added.
	  'exr_flags' that can contain a number of keywords to control the
	  exr output. The input string can contain any of the keywords
	  'float' for full floating point output, 'zbuf' to also save the
	  zbuffer with the image, and any of the following to set the
	  compression type: 'compression_none', 'compression_piz',
	  'compression_rle', 'compression_pxr24', with zip compression
	  ('compression_zip', not to be confused with 'piz') being the
	  default if not specified.  For the advantages/disadvantages of
	  each, refer to the OpenEXR documentation.

	- (eeshlo) Image textures, the interpolation has been changed, instead of
	  the former on/off switch, it can now be set to three types of
	  interpolation, 'none' for no interpolation, 'bilinear' (default)
	  for bilinear interpolation, and for special cases 'bicubic'. The
	  latter is a bit expensive and is best saved for things like low
	  resolution backgrounds, or highly zoomed in textures, where it
	  can considerably improve the otherwise blocky artifacts of
	  bilinear interpolation.

	- (eeshlo) Lights, point/soft/spherelights now have a simple glow option, a
	  simple way to make lights visible. The basic effect looks like
	  light in fog. 'glow_intensity' controls the intensity and size of
	  the glow, generally very low values (usually less than 0.01) are
	  enough, since it also depends on the light intensity itself.
	  'glow_offset' affects the central highlight of the glow, the
	  higher this is set, the less 'peaked' the glow will be, untill it
	  almost fades out and only a faint foggy effect around the
	  lightsource is left. 'glow_type' can be set to two types, though
	  they can look similar, type two is best, but also a bit slower.
	  First type is a simplified own hack, the second type is based on
	  an idea described by Han-Wen Nienhuys in RTNews7v3 (also used in
	  MegaPov).

	- (eeshlo) The spherelight when used with the global photonmap should now
	  work better since it now samples from the sphere surface only, so
	  using it with an emmiting object (sphere) at the same position
	  works better, similar to the arealight.

	- (eeshlo) For other then Blender users, the 'conetrace' sampling has been
	  improved a bit. Sampling an unweighted phong brdf, what otherwise
	  would have been used in the brdf code. At least it has a function
	  now there.  For Blender users who have no problem to edit the xml
	  and also like to have access to the other blocks like
	  'conetrace', the blendersahder now also has the possiblity to use
	  an 'environment' shader, similar to the 'phong' block.

	- (eeshlo)  The renderer block has an option 'clamp_rgb', which is to be used
	  for normal output (doesn't matter if set for float ouput anyway,
	  is bypassed) so that anti-aliasing around fast high contrast
	  changes in the image will work better, it also tends to speed up
	  AA as well, having to do less work. This is not advisable when
	  using the DoF bokeh options, since it tends to weaken the lens
	  shape details.

	- (eeshlo) Transmit absorption and dispersion. Available in
	  both the generic shader and the blendershader.  Lots of hand
	  waving here..., take everything I write below with a giant
	  *boulder* of salt ;) 'dispersion_power' controls the amount of
	  dispersion, the higher, the more dispersion, but for realistic
	  results, very low numbers should be used ( > 0.05 ). This value
	  is related to real world data available (or can be derived) from
	  several glass catologue's like the famous 'Schott' catalogue. The
	  number specifies the difference in indices of refraction of the
	  Fraunhofer F and C lines (nF-nC, the denominator to find the
	  'Abbe' number).  'dispersion_samples' controls the amount of
	  spectrum samples taken. Currently the spectrum is sampled from
	  the wavelength range of 400 to 700nm, divided in
	  'dispersion_samples' of regions. So the more samples, the more
	  accurate the result. Don't use too low values, 7 to 10 is about
	  the minimum, since these are discrete samples (unless using the
	  jitter option), using to little samples will result in objects of
	  a single color, or at least not representing the objects true
	  color. Though for realistic materials, 25 maximum should be more
	  than enough, but for extreme 'look-at-them-purty-colorz'
	  dispersion settings, you might need a lot more.  When using low
	  sample settings, setting the 'disperion_jitter' option to 'on'
	  will jitter the spectrum sampling for a better spread, but also
	  adds noise.  'absorption' sets the absorption color/coefficients,
	  this relates to what is known as Beer's law (aka Lambert-Beer or
	  Bouguer-Lambert Law, or whatever other combinations or variants
	  there might exist...) The color components set the coefficient
	  controlling the exponential loss of radiance along the distance
	  of the viewing ray through the object. For other exporter script
	  writers it might be convenient to export this color as the actual
	  color wanted at a certain distance. This can be done as follows,
	  the absorption color at distance D is: -log(result_color)/D.
	  With all that said, considering everything else in yafray it
	  would be a bit overoptimistic to expect extremely realistic
	  renders, it is all really much more of an approximation than a
	  totally physically based model.  Almost forgot, it works with
	  caustics too, not for indirect caustics, currently only the
	  photonlight.	Ok, done with the extreme hand waving, I now have a
	  severe muscle ache... ;)

	- (Alex Klein) Contributed a patch to optimize certain parts of 
		yafray, like the photonlight.

	- (eeshlo) Added two extra camera types, 'spherical' for spherical panorama 
		type renders, and 'lightprobe', renders in angular map format which can 
		be used as 'lightprobes' for HDRI. Both are full 360 degree views of the
	  scene.  For these two types, only the three main camera
	  parameters (from/to/up) are used.  The ideal image size ratio for
	  'spherical' is 2:1, and 1:1 for 'lightprobe' (square).  This is
	  not mandatory though, so it is not set automatically.

	- Added some extra parameters which might be useful for
	  compositing.	Still not quite what it should be though.
	  'alpha_maskbackground' when set will not render any background.
	  'alpha_premultiply' when set will multiply colors with alpha
	  before output.  Using both together can actually make things
	  worse...

	- (eeshlo) HDR images can now be used as regular image textures.

	- (eeshlo) Added support for the OpenEXR format.

	- (eeshlo) HDR 'exposure_adjust' parameter can now be a float for more
	  precise control.

	- (eeshlo) Added high dynamic range output support in render block parameter
	  'output_type'.  This can be 'tga','hdr' or 'exr' (if not
	  supported, will output to hdr instead).  (Does not automatically
	  change file extension) Exposure & gamma correction are bypassed
	  when 'hdr' or 'exr' is the output type.

	- (jandro) Support for rendering just an image region. From blender we
		use render border settings as internal renderer does.

	- (jandro) Improvement in the BSP implementation (raytracing core). This
		reduces rendertime from 10% to 40% in certain scenes.

	- Lot of bugfixes and minor improvements/speedups
