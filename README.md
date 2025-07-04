## 2D Car Simulation

A simple simulation to model a 2D car using a simplified **bicycle model**, traveling at **constant speed**. The motion is described by the following equations:

$$
\begin{align}
x_{i+1} = x_i + v \cdot cos(\theta_{i}) \cdot dt,
\end{align}
$$

$$
\begin{align}
y_{i+1} = y_i + v \cdot sin(\theta_{i}) \cdot dt,
\end{align}
$$

$$
\begin{align}
\theta_{i+1} = \theta_i + \frac{v}{L} \cdot tan(\delta) \cdot dt,
\end{align}
$$

Where:

- `(x, y)`: current position of the vehicle  
- `θ`: heading angle  
- `v`: velocity  
- `L`: wheelbase  
- `δ`: steering angle
- `dt`: small time difference between (i+1) and i position.

The car can turn **left** with the `A` key and **right** with the `D` key.

---

### Lane Keeping Assists

The simulation includes two types of lane keeping assist systems to help the vehicle stay within the lane:

1. **Proximity-Based Assist**  
   Activates when the distance between the car and one of the lane-defining lines becomes smaller than a predefined value.

2. **PID-Based Lane Centering**  
   Keeps the car centered in the lane using a PID controller. If the driver attempts to steer away from the center, the system overrides the input and steers the car back to the lane center.

