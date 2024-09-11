# Graphics Portfolio
<p>Created for CS5310: Computer Graphics</p>
<p>All images were created with C code I wrote from scratch.</p>

### 1. What is an Image?
<p>Lab 1 aimed to set up Unix, C and have an initial understanding of the mechanics of creating, manipulating, and writing images.</p>
<p>The red color was picked and amplified from the source image on the left. The rest of the colors were turned into grey. The second image shows the effect.</p>
<p><img src = "/../main/images/geraniums.png" width = 300 />
   <img src = "/../main/images/out.png" width = 300 /></p>
<p>I merged the source image powerpuff with the background image. </p>
<p><img src = "/../main/images/powerpuff.png" width = 300 />
   <img src = "/../main/images/composite.png" width = 300 /></p>

### 2. Fractals
<p>Lab 2 aimed to build and use useful data structures -- an Image and a Pixel. The main tasks involved creating an image data structure to represent images, implementing algorithms to generate Mandelbrot and Julia sets, and creating fractal noise functions such as Perlin noise.</p>
<p>I initialized an image structure and filled it with color values based on the row and column indices. I created a gradient effect where the red and green channels vary with the distance from the center, while the blue channel changes with the sum of row and column indices.</p>
<img src = "/../main/images/colors.png" width = 500 />
<p>Julia function created a visualization of the Julia set for the constant c = 0.7454054 + 0.1130063i. The grid of points in the complex plane was centered around (-1.8, -1.8 * 0.75) with a span of 3.6 units. The julia_escape function determined the iteration count for each point, indicating how quickly the sequence diverges or remains bounded. This count was then converted to a grayscale color, resulting in an image that displays the fractal structure of the Julia set. The Julia set demonstrates the complex, self-similar structure characteristic of fractals.</p>
<em>Julia Set</em>
<img src = "/../main/images/juliaset.png" width = 500 />
<p>The following two images are a zoomed-in view and a full view of the Mandelbrot set. The function mandelbrot was used to iterate over a grid of points in the complex plane, centered around (-1.5, -1.5) with a span of 4 units. For each point, the mandelbrot_escape function determined how many iterations it took for the sequence to exceed the escape radius. The iteration count was then mapped to a grayscale color value, where a higher iteration count (indicating points inside the Mandelbrot set) resulted in darker colors. These images captured the characteristic boundary of the Mandelbrot set with intricate fractal details.</p>
<em>Mandelbrot Set</em>
<img src = "/../main/images/mandelbrot.png" width = 500 />
<em>Mandelbrot Full Set</em>
<img src = "/../main/images/mandelbrot-full.png" width = 500 />
<p>To create perlin noise, I implemented a Perlin noise generator that filled an image with procedural noise. The Perlin noise function was initialized with a shuffled permutation array to ensure randomness. Each pixel in the image was assigned a noise value based on its coordinates, which was then normalized to a range of 0 to 1 and mapped to grayscale colors. This process produced a smooth, cloud-like texture. Perlin noise can be used for various realistic texture effects.</p>
<em>Perlin Noise</em>
<img src = "/../main/images/perlin_noise.png" />
   
### 3. Graphics Primitives
<p>Lab 3 aimed to build a graphics system. The main tasks involved implementing graphics primitives, including lines, circles, ellipses, and polylines using Bresenham’s algorithms.</p>
<img src = "/../main/images/test3d.png" />
<p>Rectangles, a cirle and lines.</p>
<img src = "/../main/images/test3a.png" width = 500 />
<p>The image has five random square objects. Each square is initially defined by four lines forming its edges. These lines are stored in a linked list and then repeatedly subdivided into smaller segments, perturbing their midpoints to add randomness. The number of subdivisions and the perturbation factor are specified by user input, with an optional random seed for reproducibility. Each square is drawn in a random color.</p>
<img src = "/../main/images/test3b.png" height = 500 />
<p>The sphere was created by calculating the distance of each pixel from the center of the circle and adjusts the color intensity accordingly, creating a shading effect that gives the appearance of a three-dimensional sphere.</p>
<img src = "/../main/images/3dball.png" width = 500 />
<p>The dashline was created by alternating between drawing and not drawing segments of the specified length (20 pixels).</p>
<img src = "/../main/images/dashline.png" width = 500 />

### 4. Scanline Fill
<p>Lab 4 aimed to implement a polygon API and the scanline fill algorithm for polygons. The main tasks involved implementing a subset of the polygon API functions, the scanline fill algorithm for polygons and the barycentric fill algorithm.</p>
<p>This image was composed of a square and three irregular polygons. Each polygon was constructed with random vertices. All the shapes were filled using a Scanline fill algorithm. </p>
<p><img src = "/../main/images/test4a.png" />
   <img src = "/../main/images/scanline_fill.gif" /></p>
