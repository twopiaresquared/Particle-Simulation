#include "pyvis/PythonVisualizer.h"
#include <array>
#include <cmath>
#include <vector>
#include "writer.h"

constexpr double G = 6.67408e-11;

struct Body
{
  // fill me in!
  double x_position = 0.0;
  double y_position = 0.0;
  double x_velocity = 0.0;
  double y_velocity = 0.0;
  double x_acceleration = 0.0;
  double y_acceleration = 0.0;
  double mass = 500000000.0;
  double time_delta = 0.1;

  void force(const Body& obj) {
    
    double dx = x_position - obj.x_position;
    double dy = y_position - obj.y_position;

    double d = std::hypot(dx, dy);
    double t = std::atan2(dy, dx);
    double f = G * mass * obj.mass / (d*d);
    x_acceleration += f * std::cos(t) / mass;
    y_acceleration += f * std::sin(t) / mass;
  }

  void apply() {
    x_position += x_velocity * time_delta + 0.5 * x_acceleration * time_delta * time_delta;
    x_velocity += x_acceleration * time_delta;
    y_position += y_velocity * time_delta + 0.5 * y_acceleration * time_delta * time_delta;
    y_velocity += y_acceleration * time_delta;

    x_acceleration = 0.0;
    y_acceleration = 0.0;
  }
};

int main()
{
  
  std::vector<Body> bodies;
  Body zero;
  zero.x_position = 2.5;
  zero.y_position = 0;
  Body one;
  one.x_position = 0;
  one.y_position = 2.5;
  Body two;
  two.x_position = -2.5;
  two.y_position = 0;
  Body three;
  three.x_position = 0;
  three.y_position = -2.5;
  bodies.push_back(zero);
  bodies.push_back(one);
  bodies.push_back(two);
  bodies.push_back(three);

  std::vector<std::vector<double>> all_x_data;
  std::vector<std::vector<double>> all_y_data;

  double time = 0.0;
  while(time < 30.0) {
    for(unsigned int i = 0; i < 4; i++) {
      for(unsigned int j = 0; j < 4; j++) {
        if(&bodies[i] != &bodies[j]) {
          bodies[i].force(bodies[j]);
        }
      }
    }
    std::vector<double> x_data;
    std::vector<double> y_data;
    for(unsigned int i = 0; i < 4; i++) {
      bodies[i].apply();
      x_data.push_back(bodies[i].x_position);
      y_data.push_back(bodies[i].y_position);
    }
    all_x_data.push_back(x_data);
    all_y_data.push_back(y_data);
    time += 0.1;
  }

  write_data(all_x_data, all_y_data);
  std::vector<double> range {-10.0, 10.0};
  py::PythonVisualizer pyvis({"../.."});
  auto figure = pyvis.make_new_figure("Particles", py::kwarg("x_range", range),  py::kwarg("y_range", range));
  auto im = pyvis.plot(figure, "circle", all_x_data[0], all_y_data[0]);
  auto slider = pyvis.slider(im, "Frame", 0, all_x_data.size()-1, py::kwarg("x", all_x_data), py::kwarg("y", all_y_data))
  auto layout = pyvis.layout(figure, slider);
  pyvis.generate_html(layout, "particle.html");

}