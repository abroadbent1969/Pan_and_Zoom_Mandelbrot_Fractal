This C++ program uses SFML to render and explore the Mandelbrot fractal. It provides real-time zooming and panning functionalities, along with the ability to record frames of a zoom animation for later use. The program includes a smooth coloring algorithm and time-based color variation to enhance the fractal's visual appeal.

Features
Real-time Fractal Rendering: Uses SFML to visualize the Mandelbrot set.
Zoom and Pan: Users can navigate the fractal using keyboard and mouse controls.
Smooth Coloring: Implements a smooth color gradient for better visualization.
Frame Recording: Captures frames of a zoom sequence and saves them as PNG files.
On-Screen Information: Displays the current zoom level and center coordinates.

Keyboard Controls:
Arrow Keys: Move the view
Mouse Wheel: Zoom in/out
T: Toggle on-screen information
R: Start frame recording

Dependencies
SFML (Simple and Fast Multimedia Library)
C++17 or later (for modern features like std::filesystem)

Installation and Compilation
Ensure SFML is installed and linked correctly in your development environment.

Recording Frames
Press R to start recording frames, which will be saved in the recorded_frames/ directory.

Code Structure
drawFractal(): Renders the Mandelbrot set.
recordFrames(): Generates a sequence of images for a zoom animation.
main(): Handles event processing, rendering, and user interaction.

Future Enhancements
Implement GPU acceleration for faster rendering.
Support exporting frames as a video.
Add interactive UI controls for adjusting parameters dynamically.

License
This project is open-source and freely available for modification and distribution.
