cmake_minimum_required(VERSION 2.8)	# Проверка версии CMake.
					# Если версия установленой программы
					# старее указаной, произайдёт аварийный выход.

project(NeuArch)			# Название проекта

include_directories(.)			# Расположение заголовочных файлов
include_directories(../Matrix.041)	# Расположение заголовочных файлов

include(GenerateExportHeader)

if(WIN32)
set(LIB_DEST_DIR bin)
else()
set(LIB_DEST_DIR lib)
endif(WIN32)

set(SOURCE_LIB NaConLaw.cpp
	NaConfig.cpp
	NaDDir.cpp
	NaDataIO.cpp
	NaBinrIO.cpp
	NaDynAr.cpp
	NaExcept.cpp
	NaInfoSt.cpp
	NaLiNois.cpp
	NaLinCon.cpp
	NaLogFil.cpp
	NaMath.cpp
	NaMatrix.cpp
	NaNDescr.cpp
	NaNNCon.cpp
	NaNNLrn.cpp
	NaNNPObj.cpp
	NaNNUnit.cpp
	NaPCnInp.cpp
	NaPCnOut.cpp
	NaPID.cpp
	NaPNAct.cpp
	NaPNBu21.cpp
	NaPNBu31.cpp
	NaPNBu12.cpp
	NaPNChPt.cpp
	NaPNCmp.cpp
	NaPNCnst.cpp
	NaPNDely.cpp
	NaPNFIn.cpp
	NaPNFOut.cpp
	NaPNFetc.cpp
	NaPNGen.cpp
	NaPNKalm.cpp
	NaPNRand.cpp
	NaPNStat.cpp
	NaPNSum.cpp
	NaPNSwap.cpp
	NaPNSwit.cpp
	NaPNTchr.cpp
	NaPNTimD.cpp
	NaPNTime.cpp
	NaPNTran.cpp
	NaPNTrig.cpp
	NaPNLAnd.cpp
	NaPetCn.cpp
	NaPetNet.cpp
	NaPetNod.cpp
	NaPetri.cpp
	NaPlotIO.cpp
	NaQProp.cpp
	NaRandom.cpp
	NaSSCtrl.cpp
	NaSSPlan.cpp
	NaStdBPE.cpp
	NaStrOps.cpp
	NaSysSch.cpp
	NaTextIO.cpp
	NaTimer.cpp
	NaTrFunc.cpp
	NaCoFunc.cpp
	NaCuFunc.cpp
	NaExFunc.cpp
	NaUnit.cpp
	NaVector.cpp
	NaParams.cpp
	NaPNDerv.cpp
	NaPNQIn.cpp
	NaPNQOut.cpp
	NaPNSkip.cpp
	NaPNFill.cpp
	NaPNNNUn.cpp
	NaPNNNCo.cpp
	NaPNCuSu.cpp
	NaPNWatc.cpp
	NaPNTDG.cpp
	NaSSModel.cpp
	NaPNSS.cpp
	NaPNAccu.cpp
	NaInvPnd.cpp
	NaWMR.cpp
	kbdif.cpp)			# Установка переменной со списком исходников для библиотеки

if(WIN32)
set(SOURCE_LIB ${SOURCE_LIB} vsscanf.win.c)
endif(WIN32)


#add_library(pnna STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем pnna
add_library(pnna SHARED ${SOURCE_LIB})	# Создание динамической библиотеки с именем pnna
generate_export_header( pnna
			EXPORT_MACRO_NAME PNNA_API
			EXPORT_FILE_NAME pnna_api.h
			STATIC_DEFINE PNNA_BUILT_AS_STATIC)
target_link_libraries(pnna matrix)



install(TARGETS pnna LIBRARY DESTINATION ${LIB_DEST_DIR} RUNTIME DESTINATION ${LIB_DEST_DIR})

if(WIN32)
set(SOURCE_EXE test_vsscanf.cpp vsscanf.win.c)
add_executable(test_vsscanf ${SOURCE_EXE})
#target_link_libraries(test_vsscanf pnna matrix)
endif(WIN32)

add_subdirectory(ExFuncs)
