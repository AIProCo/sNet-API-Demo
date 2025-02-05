### 🎉A new real-time SR model(aipro_sr_2_0) has been released!!🎉
# sNet-API-Demo (Demo program for sNet-API)

- This is a repository for demo of sNet-API developed by AIPro Inc.
  + sNet: AIPro Deep Learning Solution supporting the following features:
     - Real-Time Super Resolution
     - Night Vision

# Licensing and Restrictions

- Commercial use must be approved by AIPro Inc. 
- The maximum number of inferences for all channels is set to 10000.
- (Important!!) There are several hidden features to prevent illegal use in this repository.
  
------------------

### **Dependency**

- cuda 12.1.0 (cuda_12.1.0_531.14_windows.exe)
- cuDNN 8.9.6 (cudnn-windows-x86_64-8.9.6.50_cuda12-archive.zip)

### **Installation and Solution Guide**

- You can refer to the following technical document in the repository for installation and solution guide:
  + Korean: AIPro_sNet_solution_guide_v1.4.x(Korean).pdf

### **Download and extract files**

- Download and unzip one of the following zip files depending on your GPU. Then, copy and paste bin, inputs, and videos directories into the solution directory (the directory including the `.sln` file):
  + Cuda compute capability of your GPU should be 8.6(RTX-30xx) or 8.9(RTX-40xx): 
    - Link for RTX-30xx: https://drive.google.com/file/d/1dz8AccD8L8FMK63zZS7tglujhfEkW8nW/view?usp=sharing
    - Link for RTX-40xx: https://drive.google.com/file/d/17mW6FO3gcI8qBKRml_rhqD-j3lmyggUu/view?usp=sharing

### **Run the project**

- Open the following sln:
  + `sNet-API-Demo.sln` in the solution directory

### **Performance of AIPro Real-Time SR**
![AIPro 회사소개서_240911_v1 0_ND](https://github.com/user-attachments/assets/0994c2df-efdf-4ac7-bf5f-64d805f7f110)
### **Video Quality Demo**
- Sample #1(click)
[![SR1](https://img.youtube.com/vi/nNWG7DGQrYs/maxresdefault.jpg)](https://youtu.be/nNWG7DGQrYs)
- Sample #2(click)
[![SR2](https://img.youtube.com/vi/Q6KqRZ9lyes/maxresdefault.jpg)](https://youtu.be/Q6KqRZ9lyes)
