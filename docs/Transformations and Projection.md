# Transformations and projections

## 1 Transformations

For a point $\begin{bmatrix}x & y & z & w\end{bmatrix}^T$, the following transformations will occur before being displayed in the screen:

- *Orthographic mode*:
    
    $$
    M_{world} = T \times S \times R \times M_{model}
    $$

- *Perspective mode*:

    $$
    M_{world} = T \times R \times M_{model}
    $$

## 1.1 Model bounding box $B$

On initialization, the program will compute the model bonding box, 

$$
B = [l_m, r_m] \times [b_m, t_m] \times [n_m, f_m]
$$

used in the computation of the transformation and projection matrices.

$l_m$ and $r_m$ stands for the smallest and largest $x$-coordinate value among the model vertices, respectively. The same goes for $b_m$ and $t_m$ (smallest and largest $y$-coordinate value); and $n_m$ and $f_m$ (smallest and largest $z$-coordinate value).

## 1.2 Viewport dimensions, $VP_{width}, VP_{height}$

The viewport (screen size) width and height will be referred as $VP_{width}$ and $VP_{height}$, respectively and are measured by pixels.

## 1.3 Model space transform $M_{model}$

This transformation will translate the model to the of the world coordinate system.

$$
M_{model} = \begin{bmatrix}
    1 & 0 & 0 & -\frac{r_m + l_m}{2}\\
    0 & 1 & 0 & -\frac{t_m + b_m}{2}\\
    0 & 0 & 1 & -\frac{f_m + n_m}{2}\\
    0 & 0 & 0 & 1
\end{bmatrix}
$$

## 1.4 Rotation matrix $R$

The rotation matrix is formed by multiplying 3 matrices: 

$$
R = R_x \times R_y \times R_{acc}
$$

They are all initialized as the identity matrix.

When the user presses the mouse left button and drags, the rotation along the $x$- and $y$-axis are computed based on the deviation of the current cursor position $(m_x, m_y)$ from the click start point $(m_{x0}, m_{y0})$ as follows

- *Rotation around $x$-axis*:
    
    For $\theta = 2\pi \cdot (m_y - m_{y0})/VP_{height}$

    $$
    R_x = \begin{bmatrix}
        1 & 0 & 0 & 0\\
        0 & \cos(\theta) & -\sin(\theta) & 0\\
        0 & \sin(\theta) & \cos(\theta) & 0\\
        0 & 0 & 0 & 1
    \end{bmatrix}
    $$

- *Rotation around $y$-axis*:
    
    For $\theta = 2\pi \cdot (m_x - m_{x0})/VP_{width}$

    $$
    R_y = \begin{bmatrix}
        \cos(\theta) & 0 & \sin(\theta) & 0\\
        0 & 1 & 0 & 0\\
        -\sin(\theta) & 0 & \cos(\theta) & 0\\
        0 & 0 & 0 & 1
    \end{bmatrix}
    $$

When the user releases the mouse left button, the matrix $R_{acc}$ is updated as follows

$$
R_{acc}' = R_x \times R_y \times R_{acc}
$$

The matrices $R_x$ and $R_y$ are reset to the identity matrix.

## 1.5 Translation matrix $T$

The translation $T$ moves the model along the $z$-axis.

When the user presses the mouse right button and drags along the $y$-direction, the model is translated along $z$-axis by a distance proportional to the current mouse $y$ value, $m_y$, from the click start point $y$ value, $m_{y0}$. This distance is accumulated in a variable, $y_{acc}$ so that the translation always happen in relation to the model's current position.

For $t = (m_y - m_{y0} + y_{acc})/VP_{height}$, the translation matrix $T$ is defined as

$$
T = \begin{bmatrix}
    1 & 0 & 0 & 0\\
    0 & 1 & 0 & 0\\
    0 & 0 & 1 & n + (f - n)t\\
    0 & 0 & 0 & 1
\end{bmatrix}
$$

$n$ and $f$ stands for the viewing fustrum's near and far values, respectively. How those values are defined is presented in the section [2. Projections](#2.%20Projections).

When the user releases the mouse right button, $y_{acc}$ is updated as follows

$$
y_{acc}' = (m_y - m_{y0}) + y_{acc}
$$

## 1.6 Scale matrix $S$

In orthographic projection mode, the model is scaled based on the distance from $n$ to give a perception of depth:

$$
S = \begin{bmatrix}
    \frac{n}{n + (f - n)t} & 0 & 0 & 0\\
    0 & \frac{n}{n + (f - n)t} & 0 & 0\\
    0 & 0 & \frac{n}{n + (f - n)t} & 0\\
    0 & 0 & 0 & 1\\
\end{bmatrix}
$$

where $t$ is defined as in the previous section.

## 2. Projections

There are two projection modes: orthographic and perspective.

Before each projection is applied the model is centralized in the viewport through the $M_{center}$ transformation:

- *orthographic projection*:

$$
M_{vp} = M_{ortho} \times M_{center}
$$

- *perspective projection*:

$$
M_{vp} = M_{ortho} \times M_{center} \times P
$$

## 2.1 $M_{center}$ tranformation

The $M_{center}$ simply centers the model in the viewport volume along $x$- and $y$-axis.

$$
M_{center} = \begin{bmatrix}
    1 & 0 & 0 & \frac{l + r}{2}\\
    0 & 1 & 0 & \frac{b + t}{2}\\
    0 & 0 & 1 & 0\\
    0 & 0 & 0 & 1
\end{bmatrix}
$$

## 2.2 Orthographic projection $M_{ortho}$

The orthographic projection maps every point on the volume $[l, r] \times [b, t] \times [-f, -n]$ to the Open GL clip volume $[-1, 1]^2 \times [0, 1]$

$$
M_{ortho} = \begin{bmatrix}
\frac{2}{r - l} & 0 & 0 & 0\\
0 & \frac{2}{t - b} & 0 & 0\\
0 & 0 & \frac{1}{n - f} & 0.5\\
0 & 0 & 0 & 1
\end{bmatrix}

\times

\begin{bmatrix}
1 & 0 & 0 & -\frac{l + r}{2}\\
0 & 1 & 0 & -\frac{b + t}{2}\\
0 & 0 & 1 & \frac{f + n}{2}\\
0 & 0 & 0 & 1
\end{bmatrix}
$$

## 2.3 Perspective transform $P$

The perspective projection appends a transform $P$ to the right of the $M_{center}$ matrix:

$$
P = \begin{bmatrix}
    n & 0 & 0 & 0\\
    0 & n & 0 & 0\\
    0 & 0 & (n+f) & nf\\
    0 & 0 & -1 & 0
\end{bmatrix}
$$

The transform $P$ sets the $w$-coordinate as the negative of $z$-coordinate and sets the $z$-coordinate as an interpolated value in $[-f^2, -n^2]$ which, after the division by $w$, will be mapped to $[n, f]$

The perspective projection is defined as

$$
M_{persp} = M_{ortho}' \times P
$$

The matrix $M_{ortho}$ has the $n$ and $f$ values negated.