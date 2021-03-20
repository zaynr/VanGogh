# 相机系统

## 坐标系
OpenGL 的 world、view space 是右手系，在 projection 到 NDC 后则变成了左手系。

world space 坐标为：x 向右，y 向上，z 向外。

![world space](https://learnopengl.com/img/getting-started/coordinate_systems_right_handed.png)

## 变换

一般做变换，物体 model matrix 先 scale，再 rotate，最后 translate。
glm 提供了对应的工具函数，生成的 matrix 可以直接 upload 到 shader 里。

view space 变换在数学意义上，相当于把相机做变换到坐标系原点，且将相机的 up 对齐 y axis，right 对齐 x axis，front 对齐 negative z axis。得出的 view matrix 作用到每一个物体上（只涉及 rotate、translate），使每个物体都落到 view space 内。view matrix 则是使用 glm 的 lookAt 函数生成，理论上使用 rotate + translate 也可以达到同样效果。

最后的 projection 如果是 perspective 则是 **非线性** 的。并且由于和 z 相关，near plane 的精度远高于 far plane。所以 GLES 将 NDC \ [near, far\] 的映射从  \[0, 1\] 变成了 \[1, 0\]。因为 IEEE 754，浮点数的精度在 0 附近远高于 1 附近。
这个非线性变换会导致线性插值失效，所以需要一个逆变换作用在重心坐标插值上。
并且法向量的 projection 也会受到影响！需要做一个 normal matrix。

## Euler Angle
- 可以通过 Euler Angle 来描述相机的朝向。以上文 world space 坐标系为参考。
- pitch: 为沿着 x axis 上下转动的俯仰角。
- roll: 为沿着 z axis 旋转的旋转角。
- yaw: y axis

## 多相机
在 shadow mapping 也需要将一个 coordinate 变换到另一个 camera 的 NDC 里。depth buffer 是存在 view port 出来后的 mat\[screenWidth\]\[screenHeight\] 里的。