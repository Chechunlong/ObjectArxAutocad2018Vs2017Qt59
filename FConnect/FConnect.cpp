﻿#include "FConnect.hpp"

namespace sstd {
	extern void UCS2WCS(const double * i, double *o);
	static inline AcGePoint3d UcsToWorld(const AcGePoint3d& ucsPoint) {
		AcGePoint3d varAns;
		sstd::UCS2WCS(&ucsPoint.x, &varAns.x);
		return varAns;
	}
}

namespace sstd {
	extern void loadFConnect() {
		FConnect::load();
	}
}/*namespace sstd*/

namespace sstd {

	FConnect::FConnect() {

	}

	template<bool N = false>
	inline static void FConnect_main() try {

		auto $DB = acdbHostApplicationServices()->workingDatabase();

		class ThisLock {
		public:
			AcDbDatabase * const $DB;
			struct resbuf $osmode;
			struct resbuf $snapmode;
			struct resbuf $autosnap;
			inline ThisLock(AcDbDatabase * argDB) :$DB(argDB) {
				const static class SetValue {
				public:
					struct resbuf $osmode/*1*/;
					struct resbuf $snapmode/*0*/;
					struct resbuf $autosnap/*31*/;
					SetValue() {
						$osmode.restype = RTSHORT;
						$snapmode.restype = RTSHORT;
						$autosnap.restype = RTSHORT;

						$osmode.rbnext = nullptr;
						$snapmode.rbnext = nullptr;
						$autosnap.rbnext = nullptr;

						$autosnap.resval.rint = 31;
						$snapmode.resval.rint = 0;
						$osmode.resval.rint = 1;
					}

					void setValue() const {
						acedSetVar(LR"(AUTOSNAP)", &$autosnap);
						acedSetVar(LR"(SNAPMODE)", &$snapmode);
						acedSetVar(LR"(OSMODE)", &$osmode);
					}

				} varSetValue;

				acedGetVar(LR"(AUTOSNAP)", &$autosnap);
				acedGetVar(LR"(SNAPMODE)", &$snapmode);
				acedGetVar(LR"(OSMODE)", &$osmode);
				varSetValue.setValue();

			}
			inline ~ThisLock() {
				acedSetVar(LR"(AUTOSNAP)", &$autosnap);
				acedSetVar(LR"(SNAPMODE)", &$snapmode);
				acedSetVar(LR"(OSMODE)", &$osmode);
			}
		}varThisLock{ $DB };

		if constexpr (N) {
			AcGePoint3d varP;
			auto varGetP1 = [&varP1 = varP]() {
				auto varError = acedGetPoint(nullptr, LR"(拾取第一个点:)", &varP1.x);
				if (RTNORM == varError) {
					varP1 = UcsToWorld(varP1);
					return;
				}
				throw varError;
			};

			auto varGetP2 = [&varP1 = varP, &varP2 = varP]() {
				auto varError = acedGetPoint(&varP1.x, LR"(拾取第二个点:)", &varP2.x);
				if (RTNORM == varError) {
					varP2 = UcsToWorld(varP2);
					return;
				}
				throw varError;
			};

			sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
			sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;
			if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) { throw 1; }
			if (eOk != varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord, AcDb::kForWrite)) { throw 2; }

			varGetP1();
			auto varRunFunction = [&]() {
				auto varP1 = varP;
				varGetP2();
				if (varP1 == varP) { return; }
				sstd::ArxClosePointer<AcDbLine> varLine{ new AcDbLine(varP1, varP) };
				varBlockTableRecord->appendAcDbEntity(varLine);
				varLine->setLayer(LR"(粗实线)");
			};

			/************************/
			for (;;) {
				varRunFunction();
			}
		}
		else {

			AcGePoint3d varP1, varP2;
			/************************/
			auto varGetP1 = [&varP1]() {
				auto varError = acedGetPoint(nullptr, LR"(拾取第一个点:)", &varP1.x);
				if (RTNORM == varError) {
					varP1 = UcsToWorld(varP1);
					return;
				}
				throw varError;
			};
			/************************/
			auto varGetP2 = [&varP1, &varP2]() {
				auto varError = acedGetPoint(&varP1.x, LR"(拾取第二个点:)", &varP2.x);
				if (RTNORM == varError) {
					varP2 = UcsToWorld(varP2);
					return;
				}
				throw varError;
			};
			/************************/
			sstd::ArxClosePointer<AcDbBlockTable> varBlockTable;
			sstd::ArxClosePointer<AcDbBlockTableRecord> varBlockTableRecord;
			if (eOk != $DB->getBlockTable(varBlockTable, AcDb::kForRead)) { throw 1; }
			if (eOk != varBlockTable->getAt(ACDB_MODEL_SPACE, varBlockTableRecord, AcDb::kForWrite)) { throw 2; }
			/************************/
			auto varRunFunction = [&]() {
				varGetP1();
				varGetP2();
				if (varP1 == varP2) { return; }
				sstd::ArxClosePointer<AcDbLine> varLine{ new AcDbLine(varP1, varP2) };
				varBlockTableRecord->appendAcDbEntity(varLine);
				varLine->setLayer(LR"(粗实线)");
			};
			/************************/
			for (;;) {
				varRunFunction();
			}
		}

	}
	catch (...) {
		return;
	}

	void FConnect::main() {
		FConnect_main<false>();
	}

	namespace {
		class FXConnect {
		public:
			static void main() { FConnect_main<true>(); }
			DEFINE_ARX_NAME(LR"(_fxconnect)")
		};
	}

	void FConnect::load() {
		arx_add_main_command<FConnect>();
		arx_add_main_command<FXConnect>();
	}

}/*namespace sstd*/

