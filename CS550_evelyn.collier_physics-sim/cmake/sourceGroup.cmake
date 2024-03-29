Function(sourceGroup given_root sourceFiles) 

 # This will determine if the given files are in a folder or not and separate 
 # them on that alone. 
 ForEach(file ${${sourceFiles}}) 
   File(TO_CMAKE_PATH ${file} resultFile) 
   Get_Filename_Component(nameComponent ${resultFile} NAME) 
   Get_Filename_Component(fullPath ${nameComponent} ABSOLUTE) 

   If(EXISTS ${fullPath}) 
     Set(notInAFolder ${notInAFolder} ${file}) 
   Else() 
     Set(inAFolder ${inAFolder} ${file}) 
   EndIf() 
 EndForEach() 

 # We use a no space prefix with files from folders, otherwise the filters  
 # don't get made. 
 Source_Group(TREE ${${given_root}}  
              PREFIX "" 
              FILES ${inAFolder}) 

 # We use a one space prefix with files not in folders, otherwise the files 
 # are put into "Source Files" and "Header Files" filters. 
 Source_Group(TREE ${${given_root}}  
              PREFIX " " 
              FILES ${notInAFolder}) 
EndFunction() 
