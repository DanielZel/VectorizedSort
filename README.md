#### Outilne
This is an implementation a vectorized version of the [Bitonic Sorter](http://https://en.wikipedia.org/wiki/Bitonic_sorter "Bitonic Sorter") using ARM NEON intrinsics.  
It works on arrays with 2^N elements (although could be expanded to [work](http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/oddn.htm "work") on any N).

#### Prerequisites
ARM platform  
Boost (`sudo apt-get install libboost-dev` should be enough)


#### Results
The following table summarizes the acceleration factor, of `bitonic_sort()` compared `std::sort()`.

|  # of elements | acceleration factor  |
| ------------ | ------------ |
|32|0.828263|
|64|**1.81081**|
|128|**1.81167**|
|256|**1.70836**|
|512|**1.44313**|
|1024|**1.30703**|
|2048|**1.22619**|
|4096|1.06401|
|8192|0.869226|
|16384|0.752398|
|32768|0.668331|
|65536|0.537231|

Tested on [Orange Pi Lite2](http://www.orangepi.org/Orange%20Pi%20Lite%202/ "Orange Pi Lite2"),  ARM Cortex-A53
