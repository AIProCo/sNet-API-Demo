### 🎉A new real-time SR model(aipro_sr_2_0) has been released!!🎉
# sNet-API-Demo (Demo program for sNet-API)

- This is a repository for demo of sNet-API developped by AIPro Inc.
  + sNet: AIPro Deep Learning Solution supporting the following functionalities:
     - Real-time Super Resolution
     - Night Vision

# Licensing and Restrictions

- Commercial use must be approved by AIPro Inc. 
- The maximum number of inferences for all channels is set to 10000.
- (Important!!) there are several hidden features to prevent illegal use in this repository.
  
------------------

### **Dependency**

- cuda 12.1.0 (cuda_12.1.0_531.14_windows.exe)
- cuDNN 8.9.6 (cudnn-windows-x86_64-8.9.6.50_cuda12-archive.zip)

### **Installation and Solution Guide**

- You can refer to one of the following technical documents in the repository for installation and guide:
  + Korean: AIPro_sNet_solution_guide_v1.4.x(Korean).pdf

### **Download and extract files**

- Download and upzip the following zip file. Then, copy and paste bin, inputs, and videos directories to the solution directory (the directory including the `.sln` file):
  + Cuda compute capability of your GPU should be 8.6(RTX-30xx) or 8.9(RTX-40xx): 
    - Link for RTX-30xx: https://drive.google.com/file/d/1ngPxdCMFo73Fl49txwTUWkrB_4pN8NUv/view?usp=sharing
    - Link for RTX-40xx: https://drive.google.com/file/d/1zo1yCN8hXWSh2nWMNS_S_YVTk_Nc_Ymk/view?usp=sharing

### **Run the project**

- Open the following sln:
  + `sNet-API-Demo.sln` in the solution directory

### **Image quality demo**
![2AIPro_회사소개서V6(홈페이지전용)230523_12](https://github.com/CheaeunLee/Test/assets/127072960/ea214d4b-323d-4290-8c8a-c1d3dba9509d)

### **Video quality demo**
- Sample #1(click)
[![SR1](https://img.youtube.com/vi/nNWG7DGQrYs/maxresdefault.jpg)](https://youtu.be/nNWG7DGQrYs)
- Sample #2(click)
[![SR2](https://img.youtube.com/vi/Q6KqRZ9lyes/maxresdefault.jpg)](https://youtu.be/Q6KqRZ9lyes)
