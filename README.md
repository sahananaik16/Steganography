### **Steganography in C – Hiding Data Within Images**  

This project implements **steganography**, the technique of concealing secret data within an image file using the **Least Significant Bit (LSB)** method. By modifying the least significant bits of pixel values, data is embedded without causing noticeable changes to the image. The project is designed to securely encode and retrieve hidden information while maintaining the image’s visual integrity.  

### **Key Functionalities**  

#### 🔹 **Encoding (Hiding Data)**  
The encoding process embeds secret data into an image file by altering the LSBs of pixel values. This is done in a structured manner to ensure that the data can be accurately retrieved later. The encoding steps include:  
1. **Magic String Insertion** – A predefined string is embedded first to indicate that the image contains hidden data.  
2. **File Size Encoding** – The size of the secret file is stored to facilitate correct extraction during decoding.  
3. **Data Embedding** – The actual secret content is encoded into the image pixels using LSB modifications.  
4. **Saving the Modified Image** – The image with embedded data is stored as a new file, appearing visually unchanged.  

#### 🔹 **Decoding (Retrieving Data)**  
The decoding process reverses the encoding steps to extract the hidden data from the modified image. It follows these steps:  
1. **Magic String Verification** – Confirms the presence of hidden data by checking for the embedded magic string.  
2. **Extracting File Size** – Reads the encoded file size to determine how much data needs to be retrieved.  
3. **Extracting Hidden Data** – Recovers the secret data bit by bit from the LSBs of pixel values.  
4. **Reconstructing the File** – The extracted data is written back to a file, restoring the original hidden content.  

### **Project Structure**  
The project uses a structured approach, with a dedicated `EncodeInfo` struct to manage file details efficiently. It ensures reliable encoding and decoding while maintaining image quality.  

### **Why This Project?**  
- ✅ **Minimal Visual Distortion** – Uses LSB modifications that don’t significantly alter the image appearance.  
- ✅ **Secure Data Concealment** – Hidden data remains undetectable without decoding techniques.  
- ✅ **Efficient & Structured** – Well-organized encoding and decoding processes for accurate data retrieval.  

This project serves as a foundation for secure data hiding and can be further enhanced for real-world applications like watermarking, digital forensics, and secure communications. 🚀