<p>This image was filled with a barycentric fill algorithm.</p>
<img src = "/../main/images/test4b.png" />
<p>I drew a polygon using a scanline fill algorithm with a gradient color effect. This gradient effect is achieved by linearly interpolating the RGB values of the colors along each scanline, resulting in a smooth transition from the start color to the end color across the polygon.</p>
<img src = "/../main/images/polygon_gradient.png" width = 500 height = 400/>

### 5. Transformation and Viewing
<p>Lab 5 aimed to implement a library of 2-D and 3-D transformations and then make use of them to build an object and a simple imaging pipeline. The main tasks included creating matrix, 2D viewing and perspective viewing structures and functions</p>
<p>The geometric shapes representing a spaceship were transformed into world coordinates using a series of translation, scaling, and rotation matrices. The shapes were then combined into a spaceship and transformed into the screen's view coordinates. The transformed shapes were then rendered onto the image canvas, filled with colors.</p>
<img src = "/../main/images/test5a.png" width = 500 />
<p>50 random lines with specific orientations and frequencies were generated within a square area. Each line was rotated around its endpoint based on its orientation and frequency. The transformed lines were then drawn onto an image matrix. The view matrix was updated to translate across the screen, creating a panning effect.</p>
<img src = "/../main/images/lines.gif" />
<p>The 3D cube was projected onto a 2D image plane. It initialized a 3D view with a specified viewpoint and sets up a virtual camera using the View3D structure. These polygons were transformed using several View Transformation Matrices (vtm) to project them onto the 2D image plane. </p>
<img src = "/../main/images/cube.gif" />

### 6. Hierarchical Modeling
<p>Lab 6 aimed to implement a hierarchical modeling system and build scene graphs using primitives and matrix transforms. The main tasks included creating module structures and drawstate, traversing scene graphs to draw complex scenes.</p>
<p>This 2D scene is composed of 3 formation modules. Each formation has an X-wing module. X-wings are composed of wing, engine and body modules. Random points were generated to represent stars in the background.</p>
<img src = "/../main/images/wings.png" />
<em>Another 2D scene of Penrose triangles</em>
<img src = "/../main/images/penrose.png" />
<p>This 3D scene is composed of 3 body modules. Each body has a wings module. The wings module is composed of 4 wing modules. The wing has engine and laser modules. The structures were built using polygons and cylinders.</p>
<img src = "/../main/images/xwings.png" />
   
### 7. Bezier Curves and Surfaces
<p>Lab 7 aimed to implement Bezier curves and surfaces as part of the hierarchical modeling system. The main tasks included creating Bezier curve and surface structures, adding the structures to the hierarchical modeling system.</p>
<p>I set up 2 Bezier curves. Each of them had four control points. Three curves of each type were added into the scene graph module.</p>
<p><img src = "/../main/images/division0.gif" width = 300 />
   <img src = "/../main/images/division2.gif" width = 300 />
   <img src = "/../main/images/division4.gif" width = 300 /></p>
<p>I drew one flat plane and one curved surface using Bezier surfaces. Each surface was added to a scene graph module and drawn.</p>
<p><img src = "/../main/images/surfdiv0.gif" width = 300 />
   <img src = "/../main/images/bezSurf-1.gif" width = 300 />
   <img src = "/../main/images/bezSurf-3.gif" width = 300 /></p>

### 8. Z-Buffer Rendering
<p>Lab 8 aimed to implement z-buffer rendering to do hidden surface removal. The main tasks included implementing z-buffer fields of pixel, line and polygon structures, implementing drawing lines using z-buffer and implementing polygon_drawShade.</p>
<img src = "/../main/images/test8a.png" width = 500 />
<em>This image demonstrates overlaping. Only the visible surfaces were drawn. </em>
<img src = "/../main/images/test8b.png" width = 500 />
<p>I created multiple sets of color-shaded cubes. A single cube module was used to build three differently colored and scaled cubes, which were then combined into a set of cubes. This set was placed into a scene, and multiple instances of this scene were rotated and translated to random positions.</p> 
<img src = "/../main/images/cubism.gif" />
   
### 9. Lighting and Shading
<p>Lab 9 aimed to implement Gouraud shading for polygons with ambient and point light sources. The main tasks include implementing 3D z-buffer rendering with lighting and shading. </p>
<p>The ambient and point light were added to the lighting model that influenced the shading of the sphere. The Lighting_shading function caculates the proper color given the parameters such as surface normal and view vector.</p>
<img src = "/../main/images/test9d.png" width = 300 />
<em>Lighting and Gouraud shading on more complex shapes, a cylinder and a cone</em>
<img src = "/../main/images/test10d.png" />
<p>This 3D scene was drawn in Gouraud shading with a point light source.</p>
<img src = "/../main/images/test9b.png" />
<p>The starfuries were created by generating a high-resolution PPM image of a 3D scene rendered from a ply model. It set up 3D view and transformation matrices, and applied various lighting effects to illuminate the model. The high-resolution image was resized to 25% using the convert command, and saved as a PNG file.</p>
<img src = "/../main/images/starfuries.png" />
