# txt2dcm
Basic software written in C++11 about multimedia processing. Basic software written in C++11 about multimedia processing. Extract info from a file in txt format and save them in a file in Dicom format, according to specifications.

## What the program has to do

* command line program which accepts these options: programname <input file .txt> <output file .DCM>;
* manage command line arguments;
* extract info about patients and image name from a file in txt format and then save them in a new file in DICOM format according to specifications.

## file.txt specifications
* each row contains information in this way: <field name>:<tab><field value>;
* field name to consider: File (file PPM name), Surname (patient's), Name (patient's), ID, Sex (M,F), Implementation;
* field order is variable.

## Reference
* [PPM Format Specifications](http://netpbm.sourceforge.net/doc/ppm.html)
* [DICOM - Digital Imaging and Communications in Medicine international standard ] (https://www.dicomstandard.org/)
N.B. Dicom file format specification summarized in "DICOM_testo" (italian language); it is a simplified version for exercise purpose.

## Utility

* [Microsof Visual Studio](https://visualstudio.microsoft.com/) - IDE used
* [MicroDicom](http://www.microdicom.com/downloads.html) - DICOM viewer used

## Author

**Marcella Tincani** - [Marcella](https://github.com/tmarcy)

