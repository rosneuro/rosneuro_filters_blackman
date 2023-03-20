# ROSNeuro Blackman window

This ROSNeuro filter plugin applies an Blackman window to the provided data.

## Algorithm:
The filter applies the Blackman window to the data. The Blackman window is implemented as follows:
```cpp
	int nsamples;
	for(auto i = 0; i<nsamples; i++)
		window.at(i) = (42.0 - 50.0*cos((2.0 * M_PI * i)/(nsamples - 1)) + 8.0*cos((4.0 * M_PI * i)/(nsamples - 1))) / 100.0;
```
The provided window filters do not required mandatory parameters but the name and the type.
An example of the YAML configuration:
```
BlackmanCfgTest:
  name: blackman
  type: BlackmanWindowFloat
```
