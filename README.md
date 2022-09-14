# sNet-API-Demo (Demo program for sNet-API)

- This is a repository for demo of sNet-API developped by AIPro Inc.
- In this demo, the number of inferences is limited to 1000.
- Commercial use must be approved by AIPro Inc.
- You can refer to one of the technical documents in the repository for more information:
  + English: AIPro_sNet_solution_guide_v1(English).pdf
  + Korean: AIPro_sNet_solution_guide_v1(Korean).pdf
  
------------------

### **Dependency**

- Cuda 11.6.2
- cuDNN 8.4.0 (cudnn-windows-x86_64-8.4.0.27_cuda11.6)
- zlibwapi.dll (cuDNN 8.4.0 uses this)
  - Download "zlib123dllx64.zip", unzip, and copy "zlibwapi.dll" in dll_x4 to "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\bin"

   ![2](https://user-images.githubusercontent.com/35882715/168030331-0e727e1a-ee6a-4fe7-94d9-065334233622.jpg)
    (https://docs.nvidia.com/deeplearning/cudnn/install-guide/index.html#install-zlib-windows)

### **Environments(PATH)**

- C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\bin
- C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.6\libnvvp

     ![1](https://user-images.githubusercontent.com/35882715/168030273-6135cc5a-8a1d-4bda-baaf-6f11b7d8e5a7.jpg)


### **Config VS project properties (if needed)**

| Left panel | Right panel                    | Add value                                                                                                  |
| ---------- | ------------------------------ | ---------------------------------------------------------------------------------------------------------- |
| C/C++      | Additional Include Directories | include_opencv; include                                                          |
| Linker     | Addtional Library Directorires | bin
| Linker     | Addtional Dependencies         | generator_multi_v2.lib; opencv_world455.lib
| Debugging  | Environment                    | PATH=bin;%PATH%

### **Download and extract files**
- Download and upzip the followings zip file. Then, copy and paste bin, inputs, and videos directories to the solution directory (the directory including the `.sln` file):
  + https://drive.google.com/file/d/12UzUP7BK6VicL3dnaAXt5fd4LNh9-W-5/view?usp=sharing

### **Run the project**

- Open the following sln:
  + `sNet-API-Demo.sln` in the solution directory
