Simple simulation to modell a 2D car, based on a simplified bycicle modell, with the following equations:

$$
\begin{align}
x_{t+1} = x_t + v \cdot cos(\theta_{t}) \cdot dt,
\end{align}
$$

$$
\begin{align}
y_{t+1} = y_t + v \cdot sin(\theta_{t}) \cdot dt,
\end{align}
$$

$$
\begin{align}
\theta_{t+1} = \theta_t + \frac{v}{L} \cdot tan(\delta) \cdot dt,
\end{align}
$$


where (x,y) is the current position of the vehicle, $\theta$  is the heading angle, v is the velocity, L is the wheelbase and $\delta$ is the steering angle. <br />

The car has the ability to turn left (with button a) and right (with button d). Moreover, the simulation contains two types of assists that helps the car stay in between the lanes. These two lane keeping assists are the following:

- The first one activates in case the distance between the car and the line of the lane is less than a set value.
- The second one makes the car stay on the center of the lane and if the driver wants to move off the lane (turns the steering wheel), the system cuts off the steering and steers the car to the center of the lane.


