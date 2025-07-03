Simple simulation to modell a 2D car, based on a simplified bycicle modell, with the following equations: <br />

$$
\begin{align}
x_{t+1} = x_t + v \cdot cos(\theta_{t}) \cdot dt
\end{align}
$$

$$
\begin{align}
y_{t+1} = y_t + v \cdot sin(\theta_{t}) \cdot dt
\end{align}
$$

$$
\begin{align}
\theta_{t+1} = \theta_t + \frac{v}{L} \cdot tan(\delta) \cdot dt
\end{align}
$$
<br />


