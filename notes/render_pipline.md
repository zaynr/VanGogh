# 渲染管线

| Vertex Shader                   | Rasterization                                   | Fragement Shader          |
| :------------------------------ | :------------------------------ | :------------------------------ |
| 对顶点进行 MVP 变换，投影到 NDC。部分不投影的向量则通过参数传递到 Fragement Shader 内。 | 对 NDC 内开始采样，在三角形内的像素执行 program 内的 Fragement Shader 继续着色。 | 对每个采样点进行光照计算等处理。根据 Texture、Normal 等插值信息进行着色计算。 |

## MVP 变换

Model，View，Projection。View 和 Projection 只作用于顶点，用于计算顶点在 NDC 内的位置。

Normal Vector 在经过 Model 变换后，可能会失效（非 Uniform Scale），对 Normal Vector 进行 Model 变换的矩阵为： $(\mathbf{M}^{-1})^\top$ ，具体推导可见 [Normal Matrix](http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/)。

## Rasterization

光栅化有好几种算法。[扫描线](https://www.jianshu.com/p/d9be99077c2b)，DDA，中点Bresenham，三角形等。

OpenGL 使用的应该主要是三角形，也就是逆时针连接三个顶点出来的三个向量，和三个顶点与待判断点连线的向量分别叉乘，最后得出的向量如果符号一致则为在三角形内部。采样点的选取，应该是通过 viewPort ，根据 width\*height 逆推回 NDC 空间的 x 和 y 值，然后再做 Pixel shading。

## Shading

虽然 Vertex Shader 已经把 Vertex projection 到 NDC 里，但是采样点其他的参数如 Normal、TexCoord、Depth 等参数是没有 Projection 的，继续留在 World space 内，通过重心插值得出。

每个像素的其他参数，需要基于 NDC 内的顶点做 Perspective Correct Interpolation。推导过程 [在这](chrome-extension://ikhdkkncnoglghljlkmcimlnlhkeamad/pdf-viewer/web/viewer.html?file=https%3A%2F%2Fwww.comp.nus.edu.sg%2F~lowkl%2Fpublications%2Flowk_persp_interp_techrep.pdf)。很高兴 OpenGL 已经完成了这些 Dirty work，我们直接拿来用就行（Vertex Shader Output）。

这一点纠结了我很久，就是 Fragement Shader 的光照是基于 World sapce 计算的，Vertex projection to NDC 并不影响其他的参数，并且我们 Shading 的时候其实并不需要 Vertex，他只是插值计算用而已！

在不开启 SuperSampling 的情况下，每个采样点可以理解为对应一个 pixel。每个采样点都会有一个 color buffer、depth buffer、stencial buffer，组成了一个 mat\[width\]\[height\]。

