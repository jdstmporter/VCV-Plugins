#Meromorph modules

A collection of simple VCV modules implementing a number of useful mathematical techniques,
especially those involving complex (holomorphic) signal processing.

##Wavelet Shaper

This module uses a very basic wavelet transform to distort a signal in unusual and unexpected ways.

###Basic Theory 

[Wavelet transforms][wavelet] analyse signals into components representing activity at various *scales*, each factor of 2 longer than the previous.   Thus, the *[Haar Wavelet][haar]* (as used in this module) analyses a signal into events that occur from one sample to the next, i.e. over a scale of 2 pixels, then over a scale of 4 pixels, 8 pixels, etc. In audio terms, that can be thought of as breaking the signal down into sharp transients, and then progressively longer changes.  A signal can be reconstructed exactly from its wavelet components.

###Algorithm

This module breaks the signal into blocks of 16 samples, and analyses each using the Haar wavelet, so the result is five *layers* of wavelet components:

- 8 *level 1 details* (change between adjacent samples)
- 4 *level 2 details* (change between adjacent pairs of samples)
- 2 *level 3 details* (change between adjacent quads of samples)
- 1 *level 4 detail* (change between adjacent octets of samples)
- 1 *approximation* (what is left over: the average of all 16 samples)

The user specifies a threshold for each of these five layers: components in each layer are removed if they exceed the threshold (so threshold = 0 means all components in that layer are set to zero).  The resulting wavelet is reconstructed to produce a distorted signal.

*(This is not everything that happens: the thresholds are adaptive, using a simple IIR filter to scale their values from the range 0 - 1 to one suitable to cover the range of actual coefficient values; details can be found in the source code by those who care)*

###Module

- Controls: thresholds set by pots in the range 0 - 1:

  - Four pots to set the threshold level for each of the 4 levels of details
  - One pot to set the threshold level for the approximation
  
- Input: Clamped to -5v < input < 5v
- Output: Clamped to -5v < input < 5v

This module uses a variation of a rather crude wavelet-based [video compression algorithm][video].  It is known for its unpredictable results, which means that it is of questionable use for compressing video, but perfectly suited for creating new sounds from old. The sounds arising from even simple inputs can be incredibly varied, so the best thing to do with the effect is to play with it and see.

[wavelet]: https://en.wikipedia.org/wiki/Wavelet_transform
[haar]: https://en.wikipedia.org/wiki/Haar_wavelet
[video]: https://en.wikipedia.org/wiki/Wavelet_transform#Wavelet_compression



