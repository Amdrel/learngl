#version 330 core

in vec2 fragUv;
out vec4 color;

uniform sampler2D frameTexture;
uniform float time;

// The offset may or may not be representative of an actual pixel.
const float offset = 1.0 / 600;

void main() {
  // Get the sine of time and add t to affect the sine wave depending on where
  // the fragment is in the texture. Floor the addition so some vertical
  // fragments get the same value giving off a retro feel.
  float wave = sin(time + floor(fragUv.t * 35) / 35) / 2;

  // Sample using the wave calculation added ontop of the normal coordinate.
  vec2 coord = vec2(fragUv.s + wave / 2, fragUv.t);
  vec3 sample = vec3(texture(frameTexture, coord));

  // Scanline effect, just abuses the wave.
  float scanline = clamp(wave, -0.025f, 0.0f)

  color = vec4(sample + scanline, 1.0f);
}
