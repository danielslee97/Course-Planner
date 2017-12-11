# Course-Planner

This program reads in a requirement file, a course offering file, and a sample schedule and it will tell you whether you are set to graduate or if there are mistakes or errors within your schedule (e.g. missing prerequirements, course not offered in that semester, etc.)

FORMAT FOR REQUIREMENT FILES:
  CREDITS X Y     --     CREDITS states that you need certain Y amount of X credits
                         SAMPLE: CREDITS C 20 (This means that you need 20 C credits to graduate)
                         
  COURSE X Y Z    --     COURSE states that X is a course that you can take, Y is a tag (e.g. M for Mandatory, R for Required, etc.) and Z                          are the course prerequirements 
                         SAMPLE: COURSE CS140 R (Required) CS101 CS110 (CS101 and CS110 are prereqs)
                         
  CHOOSE X Y      --     CHOOSE states that you need to choose X from Y choices
                         SAMPLE: CHOOSE 1 BI118 CH107 PH113 (You need one of these choices to graduate)
                         
FORMAT FOR COURSE OFFERING FILE:
   COURSE NAME  
