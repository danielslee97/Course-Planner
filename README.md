# Course-Planner

This program reads in a requirement file, a course offering file, and a sample schedule and it will tell you whether you are set to graduate or if there are mistakes or errors within your schedule (e.g. missing prerequirements, course not offered in that semester, etc.)

FORMAT FOR REQUIREMENT FILES: 
CREDITS X Y --  
CREDITS states that you need certain Y amount of X credits
SAMPLE: CREDITS C 20 (This means that you need 20 C credits to graduate)
                         
COURSE X Y Z --    
COURSE states that X is a course that you can take, Y is a tag (e.g. M for Mandatory, R for Required, etc.) and Z                          are the course prerequirements 
SAMPLE: COURSE CS140 R (Required) CS101 CS110 (CS101 and CS110 are prereqs)
                         
CHOOSE X Y --     
CHOOSE states that you need to choose X from Y choices
SAMPLE: CHOOSE 1 BI118 CH107 PH113 (You need one of these choices to graduate)
                         
FORMAT FOR COURSE OFFERING FILE: 
COURSE_NAME  #CREDITS SEMESTERS_AVAILABLE X_CREDITS --        
SAMPLE: CS140 4 F (Fall) FHC (CS140 satisfies F, H, and C credit                                                                           requirement. This can be omitted)

FORMAT FOR SAMPLE SCHEDULE FILE: 
SEMESTER_AND_YEAR COURSES --        
The semester and years can be in random order and you cannot take a course and its prereqs at the                                           same time
SAMPLE: 
F2015 (Fall of 2015) CS101 CS110 ...
S2016 (Spring of 2016) MA324 EN113 ...

COURSE_NAME has two uppercase letters followed by three numbers (e.g. MA324, CS123, PH114)
