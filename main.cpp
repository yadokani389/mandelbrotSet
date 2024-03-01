#include <Siv3D.hpp>
#include <Siv3D/Keyboard.hpp>
#include <Siv3D/Scene.hpp>
#include <cmath>
#include <complex>

int check(std::complex<double> c, std::complex<double> z) {
  for (auto i : step(1, 30)) {
    z *= z;
    z += c;
    if (2 < std::abs(z))
      return i;
  }

  return 0;
}

Image makeMandelbrotImage(void) {
  Image image{1000, 1000};
  std::complex<double> c, z;
  int n;
  for (auto [x, y] : step({1000, 1000})) {
    c = {x / 500. - 1.5, y / 500. - 1};
    z = {0, 0};
    n = check(c, z);
    image[y][x] = (n ? HSV{150 + n} : HSV{0, 0, 0});
  }

  return image;
}

void renderJuliaImage(Image &image, std::complex<double> p) {
  std::complex<double> c, z;
  int n;
  for (auto [x, y] : step({1000, 1000})) {
    c = p;
    z = {x / 250. - 2, y / 250. - 2};
    n = check(c, z);
    image[y][x] = n ? HSV{150 + n} : HSV{0, 0, 0};
  }
}

void Main() {
  Window::Resize(2000, 1000);

  std::complex<double> p{0, 0}, goal{0, 0};
  bool mode = false;

  const Texture mandelbrotTexture{makeMandelbrotImage()};
  Image juliaImage(1000, 1000);
  DynamicTexture juliaTexture(juliaImage);

  renderJuliaImage(juliaImage, p);
  juliaTexture.fill(juliaImage);

  while (System::Update()) {
    mandelbrotTexture.draw(0, 0);
    juliaTexture.draw(1000, 0);

    if (Cursor::PreviousPos() != Cursor::Pos() && InRange(Cursor::Pos().x, 0, 999) && InRange(Cursor::Pos().y, 0, 999)) {
      p = {Cursor::PosF().x / 500 - 1.5, Cursor::PosF().y / 500 - 1};

      renderJuliaImage(juliaImage, p);
      juliaTexture.fill(juliaImage);
    }

    if (mode) {
      while (std::abs(goal - p) < 0.1)
        goal = {Random(-1.5, 0.5), Random(-1., 1.)};

      auto arg = std::arg(goal - p);
      p.real(p.real() + std::cos(arg) * Scene::DeltaTime() / 10);
      p.imag(p.imag() + std::sin(arg) * Scene::DeltaTime() / 10);

      renderJuliaImage(juliaImage, p);
      juliaTexture.fill(juliaImage);
    }

    Circle((p.real() + 1.5) * 500, (p.imag() + 1) * 500, 5).draw(Palette::White);

    if (KeySpace.down())
      mode = !mode;
  }
}
