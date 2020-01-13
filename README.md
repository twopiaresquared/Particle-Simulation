# this was my old cpp-assignment-particle-simulation

For this assignment we are going to create a particle simulation.




## Task

Programming this particle simulation is not necessarily complex, but there are a few pieces needed. What we need to do first is implement a `Body` class. This class needs the following:

* member variables:
    * x_position defaulted to 0.0
    * y_position defaulted to 0.0
    * x_velocity defaulted to 0.0
    * y_velocity defaulted to 0.0
    * x_acceleration defaulted to 0.0
    * y_acceleration defaulted to 0.0
    * mass defaulted to 500000000.0
    * time_delta 0.1

You may optionally use `std::array<double,2>` for the position, velocity, and acceleration. 


Our class needs two methods, one for computing and aggregating forces from every other `Body` and another for applying the aggregate force to the `Body`. We specifically need to collect all of the force and then apply them so that we avoid updating one body with the future state of another.

### Force Function

The first function we can call `force`; it has a return type of `void` and has a single input of the type `const Body&`. Computing the force between a one `Body` and another requires us to find the distance and angle between the two points.

* The distance can be computed using Pythagorean's Theorem.
* The angle can be comuted using `std::atan2`

Calculating both requires us to find the deltas in x and y position, and so to start we can create new variables `dx` and `dy` and set them equal to the differences in x and y positions. 
With the distance computed we can then compute the force of gravity between the two bodies using the formula:

```
F = G * m1 * m2 / (d^2)
```

Where:

* `F` is the force
* `G` is the gravitational constant
* `m1` is the mass of body 1
* `m2` is the mass of body 2
* `d` is the distance between body 1 and body 2

Once we have the force we need to split it into x and y accelerations to be applied to the body. As we have a magnitude and an angle, we can compute the directional force using trigonometry; and lastly we divide it by mass to compute an acceleration (Newton's 2nd Law).

For every particle combination we call this function and collect all of the forces, more on this later.

### Apply Functions

The second function we need in the `Body` class is the `apply` method. This method also has a return type of `void` and has no arguments. In this function we apply the aggregate acceleration with our kinematic equations to update the position and velocity of the `Body`.

```
x = x0 + v0 * dt + 0.5 * a0 * dt * dt
```

Here:

* `x` is the next position
* `x0` is the current position
* `v0` is the current velocity
* `a0` is the current acceleration
* `dt` is change in time

```
v = v0 + a0 * dt
```

Here:

* `v` is the next velocity
* `v0` is the current velocity
* `a0` is the current acceleration
* `dt` is change in time

These are the same equations we used for the cannonball simulation! The only difference here is that when we are done we must *reset* the acceleations by setting them to 0.0.

### Main

Our main function is not too complicated. The first thing we need to do is create a `std::vector<Body>` object and push into it 4 bodies. You will need to set the position of the bodies so that we have bodies at the points (2.5, 0), (0, 2.5), (-2.5, 0), (0, -2.5). In this arrangement they should form a diamond. They should be added to the vector explicitly in that order as the test will expect them to be in that order.

In order to visualize everything over time, we will need to capture the x and y data from each particle for each time step in the simulation. To store this data we need a vector of vectors for both the x and y position data.

```c++
std::vector<std::vector<double>> all_x_data;
std::vector<std::vector<double>> all_y_data;
```

We will use these vectors in a moment.

Now we want to start our primary loops. We want to limit our simulation to 30 seconds and we want to update the system at 10Hz, or 10 times a second. We create a new variable named `time` set equal to 0.0, and we start a `while loop` that will run while `time < 30.0`.

While our simulation is running we need to have every `Body` interact with each other `Body`. As we have all of our bodies in a vector, we can accomplish this using two for-loops. We need something like the following:

```
for every body b1 in bodies
    for every body b2 in bodies
        if b1 is not b2
            b1.force(b2)
```

By looping over our vector twice like this we can pair each body with each other body. We just need to be careful and make sure that we are not having a body assert a force onto itself, as this would be bad (the distance between a body and itself is 0, and we cannot divide by 0)!.

We can make sure that `b1` is not `b2` by checking their memory addresses; if their memory addresses are different, then they must be different objects. It is statistically improbable for us to have two points sharing the exact same location at any given time step, and so while we may observe some erratic behavior as points are arbitrarly close to one another, we do not need to worry about much else.

Once our double for-loop is complete we need to create two new vectors, `x_data` and `y_data`. These vectors represent the particle data at the current time step. We now need to loop over the bodies again and call the apply method on each of them. This will update the bodies in the system and reset the accelerations. As soon as we are done calling the `apply` function, we need to push back the position data into `x_data` and `y_data`. Finally we then push back `x_data` and `y_data` into `all_x_data` and `all_y_data` respectively.

And lastly once our last for-loop ends we increment the `time` variable by 0.1 seconds.

## Running & Visualizing

You must run the integrator yourself (outside of testing) to produce the file for the next step. Run the following:

```
make
./main.exe
```

When you run the simulation it will produce a new file named `particle.html` and a new data file. To visualize the plot we need to be able to open the file in your web browser. Run the following command in the terminal:

```
make move-plot
```

This command will create a special directory for displaying the graph, opens permissions for reading that directory and the plot, and puts the plot in that directory. Now in your web browser go to the following link:

https://clam.rutgers.edu/~NETID/particle.html

You will need to place "NETID" with your Rutgers NetID. If it all worked out you should see the plot, and you can hit the play button to view the particles interacting with one another. You can see a sample [here](http://crab.rutgers.edu/~nesan/particle.html).

Try running this a few times, changing the inputs and comparing the output, but be sure to restore the code to the correct values before submitting.

## Testing

You can use the included Makefile to test your code. Simply run:

```
make test
```

If you are having issues and need help, submit your code anyway as described in the next section and send an email.

