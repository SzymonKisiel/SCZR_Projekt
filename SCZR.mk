##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SCZR
ConfigurationName      :=Debug
WorkspacePath          :=/home/sk/Desktop/Allegro
ProjectPath            :=/home/sk/Desktop/Allegro/SCZR
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=sk
Date                   :=08/01/21
CodeLitePath           :=/home/sk/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="SCZR.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -pthread
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)usr/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)allegro $(LibrarySwitch)allegro_primitives $(LibrarySwitch)allegro_font 
ArLibs                 :=  "allegro" "allegro_primitives" "allegro_font" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)usr/lib 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/processC.cpp$(ObjectSuffix) $(IntermediateDirectory)/processD.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/processA.cpp$(ObjectSuffix) $(IntermediateDirectory)/processB.cpp$(ObjectSuffix) $(IntermediateDirectory)/functions.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/processC.cpp$(ObjectSuffix): processC.cpp $(IntermediateDirectory)/processC.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sk/Desktop/Allegro/SCZR/processC.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/processC.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/processC.cpp$(DependSuffix): processC.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/processC.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/processC.cpp$(DependSuffix) -MM processC.cpp

$(IntermediateDirectory)/processC.cpp$(PreprocessSuffix): processC.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/processC.cpp$(PreprocessSuffix) processC.cpp

$(IntermediateDirectory)/processD.cpp$(ObjectSuffix): processD.cpp $(IntermediateDirectory)/processD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sk/Desktop/Allegro/SCZR/processD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/processD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/processD.cpp$(DependSuffix): processD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/processD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/processD.cpp$(DependSuffix) -MM processD.cpp

$(IntermediateDirectory)/processD.cpp$(PreprocessSuffix): processD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/processD.cpp$(PreprocessSuffix) processD.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sk/Desktop/Allegro/SCZR/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/processA.cpp$(ObjectSuffix): processA.cpp $(IntermediateDirectory)/processA.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sk/Desktop/Allegro/SCZR/processA.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/processA.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/processA.cpp$(DependSuffix): processA.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/processA.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/processA.cpp$(DependSuffix) -MM processA.cpp

$(IntermediateDirectory)/processA.cpp$(PreprocessSuffix): processA.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/processA.cpp$(PreprocessSuffix) processA.cpp

$(IntermediateDirectory)/processB.cpp$(ObjectSuffix): processB.cpp $(IntermediateDirectory)/processB.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sk/Desktop/Allegro/SCZR/processB.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/processB.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/processB.cpp$(DependSuffix): processB.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/processB.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/processB.cpp$(DependSuffix) -MM processB.cpp

$(IntermediateDirectory)/processB.cpp$(PreprocessSuffix): processB.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/processB.cpp$(PreprocessSuffix) processB.cpp

$(IntermediateDirectory)/functions.cpp$(ObjectSuffix): functions.cpp $(IntermediateDirectory)/functions.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/sk/Desktop/Allegro/SCZR/functions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/functions.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/functions.cpp$(DependSuffix): functions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/functions.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/functions.cpp$(DependSuffix) -MM functions.cpp

$(IntermediateDirectory)/functions.cpp$(PreprocessSuffix): functions.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/functions.cpp$(PreprocessSuffix) functions.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


