# sNet-API-Demo (Demo program for sNet-API)

- This is a repository for demo of sNet-API developped by AIPro Inc.
  + sNet: AIPro Deep Learning Solution supporting Real-Time Super Resolution (SR)
- In this demo, the number of inferences is limited to 300.
- Commercial use must be approved by AIPro Inc.
  
------------------

### **Dependency**

- Cuda 11.6.2
- cuDNN 8.4.0 (cudnn-windows-x86_64-8.4.0.27_cuda11.6)
- zlibwapi.dll (cuDNN 8.4.0 uses this)

### **Installation and Solution Guide**

- You can refer to one of the following technical documents in the repository for installation and guide:
  + English: AIPro_sNet_solution_guide_v1(English).pdf
  + Korean: AIPro_sNet_solution_guide_v1(Korean).pdf

### **Download and extract files**

- Download and upzip one of the following zip files. Then, copy and paste bin, inputs, and videos directories to the solution directory (the directory including the `.sln` file):
  + Cuda compute capability 7.5(including RTX 20xx): 
    - https://drive.google.com/file/d/18SVAnWV7lKDClyqar6KW38tDl6E7LYNE/view?usp=share_link
  + Cuda compute capability 8.6(including RTX 30xx): 
    - https://drive.google.com/file/d/15GG7QJqT4ISx_plmRoOvWI1HH5fPYuVb/view?usp=share_link

### **Run the project**

- Open the following sln:
  + `sNet-API-Demo.sln` in the solution directory
