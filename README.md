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
<p>These are a zoomed-in view and a full view of the Mandelbrot set. The function mandelbrot was used to iterate over a grid of points in the complex plane, centered around (-1.5, -1.5) with a span of 4 units. For each point, the mandelbrot_escape function determined how many iterations it took for the sequence to exceed the escape radius. The iteration count was then mapped to a grayscale color value, where a higher iteration count (indicating points inside the Mandelbrot set) resulted in darker colors. These images captured the characteristic boundary of the Mandelbrot set with intricate fractal details.</p>
<em>Mandelbrot Set</em>
<img src = "/../main/images/mandelbrot.png" width = 500 />
<em>Mandelbrot Full Set</em>
<img src = "/../main/images/mandelbrot-full.png" width = 500 />
<p>To create perlin noise, I implemented a Perlin noise generator that filled an image with procedural noise. The Perlin noise function was initialized with a shuffled permutation array to ensure randomness. Each pixel in the image was assigned a noise value based on its coordinates, which was then normalized to a range of 0 to 1 and mapped to grayscale colors. This process produced a smooth, cloud-like texture. Perlin noise can be used for various realistic texture effects.</p>
<em>Perlin Noise</em>
<img src = "/../main/images/perlin_noise.png" width = 500 />
   
### 4. Graphics Primitives
5. Scanline Fill
6. Transformation and Viewing
7. Hierarchical Modeling
   
   ![wings](/../main/images/wings.png)

   ![xwings](/../main/images/xwings.png)
   
9. Bezier Curves and Surfaces
10. Z-Buffer Rendering
   
   ![cubism](/../main/images/cubism.gif)

   ![cuboid](/../main/images/test8a.png)
   
11. Lighting and Shading

    ![3D ball](/../main/images/test9d.png)

    ![cuboid](/../main/images/test9a.png)
   
   ![X wings](/../main/images/test9b.png)
   
   ![starfuries](/../main/images/starfuries.png)
