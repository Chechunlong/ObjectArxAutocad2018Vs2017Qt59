﻿varAns.emplace(LR"(@Standard)"sv, ApplyMaps::value_type::second_type{
	[](simple_code_args) {
	sstd::ArxClosePointer<AcDbTextStyleTableRecord> varLocalR;
	if (argR == nullptr) {
		argR = new AcDbTextStyleTableRecord;
		varLocalR = argR;
		argR->setName(argNM.data());
		argTST->add(argR);
	}

	argR->setFlagBits(argR->flagBits()&0b11111001)/*清除标志位*/;
	argR->setIsShapeFile(false)/**/;
	argR->setFileName(LR"(ztxt.shx)")/*shx字体文件名*/;
	argR->setBigFontFileName(LR"(whtmtxt.shx)")/*shx大字体文件名*/;
	argR->setTextSize(6.75)/*文字高度*/;
	argR->setXScale(0.8)/*宽度比*/;
	setAnnotative(argR,true)/*注释性*/;

},false });

