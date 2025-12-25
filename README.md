# Classical

This is the code for the Classical Implementation of the protocol and procedure in "Witness-Succinct UC-Secure SNARKs with Weaker Trust Assumptions" of which I am an author. This variant uses the KZG commitment System. 

**This document will provide only a high-level overview of the build instructions for this code. For a more in-depth resource on the protocol construction please reference the original paper [here](www.google.com)**

***This code may contain bugs that are not known due to not being professionally audited. Thus I advise that this code not be used as is in a production environment.***

## Dependencies

I will only provide details on how to install these dependencies. If you as a reviewer or researcher have installed previous versions of these dependencies on your device please remove them (by referencing documentation) and then run a fresh install by following the provided instructions.
- MCL
  ```
  cd ~
  git clone https://github.com/herumi/mcl.git
  cd mcl
  mkdir build
  cd build
  cmake .. \ -DMCL_BUILD_TEST=OFF \ -DMCL_BUILD_SAMPLES=OFF
  make -j$(nproc)
  sudo make install
  sudo ldconfig
  ```
- TK
- TK
