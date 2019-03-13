Compiler from SLIC to GSTAL

SLIC is a language written by Lipscomb University professor, Dr. Bryan Crawley.
GSTAL, Georgetown Stack Assembly Language, is derived from STAL, a stack assembly language designed by Gerald Wildenberg of St. John Fisher College.

Specs for both languages are included in this folder.

To run this compiler:
 
1. navigate to the folder 'Compiler'
2. run the following commands in your command line:
	make
	./a.out < <slic file>
Slic files can be found int the SLICExamples folder
This will then output a gstal file called 'gstalFile.gstal'

To run 'gstalFile.gstal':

3. Move gstalFile.gstal to ../gstal
4. Navigate to ../gstal
5. Run the following commands in your command line:
	make
	./gstal gstalFile.gstal


 Warning: This compiler is not complete and may still contain bugs
