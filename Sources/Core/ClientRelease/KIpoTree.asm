	TITLE	D:\TINHDUYEN\SwordOnline\SwordOnline\Sources\Core\Src\Scene\KIpoTree.cpp
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??_C@_00A@?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??8@YAHABU_GUID@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1KNode@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GKNode@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0KNode@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetNext@KNode@@QAEPAV1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetPrev@KNode@@QAEPAV1@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertBefore@KNode@@QAEXPAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?InsertAfter@KNode@@QAEXPAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Remove@KNode@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?GetName@KStrNode@@UAEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?SetName@KStrNode@@UAEXPAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetHead@KList@@QAEPAVKNode@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddTail@KList@@QAEXPAVKNode@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GKCacheNode@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1KStrNode@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1KCacheNode@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?GetMemPtr@KMemClass@@QAEPAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1KITabFile@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@tagSHOW_MSG_SYNC@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@tagPLAYER_COMMAND@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1KScriptNode@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@SOBJ_LIGHT_DATA@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Release@OBJ_SKILL_DATA@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GKIndexNode@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1KIndexNode@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Clear@SACTTABLEOFF@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Clear@SECTINFO@KNpcResNode@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0KWorldMsgNode@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GKWorldMsgNode@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1KWorldMsgNode@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Remove@KMissleMagicAttribsData@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1ISkill@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSkillReqLevel@ISkill@@UBEGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsAura@ISkill@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsPhysical@ISkill@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsSkillPhysical@ISkill@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsMelee@ISkill@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsUseAR@ISkill@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?IsSkillMagic@ISkill@@UBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSkillLevelUpScriptId@ISkill@@UBEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GISkill@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetSkill@KSkillManager@@QAEPAVISkill@@HH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetColor@KLColor@@QAEXK@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Scale@KLColor@@QAEXM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetColor@KLColor@@QAEKXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemchr
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemcmp
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemcpy
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemmove
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemset
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?eof@?$char_traits@G@std@@SAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?assign@?$char_traits@D@std@@SAXAADABD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?length@?$char_traits@D@std@@SAIPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?copy@?$char_traits@D@std@@SAPADPADPBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?move@?$char_traits@D@std@@SAPADPADPBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?eof@?$char_traits@D@std@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1bad_exception@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0bad_exception@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1bad_alloc@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0bad_alloc@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0logic_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1logic_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?what@logic_error@std@@UBEPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0logic_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1domain_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0domain_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1invalid_argument@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@invalid_argument@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Ginvalid_argument@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0invalid_argument@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1length_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0length_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1out_of_range@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0out_of_range@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0runtime_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1runtime_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0runtime_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1overflow_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0overflow_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1underflow_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0underflow_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1range_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0range_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Breath@KLightBase@@UAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0KSelfBreathLight@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0KLightBase@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Breath@KSelfBreathLight@@UAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0KIpoTree@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1KIpoTree@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Paint@KIpoTree@@QAEXPAUtagRECT@@W4IPOT_RENDER_LAYER@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Breathe@KIpoTree@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Clear@KIpoTree@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GKIpotBranch@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RemoveRtoGroupWithPermanentLeaf@KIpoTree@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddBranch@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Fell@KIpoTree@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddLeafLine@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddLeafPoint@KIpoTree@@QAEXPAUKIpotLeaf@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PluckRto@KIpoTree@@QAEXPAUKIpotRuntimeObj@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetPermanentBranchPos@KIpoTree@@QAEXHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?StrewRtoLeafs@KIpoTree@@QAEXAAUtagRECT@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?FillLineObstacle@KIpoTree@@AAEXHHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RenderLightMap@KIpoTree@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CanLighting@KIpoTree@@AAE_NHHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?AddBuildinLight@KIpoTree@@QAEXPAUKBuildInLightInfo@@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableBioLights@KIpoTree@@QAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?EnableDynamicLights@KIpoTree@@QAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?c_str@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@ABV?$allocator@PAVKLightBase@@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?begin@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE?AViterator@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?end@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE?AViterator@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?size@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?push_back@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAEXABQAVKLightBase@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?erase@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE?AViterator@12@V312@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?clear@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Diterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QBEAAPAVKLightBase@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Eiterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAEAAV012@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??9iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Nullstr@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@CAPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?insert@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE?AViterator@12@V312@ABQAVKLightBase@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?erase@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE?AViterator@12@V312@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Buynode@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@IAEPAU_Node@12@PAU312@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Freenode@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@IAEXPAU_Node@12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Next@_Acc@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@SAAAPAU_Node@23@PAU423@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Prev@_Acc@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@SAAAPAU_Node@23@PAU423@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Value@_Acc@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@SAAAPAVKLightBase@@PAU_Node@23@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?destroy@?$allocator@PAVKLightBase@@@std@@QAEXPAPAVKLightBase@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0const_iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Mynode@const_iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QBEPAU_Node@23@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@PAU_Node@12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Eiterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE?AV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?capacity@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Refcnt@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEAAEPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?deallocate@?$allocator@D@std@@QAEXPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Charalloc@?$allocator@PAVKLightBase@@@std@@QAEPADI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?deallocate@?$allocator@PAVKLightBase@@@std@@QAEXPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?construct@?$allocator@PAVKLightBase@@@std@@QAEXPAPAVKLightBase@@ABQAV3@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0const_iterator@?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@PAU_Node@12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?_Split@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?allocate@?$allocator@D@std@@QAEPADIPBX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?max_size@?$allocator@D@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT __imp_?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8std@@YA_NABV?$allocator@D@0@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Destroy@std@@YAXPAPAVKLightBase@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Allocate@std@@YAPADHPAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Construct@std@@YAXPAPAVKLightBase@@ABQAV2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_7KNode@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_R0?AVruntime_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA2?AVlogic_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA3?AVrange_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVout_of_range@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_7KIndexNode@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __CT??_R0?AVoverflow_error@std@@@8??0overflow_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA2?AVruntime_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVunderflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA3?AVoverflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVrange_error@std@@@8??0range_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVbad_alloc@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_7KWorldMsgNode@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __CT??_R0?AVlength_error@std@@@8??0length_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVlength_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVlength_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVdomain_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA2?AVbad_exception@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVruntime_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVout_of_range@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA3?AVunderflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVrange_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVoverflow_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA2?AVbad_alloc@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_7invalid_argument@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __TI3?AVinvalid_argument@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVbad_alloc@std@@@8??0bad_alloc@std@@QAE@ABV01@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVlogic_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_R0?AVexception@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CT??_R0?AVout_of_range@std@@@8??0out_of_range@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVdomain_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA3?AVinvalid_argument@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_7KCacheNode@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __CT??_R0?AVexception@@@8??0exception@@QAE@ABV0@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVunderflow_error@std@@@8??0underflow_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVdomain_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVlength_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_7ISkill@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_R0?AVunderflow_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_7KLightBase@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __CT??_R0?AVinvalid_argument@std@@@8??0invalid_argument@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVbad_exception@std@@@8??0bad_exception@std@@QAE@ABV01@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVlogic_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVoverflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVdomain_error@std@@@8??0domain_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVruntime_error@std@@@8??0runtime_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVinvalid_argument@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_R0?AVrange_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __TI2?AVbad_exception@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVlogic_error@std@@@8??0logic_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVbad_alloc@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_7KSelfBreathLight@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_R0?AVbad_exception@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVout_of_range@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
FLAT	GROUP _DATA, CONST, _BSS, xdata$x
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
_DATA	SEGMENT
_?nObstacleID@?1??ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z@4HA DD 01H
_DATA	ENDS
PUBLIC	??0KSelfBreathLight@@QAE@XZ			; KSelfBreathLight::KSelfBreathLight
PUBLIC	?Breath@KSelfBreathLight@@UAEXXZ		; KSelfBreathLight::Breath
PUBLIC	??_7KSelfBreathLight@@6B@			; KSelfBreathLight::`vftable'
EXTRN	__imp__timeGetTime@0:NEAR
;	COMDAT ??_7KSelfBreathLight@@6B@
; File D:\TINHDUYEN\SwordOnline\SwordOnline\Sources\Core\Src\Scene\KIpoTree.cpp
CONST	SEGMENT
??_7KSelfBreathLight@@6B@ DD FLAT:?Breath@KSelfBreathLight@@UAEXXZ ; KSelfBreathLight::`vftable'
CONST	ENDS
;	COMDAT ??0KSelfBreathLight@@QAE@XZ
_TEXT	SEGMENT
??0KSelfBreathLight@@QAE@XZ PROC NEAR			; KSelfBreathLight::KSelfBreathLight, COMDAT

; 25   : {

	push	esi
	mov	esi, ecx
	mov	DWORD PTR [esi], OFFSET FLAT:??_7KSelfBreathLight@@6B@ ; KSelfBreathLight::`vftable'

; 26   : 	nDir = 1;

	mov	DWORD PTR [esi+44], 1

; 27   : 	dwLastBreathTime = timeGetTime();

	call	DWORD PTR __imp__timeGetTime@0
	mov	DWORD PTR [esi+48], eax

; 28   : }

	mov	eax, esi
	pop	esi
	ret	0
??0KSelfBreathLight@@QAE@XZ ENDP			; KSelfBreathLight::KSelfBreathLight
_TEXT	ENDS
EXTRN	__ftol:NEAR
EXTRN	__fltused:NEAR
;	COMDAT ?Breath@KSelfBreathLight@@UAEXXZ
_TEXT	SEGMENT
?Breath@KSelfBreathLight@@UAEXXZ PROC NEAR		; KSelfBreathLight::Breath, COMDAT

; 31   : {

	sub	esp, 12					; 0000000cH
	push	esi
	push	edi
	mov	esi, ecx

; 32   : 	DWORD dwCurTime = timeGetTime();

	call	DWORD PTR __imp__timeGetTime@0

; 33   : 	
; 34   : 	fRadius += (dwCurTime - dwLastBreathTime) / fCycle * (fMaxRange - fMinRange) * nDir * 2;

	mov	edx, DWORD PTR [esi+48]
	mov	edi, eax
	sub	eax, edx
	mov	DWORD PTR -8+[esp+24], 0
	mov	DWORD PTR -8+[esp+20], eax
	mov	ecx, DWORD PTR [esi+44]
	fild	QWORD PTR -8+[esp+20]
	mov	DWORD PTR -12+[esp+20], ecx

; 35   : 	if(nDir == 1 && fRadius > fMaxRange)

	cmp	ecx, 1
	fdiv	DWORD PTR [esi+40]
	fld	DWORD PTR [esi+32]
	fsub	DWORD PTR [esi+28]
	fmulp	ST(1), ST(0)
	fimul	DWORD PTR -12+[esp+20]
	fadd	ST(0), ST(0)
	fadd	DWORD PTR [esi+36]
	fst	DWORD PTR [esi+36]
	jne	SHORT $L88563
	fcom	DWORD PTR [esi+32]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L88563

; 36   : 	{
; 37   : 		nDir = -1;
; 38   : 		fRadius = fMaxRange;

	mov	ecx, DWORD PTR [esi+32]
	mov	DWORD PTR [esi+44], -1
	fstp	ST(0)
	mov	DWORD PTR [esi+36], ecx

; 44   : 	}
; 45   : 	m_nRadius = (int)fRadius;

	fld	DWORD PTR [esi+36]
	call	__ftol

; 46   : 	dwLastBreathTime = dwCurTime;

	mov	DWORD PTR [esi+48], edi
	mov	DWORD PTR [esi+24], eax
	pop	edi
	pop	esi

; 47   : }

	add	esp, 12					; 0000000cH
	ret	0
$L88563:

; 39   : 	}
; 40   : 	else if(nDir == -1 && fRadius < fMinRange)

	cmp	ecx, -1
	jne	SHORT $L89629
	fcomp	DWORD PTR [esi+28]
	fnstsw	ax
	test	ah, 1
	je	SHORT $L88565

; 41   : 	{
; 42   : 		nDir = 1;
; 43   : 		fRadius = fMinRange;

	mov	edx, DWORD PTR [esi+28]
	mov	DWORD PTR [esi+44], 1
	mov	DWORD PTR [esi+36], edx

; 44   : 	}
; 45   : 	m_nRadius = (int)fRadius;

	fld	DWORD PTR [esi+36]
	call	__ftol

; 46   : 	dwLastBreathTime = dwCurTime;

	mov	DWORD PTR [esi+48], edi
	mov	DWORD PTR [esi+24], eax
	pop	edi
	pop	esi

; 47   : }

	add	esp, 12					; 0000000cH
	ret	0
$L89629:

; 41   : 	{
; 42   : 		nDir = 1;
; 43   : 		fRadius = fMinRange;

	fstp	ST(0)
$L88565:

; 44   : 	}
; 45   : 	m_nRadius = (int)fRadius;

	fld	DWORD PTR [esi+36]
	call	__ftol

; 46   : 	dwLastBreathTime = dwCurTime;

	mov	DWORD PTR [esi+48], edi
	mov	DWORD PTR [esi+24], eax
	pop	edi
	pop	esi

; 47   : }

	add	esp, 12					; 0000000cH
	ret	0
?Breath@KSelfBreathLight@@UAEXXZ ENDP			; KSelfBreathLight::Breath
_TEXT	ENDS
PUBLIC	??0KIpoTree@@QAE@XZ				; KIpoTree::KIpoTree
PUBLIC	??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ ; std::list<KLightBase *,std::allocator<KLightBase *> >::~list<KLightBase *,std::allocator<KLightBase *> >
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	??0KIpotBranch@@QAE@XZ:NEAR			; KIpotBranch::KIpotBranch
EXTRN	??1KIpotBranch@@QAE@XZ:NEAR			; KIpotBranch::~KIpotBranch
;	COMDAT xdata$x
; File C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\list
xdata$x	SEGMENT
$T89703	DD	019930520H
	DD	02H
	DD	FLAT:$T89706
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T89706	DD	0ffffffffH
	DD	FLAT:$L89637
	DD	00H
	DD	FLAT:$L89638
xdata$x	ENDS
;	COMDAT ??0KIpoTree@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
$T89634 = -17
__$EHRec$ = -12
??0KIpoTree@@QAE@XZ PROC NEAR				; KIpoTree::KIpoTree, COMDAT

; 51   : {

	push	-1
	push	$L89704
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	sub	esp, 8
	push	ebx
	push	esi
	mov	esi, ecx
	push	edi
	mov	DWORD PTR _this$[esp+32], esi
	lea	ecx, DWORD PTR [esi+4]
	call	??0KIpotBranch@@QAE@XZ			; KIpotBranch::KIpotBranch
	mov	al, BYTE PTR $T89634[esp+32]
	xor	ebx, ebx
	push	12					; 0000000cH
	mov	DWORD PTR __$EHRec$[esp+44], ebx
	mov	BYTE PTR [esi+92272], al
	call	??2@YAPAXI@Z				; operator new
	mov	DWORD PTR [eax], eax
	mov	DWORD PTR [eax+4], eax
	mov	DWORD PTR [esi+92276], eax
	mov	DWORD PTR [esi+92280], ebx
	mov	al, 1

; 52   : 	m_pMainBranch = NULL;
; 53   : 	m_bProcessBioLights = true;
; 54   : 	m_bDynamicLighting = true;
; 55   : 	m_nCurrentTime = 0;
; 56   : 	m_bIsIndoor = false;
; 57   : 	m_dwAmbient = 0xff000000;
; 58   : 	memset(&m_PermanentLeaf, 0, sizeof(m_PermanentLeaf));

	mov	ecx, 6
	mov	BYTE PTR __$EHRec$[esp+44], al
	mov	BYTE PTR [esi+89], al
	mov	BYTE PTR [esi+90], al
	xor	eax, eax
	lea	edi, DWORD PTR [esi+52]
	mov	DWORD PTR [esi], ebx
	mov	DWORD PTR [esi+84], ebx
	mov	BYTE PTR [esi+88], bl
	mov	DWORD PTR [esi+92], -16777216		; ff000000H

; 59   : 
; 60   :     ptrdiff_t ulAddress;
; 61   : 
; 62   :     m_pbyLColorBase = new unsigned char [(sizeof(KLColor) * LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT) + 255];

	push	37119					; 000090ffH
	rep stosd
	call	??2@YAPAXI@Z				; operator new
	mov	DWORD PTR [esi+73828], eax

; 63   :     _ASSERT(m_pbyLColorBase);
; 64   :     ulAddress = (ptrdiff_t)m_pbyLColorBase;
; 65   :     ulAddress = (ulAddress + 255) / 256 * 256;

	add	eax, 255				; 000000ffH
	cdq
	and	edx, 255				; 000000ffH

; 66   :     pLColor = (KLColor *)ulAddress;
; 67   : 
; 68   :     m_pby_ptpBase = new unsigned char [(sizeof(KLColor) * LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT) + 255];

	push	37119					; 000090ffH
	add	eax, edx
	sar	eax, 8
	shl	eax, 8
	mov	DWORD PTR [esi+73824], eax
	call	??2@YAPAXI@Z				; operator new
	mov	DWORD PTR [esi+73836], eax

; 69   :     _ASSERT(m_pby_ptpBase);
; 70   :     ulAddress = (ptrdiff_t)m_pby_ptpBase;
; 71   :     ulAddress = (ulAddress + 255) / 256 * 256;

	add	eax, 255				; 000000ffH
	cdq
	and	edx, 255				; 000000ffH

; 72   :     ptp = (KLColor *)ulAddress;
; 73   : }

	mov	ecx, DWORD PTR __$EHRec$[esp+44]
	add	eax, edx
	add	esp, 12					; 0000000cH
	sar	eax, 8
	shl	eax, 8
	mov	DWORD PTR [esi+73832], eax
	mov	eax, esi
	pop	edi
	pop	esi
	pop	ebx
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 20					; 00000014H
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L89637:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	jmp	??1KIpotBranch@@QAE@XZ			; KIpotBranch::~KIpotBranch
$L89638:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92272				; 00016870H
	jmp	??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ ; std::list<KLightBase *,std::allocator<KLightBase *> >::~list<KLightBase *,std::allocator<KLightBase *> >
$L89704:
	mov	eax, OFFSET FLAT:$T89703
	jmp	___CxxFrameHandler
text$x	ENDS
??0KIpoTree@@QAE@XZ ENDP				; KIpoTree::KIpoTree
PUBLIC	??1KIpoTree@@QAE@XZ				; KIpoTree::~KIpoTree
PUBLIC	?Clear@KIpoTree@@QAEXXZ				; KIpoTree::Clear
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
;	COMDAT xdata$x
; File C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\xmemory
xdata$x	SEGMENT
$T90027	DD	019930520H
	DD	02H
	DD	FLAT:$T90030
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T90030	DD	0ffffffffH
	DD	FLAT:$L89715
	DD	00H
	DD	FLAT:$L89716
xdata$x	ENDS
;	COMDAT ??1KIpoTree@@QAE@XZ
_TEXT	SEGMENT
_this$ = -16
__$EHRec$ = -12
??1KIpoTree@@QAE@XZ PROC NEAR				; KIpoTree::~KIpoTree, COMDAT

; 77   : {

	push	-1
	push	$L90029
	mov	eax, DWORD PTR fs:__except_list
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ecx
	push	ebx
	push	ebp
	push	esi
	mov	esi, ecx
	push	edi
	mov	DWORD PTR _this$[esp+32], esi
	mov	DWORD PTR __$EHRec$[esp+40], 1

; 78   : 	Clear();

	call	?Clear@KIpoTree@@QAEXXZ			; KIpoTree::Clear

; 79   : 
; 80   :     if (m_pby_ptpBase)

	mov	eax, DWORD PTR [esi+73836]
	xor	ebp, ebp
	cmp	eax, ebp
	je	SHORT $L88588

; 81   :     {
; 82   :         delete []m_pby_ptpBase;

	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 83   :         m_pby_ptpBase = NULL;

	mov	DWORD PTR [esi+73836], ebp

; 84   :         ptp = NULL;

	mov	DWORD PTR [esi+73832], ebp
$L88588:

; 85   :     }
; 86   : 
; 87   :     if (m_pbyLColorBase)

	mov	eax, DWORD PTR [esi+73828]
	cmp	eax, ebp
	je	SHORT $L88590

; 88   :     {
; 89   :         delete []m_pbyLColorBase;

	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 90   :         m_pbyLColorBase = NULL;

	mov	DWORD PTR [esi+73828], ebp

; 91   : 
; 92   :         pLColor = NULL;

	mov	DWORD PTR [esi+73824], ebp
$L88590:

; 93   :     }
; 94   : }

	mov	ebx, DWORD PTR [esi+92276]
	mov	edi, DWORD PTR [ebx]
	cmp	edi, ebx
	je	SHORT $L89971
$L89970:
	mov	eax, edi
	mov	edi, DWORD PTR [edi]
	push	eax
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx], edx
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+4], edx
	call	??3@YAXPAX@Z				; operator delete
	mov	ecx, DWORD PTR [esi+92280]
	add	esp, 4
	dec	ecx
	cmp	edi, ebx
	mov	DWORD PTR [esi+92280], ecx
	jne	SHORT $L89970
$L89971:
	mov	eax, DWORD PTR [esi+92276]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
	lea	ecx, DWORD PTR [esi+4]
	mov	DWORD PTR [esi+92276], ebp
	mov	DWORD PTR [esi+92280], ebp
	mov	DWORD PTR __$EHRec$[esp+40], -1
	call	??1KIpotBranch@@QAE@XZ			; KIpotBranch::~KIpotBranch
	mov	ecx, DWORD PTR __$EHRec$[esp+32]
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx
	mov	DWORD PTR fs:__except_list, ecx
	add	esp, 16					; 00000010H
	ret	0
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L89715:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 4
	jmp	??1KIpotBranch@@QAE@XZ			; KIpotBranch::~KIpotBranch
$L89716:
	mov	ecx, DWORD PTR _this$[ebp]
	add	ecx, 92272				; 00016870H
	jmp	??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ ; std::list<KLightBase *,std::allocator<KLightBase *> >::~list<KLightBase *,std::allocator<KLightBase *> >
$L90029:
	mov	eax, OFFSET FLAT:$T90027
	jmp	___CxxFrameHandler
text$x	ENDS
??1KIpoTree@@QAE@XZ ENDP				; KIpoTree::~KIpoTree
PUBLIC	?Paint@KIpoTree@@QAEXPAUtagRECT@@W4IPOT_RENDER_LAYER@@@Z ; KIpoTree::Paint
PUBLIC	?RenderLightMap@KIpoTree@@QAEXXZ		; KIpoTree::RenderLightMap
EXTRN	?g_pRepresent@@3PAUiRepresentShell@@A:DWORD	; g_pRepresent
EXTRN	?PaintObjectLayer@KIpotBranch@@QAEXPAUtagRECT@@@Z:NEAR ; KIpotBranch::PaintObjectLayer
EXTRN	?PaintNoneObjectLayer@KIpotBranch@@QAEXPAUtagRECT@@H@Z:NEAR ; KIpotBranch::PaintNoneObjectLayer
;	COMDAT ?Paint@KIpoTree@@QAEXPAUtagRECT@@W4IPOT_RENDER_LAYER@@@Z
_TEXT	SEGMENT
_pRepresentArea$ = 8
_eLayer$ = 12
?Paint@KIpoTree@@QAEXPAUtagRECT@@W4IPOT_RENDER_LAYER@@@Z PROC NEAR ; KIpoTree::Paint, COMDAT

; 98   : {

	push	esi
	push	edi

; 99   : 	if(eLayer == IPOT_RL_COVER_GROUND && m_bDynamicLighting)

	mov	edi, DWORD PTR _eLayer$[esp+4]
	mov	esi, ecx
	cmp	edi, 1
	jne	SHORT $L88597
	mov	al, BYTE PTR [esi+90]
	test	al, al
	je	SHORT $L88599

; 100  : 	{
; 101  : 		// 渲染光照图
; 102  : 		RenderLightMap();

	call	?RenderLightMap@KIpoTree@@QAEXXZ	; KIpoTree::RenderLightMap

; 103  : 		// 设置表现模块的光照信息
; 104  : 		g_pRepresent->SetLightInfo(m_nLeftTopX, m_nLeftTopY, (unsigned int*)pLightingArray);

	mov	ecx, DWORD PTR ?g_pRepresent@@3PAUiRepresentShell@@A ; g_pRepresent
	lea	edx, DWORD PTR [esi+73840]
	push	edx
	mov	edx, DWORD PTR [esi+80]
	mov	eax, DWORD PTR [ecx]
	push	edx
	mov	edx, DWORD PTR [esi+76]
	push	edx
	call	DWORD PTR [eax+104]
$L88599:

; 113  : 	}
; 114  : 	else if (m_pMainBranch)

	mov	ecx, DWORD PTR [esi]

; 115  : 		m_pMainBranch->PaintNoneObjectLayer(pRepresentArea, eLayer);

	push	edi
	test	ecx, ecx
	je	SHORT $L88603
	mov	edx, DWORD PTR _pRepresentArea$[esp+8]
	push	edx

; 116  : 	else
; 117  : 		m_DefaultBranch.PaintNoneObjectLayer(pRepresentArea, eLayer);

	call	?PaintNoneObjectLayer@KIpotBranch@@QAEXPAUtagRECT@@H@Z ; KIpotBranch::PaintNoneObjectLayer
	pop	edi
	pop	esi

; 118  : }

	ret	8
$L88597:

; 105  : 	}
; 106  : 	
; 107  : 	if (eLayer == IPOT_RL_OBJECT)

	cmp	edi, 2
	jne	SHORT $L88599

; 108  : 	{
; 109  : 		if (m_pMainBranch)

	mov	ecx, DWORD PTR [esi]
	test	ecx, ecx
	je	SHORT $L88600

; 110  : 			m_pMainBranch->PaintObjectLayer(pRepresentArea);

	mov	eax, DWORD PTR _pRepresentArea$[esp+4]
	push	eax
	call	?PaintObjectLayer@KIpotBranch@@QAEXPAUtagRECT@@@Z ; KIpotBranch::PaintObjectLayer
	pop	edi
	pop	esi

; 118  : }

	ret	8
$L88600:

; 111  : 		else
; 112  : 			m_DefaultBranch.PaintObjectLayer(pRepresentArea);

	mov	ecx, DWORD PTR _pRepresentArea$[esp+4]
	push	ecx
	lea	ecx, DWORD PTR [esi+4]
	call	?PaintObjectLayer@KIpotBranch@@QAEXPAUtagRECT@@@Z ; KIpotBranch::PaintObjectLayer
	pop	edi
	pop	esi

; 118  : }

	ret	8
$L88603:

; 116  : 	else
; 117  : 		m_DefaultBranch.PaintNoneObjectLayer(pRepresentArea, eLayer);

	mov	eax, DWORD PTR _pRepresentArea$[esp+8]
	lea	ecx, DWORD PTR [esi+4]
	push	eax
	call	?PaintNoneObjectLayer@KIpotBranch@@QAEXPAUtagRECT@@H@Z ; KIpotBranch::PaintNoneObjectLayer
	pop	edi
	pop	esi

; 118  : }

	ret	8
?Paint@KIpoTree@@QAEXPAUtagRECT@@W4IPOT_RENDER_LAYER@@@Z ENDP ; KIpoTree::Paint
_TEXT	ENDS
PUBLIC	?Breathe@KIpoTree@@QAEXXZ			; KIpoTree::Breathe
;	COMDAT ?Breathe@KIpoTree@@QAEXXZ
_TEXT	SEGMENT
?Breathe@KIpoTree@@QAEXXZ PROC NEAR			; KIpoTree::Breathe, COMDAT

; 121  : {

	push	esi
	push	edi
	mov	edi, ecx

; 122  : 	list<KLightBase*>::iterator i;
; 123  : 	for (i = m_LightList.begin(); i != m_LightList.end(); ++i)

	mov	eax, DWORD PTR [edi+92276]
	mov	esi, DWORD PTR [eax]
	cmp	esi, eax
	je	SHORT $L88614
$L90078:

; 124  : 	{
; 125  : 		if((*i)->m_pParent == NULL)

	mov	ecx, DWORD PTR [esi+8]
	mov	eax, DWORD PTR [ecx+4]
	test	eax, eax
	jne	SHORT $L88613

; 126  : 			(*i)->Breath();

	mov	eax, DWORD PTR [ecx]
	call	DWORD PTR [eax]
$L88613:
	mov	esi, DWORD PTR [esi]
	mov	eax, DWORD PTR [edi+92276]
	cmp	esi, eax
	jne	SHORT $L90078
$L88614:
	pop	edi
	pop	esi

; 127  : 	}
; 128  : }

	ret	0
?Breathe@KIpoTree@@QAEXXZ ENDP				; KIpoTree::Breathe
_TEXT	ENDS
PUBLIC	?RemoveRtoGroupWithPermanentLeaf@KIpoTree@@AAEXXZ ; KIpoTree::RemoveRtoGroupWithPermanentLeaf
EXTRN	?Clear@KIpotBranch@@QAEXXZ:NEAR			; KIpotBranch::Clear
;	COMDAT ?Clear@KIpoTree@@QAEXXZ
_TEXT	SEGMENT
?Clear@KIpoTree@@QAEXXZ PROC NEAR			; KIpoTree::Clear, COMDAT

; 132  : {

	push	ebx
	push	esi
	mov	esi, ecx
	push	edi

; 133  : 	if (m_pMainBranch)

	mov	edi, DWORD PTR [esi]
	test	edi, edi
	je	SHORT $L88619

; 134  : 	{
; 135  : 		delete (m_pMainBranch);

	mov	ecx, edi
	call	??1KIpotBranch@@QAE@XZ			; KIpotBranch::~KIpotBranch
	push	edi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 136  : 		m_pMainBranch = NULL;

	mov	DWORD PTR [esi], 0
$L88619:

; 137  : 	}
; 138  : 	m_DefaultBranch.Clear();

	lea	ecx, DWORD PTR [esi+4]
	call	?Clear@KIpotBranch@@QAEXXZ		; KIpotBranch::Clear

; 139  : 	RemoveRtoGroupWithPermanentLeaf();

	mov	ecx, esi
	call	?RemoveRtoGroupWithPermanentLeaf@KIpoTree@@AAEXXZ ; KIpoTree::RemoveRtoGroupWithPermanentLeaf

; 140  : 	
; 141  : 	//clear all dyna light
; 142  : 	list<KLightBase*>::iterator i;
; 143  : 	for (i = m_LightList.begin(); i != m_LightList.end(); ++i)

	mov	eax, DWORD PTR [esi+92276]
	mov	edi, DWORD PTR [eax]
	cmp	edi, eax
	je	SHORT $L88628
$L90176:

; 144  : 	{
; 145  : 		delete (*i);

	mov	eax, DWORD PTR [edi+8]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	mov	edi, DWORD PTR [edi]
	mov	eax, DWORD PTR [esi+92276]
	add	esp, 4
	cmp	edi, eax
	jne	SHORT $L90176
$L88628:

; 146  : 	}
; 147  : 	m_LightList.clear();

	mov	ebx, DWORD PTR [esi+92276]
	mov	edi, DWORD PTR [ebx]
	cmp	edi, ebx
	je	SHORT $L90459
$L90458:
	mov	eax, edi
	mov	edi, DWORD PTR [edi]
	push	eax
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx], edx
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+4], edx
	call	??3@YAXPAX@Z				; operator delete
	mov	ecx, DWORD PTR [esi+92280]
	add	esp, 4
	dec	ecx
	cmp	edi, ebx
	mov	DWORD PTR [esi+92280], ecx
	jne	SHORT $L90458
$L90459:
	pop	edi
	pop	esi
	pop	ebx

; 148  : }

	ret	0
?Clear@KIpoTree@@QAEXXZ ENDP				; KIpoTree::Clear
_TEXT	ENDS
;	COMDAT ?RemoveRtoGroupWithPermanentLeaf@KIpoTree@@AAEXXZ
_TEXT	SEGMENT
?RemoveRtoGroupWithPermanentLeaf@KIpoTree@@AAEXXZ PROC NEAR ; KIpoTree::RemoveRtoGroupWithPermanentLeaf, COMDAT

; 152  : 	KIpotRuntimeObj* pRto;
; 153  : 	while(pRto = (KIpotRuntimeObj*)m_PermanentLeaf.pBrother)

	mov	eax, DWORD PTR [ecx+56]
	xor	edx, edx
	cmp	eax, edx
	je	SHORT $L88640
	push	esi
$L88639:

; 154  : 	{
; 155  : 		m_PermanentLeaf.pBrother = pRto->pBrother;

	mov	esi, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+56], esi

; 156  : 		pRto->pAheadBrother = NULL;

	mov	DWORD PTR [eax+48], edx

; 157  : 		pRto->pBrother = NULL;

	mov	DWORD PTR [eax+4], edx
	mov	eax, DWORD PTR [ecx+56]
	cmp	eax, edx
	jne	SHORT $L88639
	pop	esi
$L88640:

; 158  : 	}
; 159  : }

	ret	0
?RemoveRtoGroupWithPermanentLeaf@KIpoTree@@AAEXXZ ENDP	; KIpoTree::RemoveRtoGroupWithPermanentLeaf
_TEXT	ENDS
PUBLIC	?AddBranch@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z	; KIpoTree::AddBranch
EXTRN	?AddBranch@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z:NEAR ; KIpotBranch::AddBranch
EXTRN	?AddAObject@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z:NEAR ; KIpotBranch::AddAObject
;	COMDAT xdata$x
; File D:\TINHDUYEN\SwordOnline\SwordOnline\Sources\Core\Src\Scene\KIpoTree.cpp
xdata$x	SEGMENT
$T90527	DD	019930520H
	DD	01H
	DD	FLAT:$T90529
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T90529	DD	0ffffffffH
	DD	FLAT:$L90525
xdata$x	ENDS
;	COMDAT ?AddBranch@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z
_TEXT	SEGMENT
$T90522 = 8
__$EHRec$ = -12
_pBranchObj$ = 8
?AddBranch@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z PROC NEAR ; KIpoTree::AddBranch, COMDAT

; 172  : {

	mov	eax, DWORD PTR fs:__except_list
	push	-1
	push	$L90528
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	esi
	mov	esi, ecx
	push	edi

; 173  : 	if (m_pMainBranch)

	mov	ecx, DWORD PTR [esi]
	test	ecx, ecx
	je	SHORT $L88646

; 174  : 		m_pMainBranch->AddBranch(pBranchObj);

	mov	eax, DWORD PTR _pBranchObj$[esp+16]
	push	eax
	call	?AddBranch@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z ; KIpotBranch::AddBranch

; 179  : 	}
; 180  : }

	mov	ecx, DWORD PTR __$EHRec$[esp+20]
	mov	DWORD PTR fs:__except_list, ecx
	pop	edi
	pop	esi
	add	esp, 12					; 0000000cH
	ret	4
$L88646:

; 175  : 	else if (pBranchObj)

	mov	edi, DWORD PTR _pBranchObj$[esp+16]
	test	edi, edi
	je	SHORT $L88648

; 176  : 	{
; 177  : 		m_pMainBranch = new KIpotBranch;

	push	48					; 00000030H
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	DWORD PTR $T90522[esp+16], eax
	test	eax, eax
	mov	DWORD PTR __$EHRec$[esp+28], 0
	je	SHORT $L90523
	mov	ecx, eax
	call	??0KIpotBranch@@QAE@XZ			; KIpotBranch::KIpotBranch
	jmp	SHORT $L90524
$L90523:
	xor	eax, eax
$L90524:

; 178  : 		m_pMainBranch->AddAObject(pBranchObj);

	push	edi
	mov	ecx, eax
	mov	DWORD PTR __$EHRec$[esp+32], -1
	mov	DWORD PTR [esi], eax
	call	?AddAObject@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z ; KIpotBranch::AddAObject
$L88648:

; 179  : 	}
; 180  : }

	mov	ecx, DWORD PTR __$EHRec$[esp+20]
	pop	edi
	mov	DWORD PTR fs:__except_list, ecx
	pop	esi
	add	esp, 12					; 0000000cH
	ret	4
_TEXT	ENDS
;	COMDAT text$x
text$x	SEGMENT
$L90525:
	mov	eax, DWORD PTR $T90522[ebp-4]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
	ret	0
$L90528:
	mov	eax, OFFSET FLAT:$T90527
	jmp	___CxxFrameHandler
text$x	ENDS
?AddBranch@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z ENDP	; KIpoTree::AddBranch
PUBLIC	?Fell@KIpoTree@@QAEXXZ				; KIpoTree::Fell
EXTRN	?RemoveAllRtoLeafs@KIpotBranch@@QAEXPAUKIpotLeaf@@@Z:NEAR ; KIpotBranch::RemoveAllRtoLeafs
;	COMDAT ?Fell@KIpoTree@@QAEXXZ
_TEXT	SEGMENT
?Fell@KIpoTree@@QAEXXZ PROC NEAR			; KIpoTree::Fell, COMDAT

; 184  : {

	push	esi
	push	edi
	mov	edi, ecx

; 185  : 	if (m_pMainBranch)

	mov	ecx, DWORD PTR [edi]
	test	ecx, ecx
	je	SHORT $L88656

; 186  : 	{
; 187  : 		m_pMainBranch->RemoveAllRtoLeafs(&m_PermanentLeaf);

	lea	eax, DWORD PTR [edi+52]
	push	eax
	call	?RemoveAllRtoLeafs@KIpotBranch@@QAEXPAUKIpotLeaf@@@Z ; KIpotBranch::RemoveAllRtoLeafs

; 188  : 		delete(m_pMainBranch);

	mov	esi, DWORD PTR [edi]
	test	esi, esi
	je	SHORT $L90537
	mov	ecx, esi
	call	??1KIpotBranch@@QAE@XZ			; KIpotBranch::~KIpotBranch
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L90537:

; 189  : 		m_pMainBranch = NULL;

	mov	DWORD PTR [edi], 0
$L88656:

; 190  : 	}
; 191  : 	m_DefaultBranch.RemoveAllRtoLeafs(&m_PermanentLeaf);

	lea	ecx, DWORD PTR [edi+52]
	lea	esi, DWORD PTR [edi+4]
	push	ecx
	mov	ecx, esi
	call	?RemoveAllRtoLeafs@KIpotBranch@@QAEXPAUKIpotLeaf@@@Z ; KIpotBranch::RemoveAllRtoLeafs

; 192  : 	m_DefaultBranch.Clear();

	mov	ecx, esi
	call	?Clear@KIpotBranch@@QAEXXZ		; KIpotBranch::Clear

; 193  : 
; 194  : 	//清除所有内建对象的光源
; 195  : 	list<KLightBase*>::iterator i;
; 196  : 	for (i = m_LightList.begin(); i != m_LightList.end(); )

	mov	eax, DWORD PTR [edi+92276]
	mov	esi, DWORD PTR [eax]
	cmp	esi, eax
	je	SHORT $L88665
	push	ebx
$L88664:

; 197  : 	{
; 198  : 		if((*i)->m_pParent == NULL)

	mov	eax, DWORD PTR [esi+8]
	mov	ecx, DWORD PTR [eax+4]
	test	ecx, ecx
	jne	SHORT $L88666

; 199  : 		{
; 200  : 			delete (*i);

	push	eax
	call	??3@YAXPAX@Z				; operator delete

; 201  : 			i = m_LightList.erase(i);

	mov	edx, DWORD PTR [esi+4]
	mov	ebx, DWORD PTR [esi]
	push	esi
	mov	DWORD PTR [edx], ebx
	mov	eax, DWORD PTR [esi]
	mov	ecx, DWORD PTR [esi+4]
	mov	DWORD PTR [eax+4], ecx
	call	??3@YAXPAX@Z				; operator delete
	mov	eax, DWORD PTR [edi+92280]
	add	esp, 8
	dec	eax
	mov	esi, ebx
	mov	DWORD PTR [edi+92280], eax

; 202  : 			continue;

	jmp	SHORT $L90721
$L88666:

; 203  : 		}
; 204  : 		++i;

	mov	esi, DWORD PTR [esi]
$L90721:
	cmp	esi, DWORD PTR [edi+92276]
	jne	SHORT $L88664
	pop	ebx
$L88665:
	pop	edi
	pop	esi

; 205  : 	}
; 206  : }

	ret	0
?Fell@KIpoTree@@QAEXXZ ENDP				; KIpoTree::Fell
_TEXT	ENDS
PUBLIC	?AddLeafLine@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z ; KIpoTree::AddLeafLine
EXTRN	?AddLeafLine@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z:NEAR ; KIpotBranch::AddLeafLine
;	COMDAT ?AddLeafLine@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z
_TEXT	SEGMENT
_pLeaf$ = 8
?AddLeafLine@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z PROC NEAR ; KIpoTree::AddLeafLine, COMDAT

; 221  : {

	mov	eax, ecx

; 222  : 	if (m_pMainBranch)

	mov	ecx, DWORD PTR [eax]
	test	ecx, ecx
	je	SHORT $L88673

; 223  : 		m_pMainBranch->AddLeafLine(pLeaf);

	mov	eax, DWORD PTR _pLeaf$[esp-4]
	push	eax

; 224  : 	else
; 225  : 		m_DefaultBranch.AddLeafLine(pLeaf);

	call	?AddLeafLine@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z ; KIpotBranch::AddLeafLine

; 226  : }

	ret	4
$L88673:

; 224  : 	else
; 225  : 		m_DefaultBranch.AddLeafLine(pLeaf);

	mov	ecx, DWORD PTR _pLeaf$[esp-4]
	push	ecx
	lea	ecx, DWORD PTR [eax+4]
	call	?AddLeafLine@KIpotBranch@@QAEXPAUKIpotBuildinObj@@@Z ; KIpotBranch::AddLeafLine

; 226  : }

	ret	4
?AddLeafLine@KIpoTree@@QAEXPAUKIpotBuildinObj@@@Z ENDP	; KIpoTree::AddLeafLine
_TEXT	ENDS
PUBLIC	?Breath@KLightBase@@UAEXXZ			; KLightBase::Breath
PUBLIC	??_7KLightBase@@6B@				; KLightBase::`vftable'
PUBLIC	?AddLeafPoint@KIpoTree@@QAEXPAUKIpotLeaf@@@Z	; KIpoTree::AddLeafPoint
PUBLIC	__real@4@3ff6b60b60b60b60b800
PUBLIC	__real@4@4006ff00000000000000
EXTRN	?IsPlayer@KNpc@@QAEHXZ:NEAR			; KNpc::IsPlayer
EXTRN	?CoreGetGameObjLightInfo@@YAXIIPAUKLightInfo@@@Z:NEAR ; CoreGetGameObjLightInfo
EXTRN	?Npc@@3PAVKNpc@@A:BYTE				; Npc
EXTRN	?AddLeafPoint@KIpotBranch@@QAEXPAUKIpotLeaf@@@Z:NEAR ; KIpotBranch::AddLeafPoint
;	COMDAT ??_7KLightBase@@6B@
; File C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\list
CONST	SEGMENT
??_7KLightBase@@6B@ DD FLAT:?Breath@KLightBase@@UAEXXZ	; KLightBase::`vftable'
CONST	ENDS
;	COMDAT __real@4@3ff6b60b60b60b60b800
CONST	SEGMENT
__real@4@3ff6b60b60b60b60b800 DD 03b360b61r	; 0.00277778
CONST	ENDS
;	COMDAT __real@4@4006ff00000000000000
CONST	SEGMENT
__real@4@4006ff00000000000000 DD 0437f0000r	; 255
CONST	ENDS
;	COMDAT ?AddLeafPoint@KIpoTree@@QAEXPAUKIpotLeaf@@@Z
_TEXT	SEGMENT
_pLeaf$ = 8
_lightInfo$88691 = -20
_n$88697 = 8
_color$88704 = -28
?AddLeafPoint@KIpoTree@@QAEXPAUKIpotLeaf@@@Z PROC NEAR	; KIpoTree::AddLeafPoint, COMDAT

; 230  : {

	sub	esp, 28					; 0000001cH
	push	ebx
	mov	ebx, ecx
	push	esi
	push	edi

; 231  : 	if (m_pMainBranch)

	mov	ecx, DWORD PTR [ebx]

; 232  : 		m_pMainBranch->AddLeafPoint(pLeaf);

	mov	edi, DWORD PTR _pLeaf$[esp+36]
	test	ecx, ecx
	push	edi

; 233  : 	else

	jne	SHORT $L90980

; 234  : 		m_DefaultBranch.AddLeafPoint(pLeaf);

	lea	ecx, DWORD PTR [ebx+4]
$L90980:
	call	?AddLeafPoint@KIpotBranch@@QAEXPAUKIpotLeaf@@@Z ; KIpotBranch::AddLeafPoint

; 235  : 	// 
; 236  : 	if (pLeaf->eLeafType == KIpotLeaf::IPOTL_T_RUNTIME_OBJ)

	cmp	DWORD PTR [edi], 1
	jne	$L88706

; 237  : 	{
; 238  : 		KIpotRuntimeObj* pRtoLeaf = (KIpotRuntimeObj*)pLeaf;
; 239  : 		if (pRtoLeaf->eLayerParam & IPOT_RL_LIGHT_PROP)

	test	BYTE PTR [edi+36], 8
	je	$L88706

; 240  : 		{
; 241  : 			// 增加一个光源
; 242  : 			KLightBase *pLight = new KLightBase;

	push	28					; 0000001cH
	call	??2@YAPAXI@Z				; operator new
	mov	esi, eax
	add	esp, 4
	test	esi, esi
	je	$L88706
	mov	DWORD PTR [esi], OFFSET FLAT:??_7KLightBase@@6B@ ; KLightBase::`vftable'

; 243  : 			if(pLight)
; 244  : 			{
; 245  : 				KLightInfo lightInfo;
; 246  : 				CoreGetGameObjLightInfo(pRtoLeaf->uGenre, pRtoLeaf->nId, &lightInfo);

	mov	ecx, DWORD PTR [edi+28]
	mov	edx, DWORD PTR [edi+24]
	lea	eax, DWORD PTR _lightInfo$88691[esp+40]
	push	ebp
	push	eax
	push	ecx
	push	edx
	call	?CoreGetGameObjLightInfo@@YAXIIPAUKLightInfo@@@Z ; CoreGetGameObjLightInfo

; 247  : 				pLight->m_dwColor = lightInfo.dwColor;

	mov	eax, DWORD PTR _lightInfo$88691[esp+68]

; 248  : 				pLight->m_oPosition = lightInfo.oPosition;

	mov	edx, DWORD PTR _lightInfo$88691[esp+56]
	lea	ecx, DWORD PTR [esi+8]
	mov	DWORD PTR [esi+20], eax
	mov	eax, DWORD PTR _lightInfo$88691[esp+60]

; 249  : 				pLight->m_nRadius = lightInfo.nRadius;
; 250  : 				pLight->m_pParent = pLeaf;

	mov	DWORD PTR [esi+4], edi
	mov	DWORD PTR [ecx], edx
	mov	edx, DWORD PTR _lightInfo$88691[esp+64]
	add	esp, 12					; 0000000cH

; 251  : 				
; 252  : 				if (pRtoLeaf->uGenre == CGOG_NPC && Npc[pRtoLeaf->nId].IsPlayer())

	mov	ebp, 320				; 00000140H
	mov	DWORD PTR [ecx+4], eax
	mov	eax, DWORD PTR _lightInfo$88691[esp+60]
	mov	DWORD PTR [esi+24], eax
	mov	DWORD PTR [ecx+8], edx
	mov	eax, DWORD PTR [edi+24]
	cmp	eax, 6
	jne	$L88696
	mov	edi, DWORD PTR [edi+28]
	lea	ecx, DWORD PTR [edi+edi*2]
	lea	eax, DWORD PTR [edi+ecx*4]
	shl	eax, 5
	add	eax, edi
	lea	ecx, DWORD PTR [eax+eax*2]
	shl	ecx, 5
	add	ecx, OFFSET FLAT:?Npc@@3PAVKNpc@@A	; Npc
	call	?IsPlayer@KNpc@@QAEHXZ			; KNpc::IsPlayer
	test	eax, eax
	je	$L88696

; 253  : 				{
; 254  : 					pLight->m_dwColor = 0xffffffff;

	mov	DWORD PTR [esi+20], -1

; 255  : 					if(m_bIsIndoor)

	mov	al, BYTE PTR [ebx+88]
	test	al, al
	je	SHORT $L88693

; 256  : 					{
; 257  : 						if(m_dwAmbient == 0xff404040)

	mov	edx, DWORD PTR [ebx+92]
	sub	edx, -12566464				; ff404040H
	neg	edx
	sbb	edx, edx
	and	edx, ebp
	mov	DWORD PTR [esi+24], edx

; 258  : 							pLight->m_nRadius = 0;
; 259  : 						else
; 260  : 							pLight->m_nRadius = MAIN_PLAYER_LIGHT_RADIUS;
; 261  : 					}
; 262  : 					else

	jmp	$L88696
$L88693:

; 263  : 					{
; 264  : 						// 是主角，按时间调整光源亮度
; 265  : 						int n = 0;
; 266  : 						if(m_nCurrentTime >= 480 && m_nCurrentTime <= 960)

	mov	ecx, DWORD PTR [ebx+84]
	xor	eax, eax
	cmp	ecx, 480				; 000001e0H
	mov	DWORD PTR _n$88697[esp+40], eax
	jl	SHORT $L90982
	cmp	ecx, 960				; 000003c0H
	jg	SHORT $L88698

; 267  : 							pLight->m_nRadius = 0;

	mov	DWORD PTR [esi+24], eax

; 268  : 						else if(m_nCurrentTime < 480)

	jmp	SHORT $L88702
$L88698:
	cmp	ecx, 480				; 000001e0H
	jge	SHORT $L88700
$L90982:

; 269  : 						{
; 270  : 							n = 480 - m_nCurrentTime;

	mov	eax, 480				; 000001e0H
	sub	eax, ecx

; 271  : 							pLight->m_nRadius = MAIN_PLAYER_LIGHT_RADIUS;
; 272  : 						}
; 273  : 						else

	jmp	SHORT $L90983
$L88700:

; 274  : 						{
; 275  : 							n = m_nCurrentTime - 960;

	lea	eax, DWORD PTR [ecx-960]
$L90983:

; 276  : 							pLight->m_nRadius = MAIN_PLAYER_LIGHT_RADIUS;
; 277  : 						}
; 278  : 
; 279  : 						if(n > 360)

	cmp	eax, 360				; 00000168H
	mov	DWORD PTR _n$88697[esp+40], eax
	mov	DWORD PTR [esi+24], ebp
	jle	SHORT $L88702

; 280  : 							n = 360;

	mov	DWORD PTR _n$88697[esp+40], 360		; 00000168H
$L88702:

; 281  : 						float f = n / 360.0f;

	fild	DWORD PTR _n$88697[esp+40]
	fmul	DWORD PTR __real@4@3ff6b60b60b60b60b800

; 282  : 						f = f * f;

	fld	ST(0)
	fmulp	ST(1), ST(0)

; 283  : 						KLColor color;
; 284  : 						color.SetColor(pLight->m_dwColor);
; 285  : 						color.Scale(f);

	fmul	DWORD PTR __real@4@4006ff00000000000000
	call	__ftol
	mov	WORD PTR _color$88704[esp+44], ax
	mov	WORD PTR _color$88704[esp+46], ax
	mov	WORD PTR _color$88704[esp+48], ax

; 286  : 						pLight->m_dwColor = color.GetColor();

	mov	eax, DWORD PTR _color$88704[esp+44]
	mov	ecx, DWORD PTR _color$88704[esp+46]
	mov	edx, DWORD PTR _color$88704[esp+48]
	and	eax, 65535				; 0000ffffH
	and	ecx, 65535				; 0000ffffH
	or	eax, -256				; ffffff00H
	and	edx, 65535				; 0000ffffH
	shl	eax, 8
	or	eax, ecx
	shl	eax, 8
	or	eax, edx
	mov	DWORD PTR [esi+20], eax
$L88696:

; 287  : 					}
; 288  : 				}
; 289  : 
; 290  : 				pLight->m_nRadius = MAIN_PLAYER_LIGHT_RADIUS;

	mov	DWORD PTR [esi+24], ebp

; 291  : 				pLight->m_dwColor = 0xff808080;

	mov	DWORD PTR [esi+20], -8355712		; ff808080H

; 292  : 
; 293  : 				if(pLight->m_nRadius)
; 294  :                 {
; 295  : 					m_LightList.push_back(pLight);

	mov	edi, DWORD PTR [ebx+92276]
	push	12					; 0000000cH
	mov	ebp, DWORD PTR [edi+4]
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	ecx, edi
	test	edi, edi
	jne	SHORT $L90940
	mov	ecx, eax
$L90940:
	test	ebp, ebp
	mov	DWORD PTR [eax], ecx
	jne	SHORT $L90942
	mov	ebp, eax
$L90942:
	mov	DWORD PTR [eax+4], ebp
	mov	DWORD PTR [edi+4], eax
	mov	ecx, DWORD PTR [eax+4]
	pop	ebp
	mov	DWORD PTR [ecx], eax
	add	eax, 8
	test	eax, eax
	je	SHORT $L90967
	mov	DWORD PTR [eax], esi
$L90967:
	inc	DWORD PTR [ebx+92280]
$L88706:
	pop	edi
	pop	esi
	pop	ebx

; 296  :                 }
; 297  :                 else
; 298  :                 {
; 299  :                     delete pLight;
; 300  :                     pLight = NULL;
; 301  :                 }
; 302  : 			}
; 303  : 		}
; 304  : 	}
; 305  : }

	add	esp, 28					; 0000001cH
	ret	4
?AddLeafPoint@KIpoTree@@QAEXPAUKIpotLeaf@@@Z ENDP	; KIpoTree::AddLeafPoint
_TEXT	ENDS
;	COMDAT ?Breath@KLightBase@@UAEXXZ
_TEXT	SEGMENT
?Breath@KLightBase@@UAEXXZ PROC NEAR			; KLightBase::Breath, COMDAT

; 50   : 	virtual void Breath(){}

	ret	0
?Breath@KLightBase@@UAEXXZ ENDP				; KLightBase::Breath
_TEXT	ENDS
PUBLIC	?PluckRto@KIpoTree@@QAEXPAUKIpotRuntimeObj@@@Z	; KIpoTree::PluckRto
EXTRN	?Pluck@KIpotRuntimeObj@@QAEXXZ:NEAR		; KIpotRuntimeObj::Pluck
;	COMDAT ?PluckRto@KIpoTree@@QAEXPAUKIpotRuntimeObj@@@Z
_TEXT	SEGMENT
_pLeaf$ = 8
?PluckRto@KIpoTree@@QAEXPAUKIpotRuntimeObj@@@Z PROC NEAR ; KIpoTree::PluckRto, COMDAT

; 308  : {

	push	ebx

; 309  : 	pLeaf->Pluck();

	mov	ebx, DWORD PTR _pLeaf$[esp]
	push	esi
	push	edi
	mov	edi, ecx
	mov	ecx, ebx
	call	?Pluck@KIpotRuntimeObj@@QAEXXZ		; KIpotRuntimeObj::Pluck

; 310  : 	list<KLightBase*>::iterator i;
; 311  : 	for (i = m_LightList.begin(); i != m_LightList.end(); ++i)

	mov	eax, DWORD PTR [edi+92276]
	mov	esi, DWORD PTR [eax]
	cmp	esi, eax
	je	SHORT $L91135
$L91030:

; 312  : 	{
; 313  : 		if((*i)->m_pParent == pLeaf)

	mov	ecx, DWORD PTR [esi+8]
	cmp	DWORD PTR [ecx+4], ebx
	je	SHORT $L91173
	mov	esi, DWORD PTR [esi]
	cmp	esi, eax
	jne	SHORT $L91030
	pop	edi
	pop	esi
	pop	ebx

; 317  : 			break;
; 318  : 		}
; 319  : 	}
; 320  : }

	ret	4
$L91173:

; 314  : 		{
; 315  : 			delete (*i);

	mov	edx, DWORD PTR [esi+8]
	push	edx
	call	??3@YAXPAX@Z				; operator delete

; 316  : 			m_LightList.erase(i);

	mov	eax, DWORD PTR [esi+4]
	mov	ecx, DWORD PTR [esi]
	push	esi
	mov	DWORD PTR [eax], ecx
	mov	edx, DWORD PTR [esi]
	mov	eax, DWORD PTR [esi+4]
	mov	DWORD PTR [edx+4], eax
	call	??3@YAXPAX@Z				; operator delete
	mov	eax, DWORD PTR [edi+92280]
	add	esp, 8
	dec	eax
	mov	DWORD PTR [edi+92280], eax
$L91135:
	pop	edi
	pop	esi
	pop	ebx

; 317  : 			break;
; 318  : 		}
; 319  : 	}
; 320  : }

	ret	4
?PluckRto@KIpoTree@@QAEXPAUKIpotRuntimeObj@@@Z ENDP	; KIpoTree::PluckRto
_TEXT	ENDS
PUBLIC	?SetPermanentBranchPos@KIpoTree@@QAEXHHH@Z	; KIpoTree::SetPermanentBranchPos
EXTRN	?SetLine@KIpotBranch@@QAEXPAUtagPOINT@@0@Z:NEAR	; KIpotBranch::SetLine
;	COMDAT ?SetPermanentBranchPos@KIpoTree@@QAEXHHH@Z
_TEXT	SEGMENT
_nLeftX$ = 8
_nRightX$ = 12
_y$ = 16
_p1$ = -8
_p2$ = -16
?SetPermanentBranchPos@KIpoTree@@QAEXHHH@Z PROC NEAR	; KIpoTree::SetPermanentBranchPos, COMDAT

; 324  : 	POINT	p1, p2;
; 325  : 	if (nLeftX < nRightX)

	mov	eax, DWORD PTR _nLeftX$[esp-4]
	mov	edx, DWORD PTR _nRightX$[esp-4]
	sub	esp, 16					; 00000010H
	cmp	eax, edx
	jge	SHORT $L91178

; 326  : 	{
; 327  : 		p1.x = nLeftX;

	mov	DWORD PTR _p1$[esp+16], eax

; 328  : 		p2.x = nRightX;
; 329  : 	}
; 330  : 	else if (nLeftX > nRightX)

	jmp	SHORT $L91179
$L91178:
	jle	SHORT $L88732

; 331  : 	{
; 332  : 		p2.x = nLeftX;

	mov	DWORD PTR _p2$[esp+16], eax

; 333  : 		p1.x = nRightX;

	mov	DWORD PTR _p1$[esp+16], edx

; 334  : 	}
; 335  : 	else

	jmp	SHORT $L88733
$L88732:

; 336  : 	{
; 337  : 		p1.x = nLeftX;

	mov	DWORD PTR _p1$[esp+16], eax

; 338  : 		p2.x = nRightX + 2048;

	add	edx, 2048				; 00000800H
$L91179:
	mov	DWORD PTR _p2$[esp+16], edx
$L88733:

; 339  : 	}
; 340  : 	p2.y = p1.y = y;

	mov	eax, DWORD PTR _y$[esp+12]

; 341  : 	m_DefaultBranch.SetLine(&p1, &p2);

	lea	edx, DWORD PTR _p1$[esp+16]
	mov	DWORD PTR _p1$[esp+20], eax
	mov	DWORD PTR _p2$[esp+20], eax
	lea	eax, DWORD PTR _p2$[esp+16]
	add	ecx, 4
	push	eax
	push	edx
	call	?SetLine@KIpotBranch@@QAEXPAUtagPOINT@@0@Z ; KIpotBranch::SetLine

; 342  : }

	add	esp, 16					; 00000010H
	ret	12					; 0000000cH
?SetPermanentBranchPos@KIpoTree@@QAEXHHH@Z ENDP		; KIpoTree::SetPermanentBranchPos
_TEXT	ENDS
PUBLIC	?StrewRtoLeafs@KIpoTree@@QAEXAAUtagRECT@@@Z	; KIpoTree::StrewRtoLeafs
PUBLIC	?ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z ; KIpoTree::ObjectsCallback
EXTRN	?EnumerateObjects@KIpotBranch@@QAEXPAXP6AX0PAUKIpotLeaf@@@Z@Z:NEAR ; KIpotBranch::EnumerateObjects
;	COMDAT ?StrewRtoLeafs@KIpoTree@@QAEXAAUtagRECT@@@Z
_TEXT	SEGMENT
_KeepRtoArea$ = 8
?StrewRtoLeafs@KIpoTree@@QAEXAAUtagRECT@@@Z PROC NEAR	; KIpoTree::StrewRtoLeafs, COMDAT

; 345  : {

	push	ebx
	push	ebp
	push	esi
	mov	esi, ecx

; 346  : 	KIpotBranch* pBranch;
; 347  : 	if (m_pMainBranch)

	xor	ebx, ebx
	mov	eax, DWORD PTR [esi]
	cmp	eax, ebx

; 348  : 		pBranch = m_pMainBranch;

	mov	ebp, eax
	jne	SHORT $L91183

; 349  : 	else
; 350  : 		pBranch = &m_DefaultBranch;

	lea	ebp, DWORD PTR [esi+4]
$L91183:

; 351  : 
; 352  : 	KIpotRuntimeObj* pRto;
; 353  : 	while(pRto = (KIpotRuntimeObj*)m_PermanentLeaf.pBrother)

	mov	eax, DWORD PTR [esi+56]
	cmp	eax, ebx
	je	SHORT $L88744
	push	edi
	mov	edi, DWORD PTR _KeepRtoArea$[esp+12]
$L88743:

; 354  : 	{
; 355  : 		m_PermanentLeaf.pBrother = pRto->pBrother;

	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [esi+56], ecx

; 356  : 		pRto->pAheadBrother = NULL;
; 357  : 		pRto->pBrother = NULL;
; 358  : 
; 359  : 		if (pRto->oPosition.x >= KeepRtoArea.left  ||
; 360  : 			pRto->oPosition.x <  KeepRtoArea.right ||
; 361  : 			pRto->oPosition.y >= KeepRtoArea.top   ||
; 362  : 			pRto->oPosition.y <  KeepRtoArea.bottom)

	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR [eax+48], ebx
	mov	DWORD PTR [eax+4], ebx
	cmp	ecx, DWORD PTR [edi]
	jge	SHORT $L88747
	cmp	ecx, DWORD PTR [edi+8]
	jl	SHORT $L88747
	mov	ecx, DWORD PTR [eax+20]
	mov	edx, DWORD PTR [edi+4]
	cmp	ecx, edx
	jge	SHORT $L88747
	cmp	ecx, DWORD PTR [edi+12]
	jl	SHORT $L88747

; 365  : 		}
; 366  : 		else
; 367  : 		{
; 368  : 			PluckRto(pRto);				

	push	eax
	mov	ecx, esi
	call	?PluckRto@KIpoTree@@QAEXPAUKIpotRuntimeObj@@@Z ; KIpoTree::PluckRto
	jmp	SHORT $L88748
$L88747:

; 363  : 		{
; 364  : 			pBranch->AddLeafPoint(pRto);

	push	eax
	mov	ecx, ebp
	call	?AddLeafPoint@KIpotBranch@@QAEXPAUKIpotLeaf@@@Z ; KIpotBranch::AddLeafPoint
$L88748:
	mov	eax, DWORD PTR [esi+56]
	cmp	eax, ebx
	jne	SHORT $L88743
	pop	edi
$L88744:

; 369  : 		}
; 370  : 	}
; 371  : 
; 372  : 	if(m_bDynamicLighting)

	cmp	BYTE PTR [esi+90], bl
	je	SHORT $L88755

; 373  : 	{
; 374  : 		// 清空遮挡信息
; 375  : 		for(int j=0; j<LIGHTING_GRID_WIDTH*LIGHTING_GRID_HEIGHT; j++)

	lea	eax, DWORD PTR [esi+96]
	mov	ecx, 4608				; 00001200H
$L88751:

; 376  : 		{
; 377  : 			pObstacle[j].nObstacle = 0;

	mov	DWORD PTR [eax], ebx
	add	eax, 16					; 00000010H
	dec	ecx
	jne	SHORT $L88751

; 378  : 		}
; 379  : 
; 380  : 		// 遍历树，计算遮挡信息
; 381  : 		if (m_pMainBranch)

	mov	ecx, DWORD PTR [esi]

; 382  : 			m_pMainBranch->EnumerateObjects(this, ObjectsCallback);

	push	OFFSET FLAT:?ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z ; KIpoTree::ObjectsCallback
	cmp	ecx, ebx
	push	esi

; 383  : 		else

	jne	SHORT $L91186

; 384  : 			m_DefaultBranch.EnumerateObjects(this, ObjectsCallback);

	lea	ecx, DWORD PTR [esi+4]
$L91186:
	call	?EnumerateObjects@KIpotBranch@@QAEXPAXP6AX0PAUKIpotLeaf@@@Z@Z ; KIpotBranch::EnumerateObjects
$L88755:
	pop	esi
	pop	ebp
	pop	ebx

; 385  : 	}
; 386  : }

	ret	4
?StrewRtoLeafs@KIpoTree@@QAEXAAUtagRECT@@@Z ENDP	; KIpoTree::StrewRtoLeafs
_TEXT	ENDS
PUBLIC	?FillLineObstacle@KIpoTree@@AAEXHHHHH@Z		; KIpoTree::FillLineObstacle
;	COMDAT ?ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z
_TEXT	SEGMENT
_p$ = 8
_pLeaf$ = 12
?ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z PROC NEAR ; KIpoTree::ObjectsCallback, COMDAT

; 391  : 	_ASSERT(p);
; 392  : 	_ASSERT(pLeaf);
; 393  : 	static int nObstacleID = 1;
; 394  : 	nObstacleID++;
; 395  : 
; 396  : 	if(pLeaf->eLeafType == KIpotLeaf::IPOTL_T_RUNTIME_OBJ)

	mov	eax, DWORD PTR _pLeaf$[esp-4]
	push	ebx
	push	ebp
	mov	ebp, DWORD PTR _?nObstacleID@?1??ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z@4HA
	inc	ebp
	push	esi
	mov	DWORD PTR _?nObstacleID@?1??ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z@4HA, ebp
	mov	ecx, DWORD PTR [eax]
	cmp	ecx, 1
	push	edi
	je	$L88779

; 397  : 		return;
; 398  : 
; 399  : 	KIpoTree *pTree = (KIpoTree*)p;
; 400  : 	KIpotBuildinObj* pObj = (KIpotBuildinObj*)pLeaf;
; 401  : 	KBuildinObj* pBio = ((KIpotBuildinObj*)pLeaf)->pBio;

	mov	ecx, DWORD PTR [eax+48]

; 402  : 	if((pBio->Props & SPBIO_P_BLOCK_LIGHT_MASK) == SPBIO_P_BLOCK_LIGHT_NONE)

	mov	ecx, DWORD PTR [ecx]
	and	ecx, 48					; 00000030H
	je	$L88779

; 403  : 		return;
; 404  : 
; 405  : 	if((pBio->Props & SPBIO_P_BLOCK_LIGHT_MASK) == SPBIO_P_BLOCK_LIGHT_BOTTOM)

	cmp	ecx, 16					; 00000010H
	jne	SHORT $L88772

; 406  : 	{
; 407  : 		// 按线段处理遮挡
; 408  : 		pTree->FillLineObstacle(pObj->oPosition.x - pTree->m_nLeftTopX, pObj->oPosition.y - pTree->m_nLeftTopY, 
; 409  : 								pObj->oEndPos.x - pTree->m_nLeftTopX, pObj->oEndPos.y - pTree->m_nLeftTopY, nObstacleID);

	mov	ecx, DWORD PTR _p$[esp+12]
	mov	edi, DWORD PTR [eax+32]
	push	ebp
	mov	edx, DWORD PTR [ecx+80]
	mov	esi, DWORD PTR [ecx+76]
	sub	edi, edx
	push	edi
	mov	edi, DWORD PTR [eax+28]
	sub	edi, esi
	push	edi
	mov	edi, DWORD PTR [eax+20]
	mov	eax, DWORD PTR [eax+16]
	sub	edi, edx
	sub	eax, esi
	push	edi
	push	eax
	call	?FillLineObstacle@KIpoTree@@AAEXHHHHH@Z	; KIpoTree::FillLineObstacle
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 428  : 		}
; 429  : 	}
; 430  : }

	ret	0
$L88772:

; 410  : 	}
; 411  : 	else if((pBio->Props & SPBIO_P_BLOCK_LIGHT_MASK) == SPBIO_P_BLOCK_LIGHT_CIRCLE)

	cmp	ecx, 32					; 00000020H
	jne	SHORT $L88779

; 412  : 	{
; 413  : 		// 按圆处理光线遮挡，忽略半径，都按一个格子处理
; 414  : 		int nX = pObj->oPosition.x;
; 415  : 		int nY = pObj->oPosition.y;
; 416  : 		int nIdx;
; 417  : 		if(nX < pTree->m_nLeftTopX || nY < pTree->m_nLeftTopY ||
; 418  : 				nX >= pTree->m_nLeftTopX + AREGION_WIDTH * 3 || nY >= pTree->m_nLeftTopY + AREGION_HEIGHT * 3)

	mov	esi, DWORD PTR _p$[esp+12]
	mov	edi, DWORD PTR [eax+16]
	mov	eax, DWORD PTR [eax+20]
	mov	ebx, DWORD PTR [esi+76]
	cmp	edi, ebx
	jl	SHORT $L88779
	mov	ecx, DWORD PTR [esi+80]
	cmp	eax, ecx
	jl	SHORT $L88779
	lea	edx, DWORD PTR [ebx+1536]
	cmp	edi, edx
	jge	SHORT $L88779
	lea	edx, DWORD PTR [ecx+3072]
	cmp	eax, edx
	jge	SHORT $L88779

; 419  : 			return;
; 420  : 		else
; 421  : 		{
; 422  : 			nX = (nX - pTree->m_nLeftTopX) / LIGHTING_GRID_SIZEX;
; 423  : 			nY = (nY - pTree->m_nLeftTopY) / LIGHTING_GRID_SIZEY;

	sub	eax, ecx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5

; 424  : 			nIdx = nY * LIGHTING_GRID_WIDTH + nX;

	lea	ecx, DWORD PTR [eax+eax*2]
	mov	eax, edi
	sub	eax, ebx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	shl	ecx, 4
	sar	eax, 5
	add	ecx, eax

; 425  : 			pTree->pObstacle[nIdx].nObstacle = nObstacleID;

	lea	eax, DWORD PTR [ecx+6]
	shl	eax, 4

; 426  : 			// 将方向的x分量设为0，表示不考虑方向
; 427  : 			pTree->pObstacle[nIdx].vDir.fX = 0.0f;

	shl	ecx, 4
	mov	DWORD PTR [eax+esi], ebp
	mov	DWORD PTR [ecx+esi+100], 0
$L88779:
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 428  : 		}
; 429  : 	}
; 430  : }

	ret	0
?ObjectsCallback@KIpoTree@@CAXPAXPAUKIpotLeaf@@@Z ENDP	; KIpoTree::ObjectsCallback
_TEXT	ENDS
PUBLIC	__real@4@40048000000000000000
PUBLIC	__real@4@c0048000000000000000
;	COMDAT __real@4@40048000000000000000
; File D:\TINHDUYEN\SwordOnline\SwordOnline\Sources\Core\Src\Scene\KIpoTree.cpp
CONST	SEGMENT
__real@4@40048000000000000000 DD 042000000r	; 32
CONST	ENDS
;	COMDAT __real@4@c0048000000000000000
CONST	SEGMENT
__real@4@c0048000000000000000 DD 0c2000000r	; -32
CONST	ENDS
;	COMDAT ?FillLineObstacle@KIpoTree@@AAEXHHHHH@Z
_TEXT	SEGMENT
_x1$ = 8
_y1$ = 12
_x2$ = 16
_y2$ = 20
_nObstacleID$ = 24
_fy1$ = 12
_fdx$ = 8
_fdy$ = -36
_gStepX$ = 16
_gStepY$ = 12
_gx1$ = -28
_gy1$ = -20
_gx2$ = -16
_gy2$ = -12
_vDir$ = -8
_fFlope1$ = -24
?FillLineObstacle@KIpoTree@@AAEXHHHHH@Z PROC NEAR	; KIpoTree::FillLineObstacle, COMDAT

; 433  : {

	sub	esp, 36					; 00000024H
	push	ebx

; 434  : 	if(x1 >= x2)

	mov	ebx, DWORD PTR _x2$[esp+36]
	push	ebp
	mov	ebp, DWORD PTR _x1$[esp+40]
	push	esi
	cmp	ebp, ebx
	push	edi
	mov	esi, ecx
	jge	$L88857

; 435  : 		return;
; 436  : 
; 437  : 	float fx1, fy1, fx2, fy2, fdx, fdy, fStepX, fStepY;
; 438  : 	int gStepX, gStepY, gx, gy, nIdx, gx1, gy1, gx2, gy2;
; 439  : 	fx1 = (float)x1, fy1 = (float)y1, fx2 = (float)x2, fy2 = (float)y2;

	fild	DWORD PTR _x1$[esp+48]
	fild	DWORD PTR _y1$[esp+48]

; 440  : 	fdx = (float)fabs(fx2 - fx1);

	fild	DWORD PTR _x2$[esp+48]
	fsub	ST(0), ST(2)
	fabs
	fstp	DWORD PTR _fdx$[esp+48]

; 441  : 	fdy = (float)fabs(fy2 - fy1);

	fild	DWORD PTR _y2$[esp+48]

; 442  : 	KVector2 vDir;
; 443  : 	vDir.fX = fdx;

	mov	eax, DWORD PTR _fdx$[esp+48]
	mov	DWORD PTR _vDir$[esp+52], eax

; 444  : 	vDir.fY = fdy;
; 445  : 	float fFlope1, fFlope2;
; 446  : 	gx1 = x1 / LIGHTING_GRID_SIZEX;

	mov	eax, ebp
	fsub	ST(0), ST(1)
	cdq
	fabs
	fstp	DWORD PTR _fdy$[esp+52]

; 447  : 	gy1 = y1 / LIGHTING_GRID_SIZEX;
; 448  : 	gx2 = x2 / LIGHTING_GRID_SIZEX;
; 449  : 	gy2 = y2 / LIGHTING_GRID_SIZEX;
; 450  : 
; 451  : 	fFlope1 = fdy / fdx;

	fld	DWORD PTR _fdy$[esp+52]
	fdiv	DWORD PTR _fdx$[esp+48]
	and	edx, 31					; 0000001fH
	mov	ecx, DWORD PTR _fdy$[esp+52]
	add	eax, edx
	mov	DWORD PTR _vDir$[esp+56], ecx
	mov	edi, eax
	mov	eax, DWORD PTR _y1$[esp+48]
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	ecx, eax
	mov	eax, ebx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	ebx, eax
	mov	eax, DWORD PTR _y2$[esp+48]
	cdq
	and	edx, 31					; 0000001fH
	add	edx, eax
	sar	edi, 5
	sar	ecx, 5
	sar	ebx, 5
	sar	edx, 5
	mov	DWORD PTR _gx1$[esp+52], edi
	mov	DWORD PTR _gy1$[esp+52], ecx
	mov	DWORD PTR _gx2$[esp+52], ebx
	mov	DWORD PTR _gy2$[esp+52], edx
	fstp	DWORD PTR -32+[esp+52]

; 452  : 
; 453  : 	// 斜率小于45度的情况
; 454  : 	if(fdx >= fdy)

	fld	DWORD PTR _fdx$[esp+48]
	fcomp	DWORD PTR _fdy$[esp+52]
	mov	eax, DWORD PTR -32+[esp+52]
	mov	DWORD PTR _fFlope1$[esp+52], eax
	fnstsw	ax
	test	ah, 1
	jne	$L88816

; 455  : 	{
; 456  : 		if(gx1 == gx2)

	cmp	edi, ebx
	je	$L91205

; 457  : 			return;
; 458  : 
; 459  : 		if(x2 > x1)

	mov	edx, DWORD PTR _x2$[esp+48]
	or	eax, -1
	cmp	edx, ebp
	mov	ecx, 1
	jle	SHORT $L88818

; 460  : 		{
; 461  : 			gStepX = 1;
; 462  : 			fStepX = LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@40048000000000000000
	mov	DWORD PTR _gStepX$[esp+48], ecx

; 463  : 		}
; 464  : 		else

	jmp	SHORT $L88819
$L88818:

; 465  : 		{
; 466  : 			gStepX = -1;
; 467  : 			fStepX = -LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@c0048000000000000000
	mov	DWORD PTR _gStepX$[esp+48], eax
$L88819:

; 468  : 		}
; 469  : 		if(y2 > y1)

	mov	edx, DWORD PTR _y1$[esp+48]
	mov	edi, DWORD PTR _y2$[esp+48]
	cmp	edi, edx
	jle	SHORT $L88820

; 470  : 		{
; 471  : 			gStepY = 1;
; 472  : 			fStepY = fdy / fdx * LIGHTING_GRID_SIZEX;

	fld	DWORD PTR -32+[esp+52]
	fmul	DWORD PTR __real@4@40048000000000000000
	mov	DWORD PTR _gStepY$[esp+48], ecx

; 473  : 		}
; 474  : 		else

	jmp	SHORT $L91203
$L88820:

; 475  : 		{
; 476  : 			gStepY = -1;
; 477  : 			fStepY = -fdy / fdx * LIGHTING_GRID_SIZEX;

	fld	DWORD PTR -32+[esp+52]
	fmul	DWORD PTR __real@4@c0048000000000000000
	mov	DWORD PTR _gStepY$[esp+48], eax
$L91203:
	mov	ebx, DWORD PTR _nObstacleID$[esp+48]
	mov	ebp, DWORD PTR _vDir$[esp+52]
$L88823:

; 478  : 		}
; 479  : 		while(1)
; 480  : 		{
; 481  : 			int nx = (int)fx1;
; 482  : 			int ny = (int)fy1;
; 483  : 
; 484  : 			gx = nx / LIGHTING_GRID_SIZEX;

	fld	ST(3)
	call	__ftol
	cdq

; 485  : 			gy = ny / LIGHTING_GRID_SIZEX;

	fld	ST(2)
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edi, eax
	sar	edi, 5
	call	__ftol
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5

; 486  : 
; 487  : 			if(gx >0 && gx < LIGHTING_GRID_WIDTH - 1 && gy >0 && gy < LIGHTING_GRID_HEIGHT - 1)

	test	edi, edi
	jle	$L88832
	cmp	edi, 47					; 0000002fH
	jge	$L88832
	test	eax, eax
	jle	$L88832
	cmp	eax, 95					; 0000005fH
	jge	$L88832

; 488  : 			{
; 489  : 				nIdx = gy * LIGHTING_GRID_WIDTH + gx;

	lea	ecx, DWORD PTR [eax+eax*2]
	shl	ecx, 4
	add	ecx, edi

; 490  : 				pObstacle[nIdx].nObstacle = nObstacleID;

	lea	edx, DWORD PTR [ecx+6]
	shl	edx, 4
	mov	DWORD PTR [edx+esi], ebx

; 491  : 				pObstacle[nIdx].vDir = vDir;

	mov	edx, ecx
	shl	edx, 4
	mov	DWORD PTR [edx+esi+100], ebp
	mov	ebp, DWORD PTR _vDir$[esp+56]
	mov	DWORD PTR [edx+esi+104], ebp

; 492  : 
; 493  : 				fFlope2 = (float)(fabs((gy + gStepY) * 32.0f - fy1) / fabs((gx + gStepX) * 32.0f - fx1));

	mov	edx, DWORD PTR _gStepY$[esp+48]
	add	edx, eax
	mov	ebp, DWORD PTR _gStepX$[esp+48]
	mov	DWORD PTR 20+[esp+48], edx

; 494  : 				if(fFlope2 > fFlope1)

	fild	DWORD PTR 20+[esp+48]
	lea	eax, DWORD PTR [edi+ebp]
	mov	DWORD PTR 20+[esp+48], eax
	fmul	DWORD PTR __real@4@40048000000000000000
	fsub	ST(0), ST(3)
	fabs
	fild	DWORD PTR 20+[esp+48]
	fmul	DWORD PTR __real@4@40048000000000000000
	fsub	ST(0), ST(5)
	fabs
	fdivp	ST(1), ST(0)
	fcomp	DWORD PTR _fFlope1$[esp+52]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L88831

; 495  : 				{
; 496  : 					nIdx = (gy + gStepY) * LIGHTING_GRID_WIDTH + gx;

	lea	eax, DWORD PTR [edx+edx*2]
	shl	eax, 4
	add	eax, edi

; 497  : 					pObstacle[nIdx].nObstacle = nObstacleID;
; 498  : 					pObstacle[nIdx].vDir = vDir;
; 499  : 				}
; 500  : 				else

	jmp	SHORT $L91209
$L88831:

; 501  : 				{
; 502  : 					nIdx = gy * LIGHTING_GRID_WIDTH + gx + gStepX;

	lea	eax, DWORD PTR [ecx+ebp]
$L91209:

; 503  : 					pObstacle[nIdx].nObstacle = nObstacleID;
; 504  : 					pObstacle[nIdx].vDir = vDir;

	mov	ebp, DWORD PTR _vDir$[esp+52]
	mov	edx, DWORD PTR _vDir$[esp+56]
	lea	ecx, DWORD PTR [eax+6]
	shl	ecx, 4
	shl	eax, 4
	mov	DWORD PTR [ecx+esi], ebx
	mov	DWORD PTR [eax+esi+100], ebp
	mov	DWORD PTR [eax+esi+104], edx
$L88832:

; 510  : 			gx1 += gStepX;

	mov	eax, DWORD PTR _gx1$[esp+52]
	mov	edx, DWORD PTR _gStepX$[esp+48]

; 511  : 			if(gx1 == gx2)

	mov	ecx, DWORD PTR _gx2$[esp+52]
	add	eax, edx
	fld	ST(1)
	faddp	ST(4), ST(0)
	cmp	eax, ecx
	mov	DWORD PTR _gx1$[esp+52], eax
	fld	ST(0)
	faddp	ST(3), ST(0)
	jne	$L88823

; 515  : 				if(gx >0 && gx < LIGHTING_GRID_WIDTH - 1 && gy >0 && gy < LIGHTING_GRID_HEIGHT - 1)
; 516  : 				{
; 517  : 					nIdx = gy * LIGHTING_GRID_WIDTH + gx;
; 518  : 					pObstacle[nIdx].nObstacle = nObstacleID;
; 519  : 					pObstacle[nIdx].vDir = vDir;
; 520  : 				}
; 521  : 				return;

	jmp	$L91211
$L88816:

; 522  : 			}
; 523  : 		}
; 524  : 	}
; 525  : 	// 斜率大于45度的情况
; 526  : 	else
; 527  : 	{
; 528  : 		if(gy1 == gy2)

	cmp	ecx, edx
	je	$L91205

; 529  : 			return;
; 530  : 
; 531  : 		if(y2 > y1)

	mov	edx, DWORD PTR _y1$[esp+48]
	mov	edi, DWORD PTR _y2$[esp+48]
	or	eax, -1
	cmp	edi, edx
	mov	ecx, 1
	jle	SHORT $L88839

; 532  : 		{
; 533  : 			gStepY = 1;
; 534  : 			fStepY = LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@40048000000000000000
	mov	DWORD PTR _gStepY$[esp+48], ecx

; 535  : 		}
; 536  : 		else

	jmp	SHORT $L88840
$L88839:

; 537  : 		{
; 538  : 			gStepY = -1;
; 539  : 			fStepY = -LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@c0048000000000000000
	mov	DWORD PTR _gStepY$[esp+48], eax
$L88840:

; 540  : 		}
; 541  : 		if(x2 > x1)

	cmp	DWORD PTR _x2$[esp+48], ebp
	jle	SHORT $L88841

; 542  : 		{
; 543  : 			gStepX = 1;
; 544  : 			fStepX = fdx / fdy * LIGHTING_GRID_SIZEX;

	fld	DWORD PTR _fdx$[esp+48]
	fdiv	DWORD PTR _fdy$[esp+52]
	mov	DWORD PTR _gStepX$[esp+48], ecx
	fmul	DWORD PTR __real@4@40048000000000000000

; 545  : 		}
; 546  : 		else

	jmp	SHORT $L91204
$L88841:

; 547  : 		{
; 548  : 			gStepX = -1;
; 549  : 			fStepX = -fdx / fdy * LIGHTING_GRID_SIZEX;

	fld	DWORD PTR _fdx$[esp+48]
	fdiv	DWORD PTR _fdy$[esp+52]
	mov	DWORD PTR _gStepX$[esp+48], eax
	fmul	DWORD PTR __real@4@c0048000000000000000
$L91204:
	mov	ebx, DWORD PTR _nObstacleID$[esp+48]
	mov	ebp, DWORD PTR _vDir$[esp+52]
$L88844:

; 550  : 		}
; 551  : 		while(1)
; 552  : 		{
; 553  : 			int nx = (int)fx1;
; 554  : 			int ny = (int)fy1;
; 555  : 
; 556  : 			gx = nx / LIGHTING_GRID_SIZEX;

	fld	ST(3)
	call	__ftol
	cdq

; 557  : 			gy = ny / LIGHTING_GRID_SIZEX;

	fld	ST(2)
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edi, eax
	sar	edi, 5
	call	__ftol
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5

; 558  : 
; 559  : 			if(gx >0 && gx < LIGHTING_GRID_WIDTH - 1 && gy >0 && gy < LIGHTING_GRID_HEIGHT - 1)

	test	edi, edi
	jle	$L88853
	cmp	edi, 47					; 0000002fH
	jge	$L88853
	test	eax, eax
	jle	$L88853
	cmp	eax, 95					; 0000005fH
	jge	$L88853

; 560  : 			{
; 561  : 				nIdx = gy * LIGHTING_GRID_WIDTH + gx;

	lea	ecx, DWORD PTR [eax+eax*2]
	shl	ecx, 4
	add	ecx, edi

; 562  : 				pObstacle[nIdx].nObstacle = nObstacleID;

	lea	edx, DWORD PTR [ecx+6]
	shl	edx, 4
	mov	DWORD PTR [edx+esi], ebx

; 563  : 				pObstacle[nIdx].vDir = vDir;

	mov	edx, ecx
	shl	edx, 4
	mov	DWORD PTR [edx+esi+100], ebp
	mov	ebp, DWORD PTR _vDir$[esp+56]
	mov	DWORD PTR [edx+esi+104], ebp

; 564  : 
; 565  : 				fFlope2 = (float)(fabs((gy + gStepY) * 32.0f - fy1) / fabs((gx + gStepX) * 32.0f - fx1));

	mov	edx, DWORD PTR _gStepY$[esp+48]
	add	edx, eax
	mov	ebp, DWORD PTR _gStepX$[esp+48]
	mov	DWORD PTR 20+[esp+48], edx

; 566  : 				if(fFlope2 > fFlope1)

	fild	DWORD PTR 20+[esp+48]
	lea	eax, DWORD PTR [edi+ebp]
	mov	DWORD PTR 20+[esp+48], eax
	fmul	DWORD PTR __real@4@40048000000000000000
	fsub	ST(0), ST(3)
	fabs
	fild	DWORD PTR 20+[esp+48]
	fmul	DWORD PTR __real@4@40048000000000000000
	fsub	ST(0), ST(5)
	fabs
	fdivp	ST(1), ST(0)
	fcomp	DWORD PTR _fFlope1$[esp+52]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L88852

; 567  : 				{
; 568  : 					nIdx = (gy + gStepY) * LIGHTING_GRID_WIDTH + gx;

	lea	eax, DWORD PTR [edx+edx*2]
	shl	eax, 4
	add	eax, edi

; 569  : 					pObstacle[nIdx].nObstacle = nObstacleID;
; 570  : 					pObstacle[nIdx].vDir = vDir;
; 571  : 				}
; 572  : 				else

	jmp	SHORT $L91210
$L88852:

; 573  : 				{
; 574  : 					nIdx = gy * LIGHTING_GRID_WIDTH + gx + gStepX;

	lea	eax, DWORD PTR [ecx+ebp]
$L91210:

; 575  : 					pObstacle[nIdx].nObstacle = nObstacleID;
; 576  : 					pObstacle[nIdx].vDir = vDir;

	mov	ebp, DWORD PTR _vDir$[esp+52]
	mov	edx, DWORD PTR _vDir$[esp+56]
	lea	ecx, DWORD PTR [eax+6]
	shl	ecx, 4
	shl	eax, 4
	mov	DWORD PTR [ecx+esi], ebx
	mov	DWORD PTR [eax+esi+100], ebp
	mov	DWORD PTR [eax+esi+104], edx
$L88853:

; 582  : 			gy1 += gStepY;

	mov	eax, DWORD PTR _gy1$[esp+52]
	mov	edx, DWORD PTR _gStepY$[esp+48]

; 583  : 			if(gy1 == gy2)

	mov	ecx, DWORD PTR _gy2$[esp+52]
	add	eax, edx
	fld	ST(0)
	faddp	ST(4), ST(0)
	fld	ST(1)
	cmp	eax, ecx
	mov	DWORD PTR _gy1$[esp+52], eax
	faddp	ST(3), ST(0)
	jne	$L88844
$L91211:

; 577  : 				}
; 578  : 			}
; 579  : 
; 580  : 			fx1 += fStepX;
; 581  : 			fy1 += fStepY;

	fstp	ST(0)
	fstp	ST(0)
	fstp	DWORD PTR _fy1$[esp+48]

; 584  : 			{
; 585  : 				gx = ((int)fx1) / LIGHTING_GRID_SIZEX;

	call	__ftol
	cdq

; 586  : 				gy = ((int)fy1) / LIGHTING_GRID_SIZEX;

	fld	DWORD PTR _fy1$[esp+48]
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edi, eax
	sar	edi, 5
	call	__ftol
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5

; 587  : 				if(gx >0 && gx < LIGHTING_GRID_WIDTH - 1 && gy >0 && gy < LIGHTING_GRID_HEIGHT - 1)

	test	edi, edi
	jle	SHORT $L88857
	cmp	edi, 47					; 0000002fH
	jge	SHORT $L88857
	test	eax, eax
	jle	SHORT $L88857
	cmp	eax, 95					; 0000005fH
	jge	SHORT $L88857

; 588  : 				{
; 589  : 					nIdx = gy * LIGHTING_GRID_WIDTH + gx;

	lea	eax, DWORD PTR [eax+eax*2]

; 590  : 					pObstacle[nIdx].nObstacle = nObstacleID;
; 591  : 					pObstacle[nIdx].vDir = vDir;

	mov	edx, DWORD PTR _vDir$[esp+56]
	shl	eax, 4
	add	eax, edi
	pop	edi
	lea	ecx, DWORD PTR [eax+6]
	shl	ecx, 4
	shl	eax, 4
	mov	DWORD PTR [ecx+esi], ebx
	mov	DWORD PTR [eax+esi+100], ebp
	mov	DWORD PTR [eax+esi+104], edx
	pop	esi
	pop	ebp
	pop	ebx

; 592  : 				}
; 593  : 				return;
; 594  : 			}
; 595  : 		}
; 596  : 	}
; 597  : }

	add	esp, 36					; 00000024H
	ret	20					; 00000014H
$L91205:

; 505  : 				}
; 506  : 			}
; 507  : 
; 508  : 			fx1 += fStepX;
; 509  : 			fy1 += fStepY;

	fstp	ST(0)

; 512  : 			{
; 513  : 				gx = ((int)fx1) / LIGHTING_GRID_SIZEX;
; 514  : 				gy = ((int)fy1) / LIGHTING_GRID_SIZEX;

	fstp	ST(0)
$L88857:
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 592  : 				}
; 593  : 				return;
; 594  : 			}
; 595  : 		}
; 596  : 	}
; 597  : }

	add	esp, 36					; 00000024H
	ret	20					; 00000014H
?FillLineObstacle@KIpoTree@@AAEXHHHHH@Z ENDP		; KIpoTree::FillLineObstacle
_TEXT	ENDS
PUBLIC	__real@8@3fff8000000000000000
PUBLIC	__real@8@400f8000000000000000
PUBLIC	?CanLighting@KIpoTree@@AAE_NHHHH@Z		; KIpoTree::CanLighting
;	COMDAT __real@8@3fff8000000000000000
; File C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\list
CONST	SEGMENT
__real@8@3fff8000000000000000 DQ 03ff0000000000000r ; 1
CONST	ENDS
;	COMDAT __real@8@400f8000000000000000
CONST	SEGMENT
__real@8@400f8000000000000000 DQ 040f0000000000000r ; 65536
CONST	ENDS
;	COMDAT ?RenderLightMap@KIpoTree@@QAEXXZ
_TEXT	SEGMENT
_this$ = -92
_i$ = -24
_nLightPosX$88879 = -32
_nLightPosY$88880 = -36
_ur$88887 = -52
_ug$88888 = -48
_ub$88889 = -44
_j$88890 = -96
_y$88892 = -20
_gx$88893 = -68
_gy$88894 = -68
_fLightRadiusDenom$88896 = -56
_pdwLight$ = -32
?RenderLightMap@KIpoTree@@QAEXXZ PROC NEAR		; KIpoTree::RenderLightMap, COMDAT

; 934  : {

	sub	esp, 96					; 00000060H
	push	ebx
	mov	ebx, ecx
	push	ebp
	push	esi

; 935  : 	int nn = m_LightList.size();
; 936  : 	int j, nLightID=1;
; 937  : 
; 938  : 	m_dwAmbient = 0xff101010;
; 939  : 
; 940  : 	// 按环境光清空光照图
; 941  : 	DWORD dwR, dwG, dwB;
; 942  : 	dwR = (m_dwAmbient & 0x00ff0000) >>16;
; 943  : 	dwG = (m_dwAmbient & 0x0000ff00) >>8;
; 944  : 	dwB = m_dwAmbient & 0x000000ff;
; 945  :     KLColor *pTempColor = pLColor;

	mov	eax, DWORD PTR [ebx+73824]
	push	edi
	mov	DWORD PTR _this$[esp+112], ebx
	mov	DWORD PTR [ebx+92], -15724528		; ff101010H
	mov	edx, 1152				; 00000480H
$L88868:

; 946  :     
; 947  :     // 如果每一个颜色分量是2个字节,可以考虑多一个项,然后用一个MMX的寄存器存放
; 948  :     // 正好8个字节
; 949  : 	for(j=0; j < LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT / 4; j++)
; 950  : 	{
; 951  : 			pTempColor[0].r = dwR;

	mov	ecx, 16					; 00000010H
	mov	WORD PTR [eax], cx

; 952  : 			pTempColor[1].r = dwR;

	mov	WORD PTR [eax+8], cx

; 953  : 			pTempColor[2].r = dwR;

	mov	WORD PTR [eax+16], cx

; 954  : 			pTempColor[3].r = dwR;

	mov	WORD PTR [eax+24], cx

; 955  : 
; 956  : 			pTempColor[0].g = dwG;

	mov	WORD PTR [eax+2], cx

; 957  : 			pTempColor[1].g = dwG;

	mov	WORD PTR [eax+10], cx

; 958  : 			pTempColor[2].g = dwG;

	mov	WORD PTR [eax+18], cx

; 959  : 			pTempColor[3].g = dwG;

	mov	WORD PTR [eax+26], cx

; 960  : 
; 961  : 
; 962  : 			pTempColor[0].b = dwB;

	mov	WORD PTR [eax+4], cx

; 963  : 			pTempColor[1].b = dwB;

	mov	WORD PTR [eax+12], cx

; 964  : 			pTempColor[2].b = dwB;

	mov	WORD PTR [eax+20], cx

; 965  : 			pTempColor[3].b = dwB;

	mov	WORD PTR [eax+28], cx

; 966  :             
; 967  :             pTempColor += 4;

	add	eax, 32					; 00000020H
	dec	edx
	jne	SHORT $L88868

; 968  : 	}
; 969  : 
; 970  : 	list<KLightBase*>::iterator i;
; 971  : 	// 计算每一个光源的光照结果
; 972  : 	for (i = m_LightList.begin(); i != m_LightList.end(); ++i)

	mov	eax, DWORD PTR [ebx+92276]
	mov	esi, DWORD PTR [eax]
	cmp	esi, eax
	mov	DWORD PTR _i$[esp+112], esi
	je	$L88877
	jmp	SHORT $L91263
$L91336:
	mov	esi, DWORD PTR _i$[esp+112]
$L91263:

; 973  : 	{
; 974  : 		nLightID++;
; 975  : 		KLightBase *pLight = *i;

	mov	ebp, DWORD PTR [esi+8]

; 976  : 		// 光源相对于九区域左上角的坐标
; 977  : 		int nLightPosX = pLight->m_oPosition.nX - m_nLeftTopX;

	mov	edi, DWORD PTR [ebx+76]

; 978  : 		int nLightPosY = pLight->m_oPosition.nY - m_nLeftTopY;

	mov	edx, DWORD PTR [ebx+80]
	mov	eax, DWORD PTR [ebp+8]
	mov	ecx, DWORD PTR [ebp+12]
	sub	eax, edi
	sub	ecx, edx
	mov	DWORD PTR _nLightPosX$88879[esp+112], eax
	mov	DWORD PTR _nLightPosY$88880[esp+112], ecx

; 979  : 		// 光源的格子坐标
; 980  : 		int nLightGridX = nLightPosX / LIGHTING_GRID_SIZEX;

	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	edi, eax

; 981  : 		int nLightGridY = nLightPosY / LIGHTING_GRID_SIZEY;

	mov	eax, ecx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	ecx, eax
	sar	edi, 5
	sar	ecx, 5

; 982  : 		if(nLightGridX < 0 || nLightGridX >= LIGHTING_GRID_WIDTH || 
; 983  : 			nLightGridY < 0 || nLightGridY >= LIGHTING_GRID_HEIGHT)

	test	edi, edi
	jl	$L88876
	cmp	edi, 48					; 00000030H
	jge	$L88876
	test	ecx, ecx
	jl	$L88876
	cmp	ecx, 96					; 00000060H
	jge	$L88876

; 984  : 			continue;
; 985  : 
; 986  : 		// 光源所在格子中心的坐标
; 987  : 		int nLightGridCenterX = nLightGridX * LIGHTING_GRID_SIZEX + LIGHTING_GRID_SIZEX / 2;
; 988  : 		int nLightGridCenterY = nLightGridY * LIGHTING_GRID_SIZEY + LIGHTING_GRID_SIZEY / 2;
; 989  : 
; 990  : 		// 获取光源的r，g，b分量
; 991  : 		unsigned int ur, ug, ub;
; 992  : 
; 993  : 		ur = (pLight->m_dwColor & 0x00ff0000) >> 18;

	mov	eax, DWORD PTR [ebp+20]

; 994  : 		ug = (pLight->m_dwColor & 0x0000ff00) >> 10;
; 995  : 		ub = (pLight->m_dwColor & 0x000000ff) >> 2;
; 996  : 
; 997  : 		//float fr = (float)pLight->m_nRadius * (float)pLight->m_nRadius;
; 998  : 
; 999  : 		int j, x, y, gx, gy;
; 1000 : 		int nGridRadius = pLight->m_nRadius / LIGHTING_GRID_SIZEX;

	mov	ebp, DWORD PTR [ebp+24]
	mov	DWORD PTR -16+[esp+112], ebp
	mov	edx, eax

; 1001 :         float fLightRadiusDenom =  1.0 / ((float)pLight->m_nRadius);

	fild	DWORD PTR -16+[esp+112]
	shr	edx, 18					; 00000012H
	and	edx, 63					; 0000003fH
	mov	esi, edi
	fdivr	QWORD PTR __real@8@3fff8000000000000000
	mov	DWORD PTR _ur$88887[esp+112], edx
	mov	edx, eax
	shr	eax, 2
	shr	edx, 10					; 0000000aH
	and	eax, 63					; 0000003fH
	and	edx, 63					; 0000003fH
	mov	DWORD PTR _ub$88889[esp+112], eax
	mov	eax, ebp
	mov	DWORD PTR _ug$88888[esp+112], edx
	mov	ebx, ecx
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	shl	esi, 5
	shl	ebx, 5
	sar	eax, 5
	add	esi, 16					; 00000010H
	add	ebx, 16					; 00000010H

; 1002 :         //fLightRadiusDenom *= fLightRadiusDenom;
; 1003 :         unsigned int uDistance65536 = 0;
; 1004 :         KLColor *pCurPos = NULL;
; 1005 : 
; 1006 : 		
; 1007 : 		gy = nLightGridY - (nGridRadius - 1);
; 1008 : 		// 从中心点向外扩展计算光照强度
; 1009 : 		for(int r = 0; r < nGridRadius; r++)

	test	eax, eax
	fstp	DWORD PTR _fLightRadiusDenom$88896[esp+112]
	jle	$L91342
	lea	ebp, DWORD PTR [ecx+ecx*2]
	or	edx, -1
	shl	ebp, 4
	mov	DWORD PTR -28+[esp+112], ebp
	lea	ebp, DWORD PTR [ecx+1]
	mov	DWORD PTR -80+[esp+112], ebp
	mov	DWORD PTR -40+[esp+112], edi
	lea	ebp, DWORD PTR [ebp+ebp*2]
	mov	DWORD PTR -88+[esp+112], edx
	shl	ebp, 4
	mov	DWORD PTR -64+[esp+112], ebp
	lea	ebp, DWORD PTR [ebx+32]
	sub	ebx, esi
	sub	ecx, edi
	mov	DWORD PTR -60+[esp+112], esi
	mov	DWORD PTR -84+[esp+112], edi
	mov	DWORD PTR -76+[esp+112], esi
	mov	DWORD PTR -72+[esp+112], ebp
	mov	DWORD PTR -8+[esp+112], ebx
	mov	DWORD PTR -12+[esp+112], ecx
	mov	DWORD PTR -16+[esp+112], eax
$L88901:

; 1010 : 		{
; 1011 : 			gx = nLightGridX - r;

	mov	eax, DWORD PTR -84+[esp+112]

; 1012 : 			gy = nLightGridY - r;
; 1013 : 			if (!
; 1014 : 				((gx < 0) || (gy < 0))
; 1015 : 			)

	test	eax, eax
	mov	DWORD PTR _gx$88893[esp+112], eax
	jl	$L91339
	mov	ecx, DWORD PTR -80+[esp+112]
	dec	ecx
	test	ecx, ecx
	jl	$L91339

; 1016 : 			{
; 1017 : 				x = nLightGridCenterX - r * LIGHTING_GRID_SIZEX;
; 1018 : 				y = nLightGridCenterY - r * LIGHTING_GRID_SIZEY;
; 1019 : 				pCurPos = &pLColor[gy * LIGHTING_GRID_WIDTH + gx];

	mov	esi, DWORD PTR _this$[esp+112]
	mov	ecx, DWORD PTR -64+[esp+112]
	mov	edi, DWORD PTR -76+[esp+112]
	add	ecx, eax
	mov	esi, DWORD PTR [esi+73824]

; 1020 : 				for(j=0; j<(r * 2 + 1); j++)

	mov	DWORD PTR _j$88890[esp+112], 0
	lea	esi, DWORD PTR [esi+ecx*8-384]
	lea	ecx, DWORD PTR [edx+2]
	test	ecx, ecx
	jle	$L91339
$L88905:

; 1021 : 				{
; 1022 : 					if(CanLighting(x, y, nLightPosX, nLightPosY))

	mov	edx, DWORD PTR -72+[esp+112]
	mov	ebp, DWORD PTR _nLightPosY$88880[esp+112]
	mov	ebx, DWORD PTR _nLightPosX$88879[esp+112]
	mov	ecx, DWORD PTR _this$[esp+112]
	push	ebp
	lea	eax, DWORD PTR [edx-32]
	push	ebx
	push	eax
	push	edi
	call	?CanLighting@KIpoTree@@AAE_NHHHH@Z	; KIpoTree::CanLighting
	test	al, al
	je	SHORT $L88913

; 1023 : 					{
; 1024 : 						//float f = 65536.0 - ((float)(
; 1025 : 						//    (
; 1026 : 						//        ((float)(x - nLightPosX)) * ((float)(x - nLightPosX)) + 
; 1027 : 						//        ((float)(y - nLightPosY)) * ((float)(y - nLightPosY))
; 1028 : 						//    ) * 
; 1029 : 						//    fLightRadiusDenom *
; 1030 : 						//    65536.0
; 1031 : 						//));
; 1032 : 						float f = 65536.0 - ((float)(
; 1033 : 							sqrt(
; 1034 : 								(x - nLightPosX) * (x - nLightPosX) + 
; 1035 : 								(y - nLightPosY) * (y - nLightPosY)
; 1036 : 							) * 
; 1037 : 							fLightRadiusDenom * 
; 1038 : 							65536.0
; 1039 : 						));

	mov	eax, DWORD PTR -72+[esp+112]
	mov	ecx, edi
	sub	ecx, ebx
	add	eax, -32				; ffffffe0H
	mov	edx, ecx
	sub	eax, ebp
	imul	edx, ecx
	mov	ecx, eax
	imul	ecx, eax
	add	edx, ecx

; 1040 : 
; 1041 : 						uDistance65536 = (int)(f);

	mov	DWORD PTR -20+[esp+112], edx
	fild	DWORD PTR -20+[esp+112]
	fsqrt
	fmul	DWORD PTR _fLightRadiusDenom$88896[esp+112]
	fmul	QWORD PTR __real@8@400f8000000000000000
	fsubr	QWORD PTR __real@8@400f8000000000000000
	call	__ftol

; 1042 : 
; 1043 : 						if (((int)uDistance65536) > 0)

	test	eax, eax
	jle	SHORT $L88913

; 1044 : 						{
; 1045 : 							pCurPos->r += (ur * uDistance65536 / 65536);

	mov	edx, eax

; 1046 : 							pCurPos->g += (ug * uDistance65536 / 65536);

	mov	ecx, eax
	imul	edx, DWORD PTR _ur$88887[esp+112]
	imul	ecx, DWORD PTR _ug$88888[esp+112]

; 1047 : 							pCurPos->b += (ub * uDistance65536 / 65536);

	imul	eax, DWORD PTR _ub$88889[esp+112]
	shr	edx, 16					; 00000010H
	add	WORD PTR [esi], dx
	shr	ecx, 16					; 00000010H
	add	WORD PTR [esi+2], cx
	shr	eax, 16					; 00000010H
	add	WORD PTR [esi+4], ax
$L88913:

; 1048 : 						}
; 1049 : 					}
; 1050 : 
; 1051 : 					gx++;

	mov	eax, DWORD PTR _gx$88893[esp+112]
	inc	eax

; 1052 :                     if (gx >= LIGHTING_GRID_WIDTH)

	cmp	eax, 48					; 00000030H
	mov	DWORD PTR _gx$88893[esp+112], eax
	jge	SHORT $L91331
	mov	edx, DWORD PTR -88+[esp+112]
	mov	eax, DWORD PTR _j$88890[esp+112]

; 1053 :                         break;
; 1054 : 
; 1055 : 					x += LIGHTING_GRID_SIZEX;

	add	edi, 32					; 00000020H

; 1056 : 					pCurPos++;

	add	esi, 8
	inc	eax
	lea	ecx, DWORD PTR [edx+2]
	cmp	eax, ecx
	mov	DWORD PTR _j$88890[esp+112], eax
	jl	$L88905
$L91331:

; 1020 : 				for(j=0; j<(r * 2 + 1); j++)

	mov	edx, DWORD PTR -88+[esp+112]
	mov	eax, DWORD PTR -84+[esp+112]
	jmp	SHORT $L91312
$L91339:
	mov	ebp, DWORD PTR _nLightPosY$88880[esp+112]
	mov	ebx, DWORD PTR _nLightPosX$88879[esp+112]
$L91312:

; 1057 : 				}
; 1058 : 			}
; 1059 : 
; 1060 : 			if(r == 0)

	cmp	edx, -1
	je	$L88902

; 1061 : 				continue;
; 1062 : 
; 1063 : 			gx = nLightGridX - r;
; 1064 : 			gy = nLightGridY + r;
; 1065 : 			if (!
; 1066 : 					((gx < 0) || (gy >= LIGHTING_GRID_HEIGHT))
; 1067 : 			)

	test	eax, eax
	mov	DWORD PTR _gx$88893[esp+112], eax
	jl	$L91313
	mov	ecx, DWORD PTR -12+[esp+112]
	mov	esi, DWORD PTR -40+[esp+112]
	add	ecx, esi
	cmp	ecx, 96					; 00000060H
	jge	$L91313

; 1068 : 			{
; 1069 : 				x = nLightGridCenterX - r * LIGHTING_GRID_SIZEX;
; 1070 : 				y = nLightGridCenterY + r * LIGHTING_GRID_SIZEY;

	mov	ecx, DWORD PTR -8+[esp+112]
	mov	esi, DWORD PTR -60+[esp+112]
	add	ecx, esi

; 1071 : 				pCurPos = &pLColor[gy * LIGHTING_GRID_WIDTH + gx];

	mov	esi, DWORD PTR _this$[esp+112]
	mov	DWORD PTR _y$88892[esp+112], ecx
	mov	ecx, DWORD PTR -28+[esp+112]
	mov	esi, DWORD PTR [esi+73824]
	mov	edi, DWORD PTR -76+[esp+112]
	add	ecx, eax

; 1072 : 				for(j=0; j<(r * 2 + 1); j++)

	mov	DWORD PTR _j$88890[esp+112], 0
	lea	esi, DWORD PTR [esi+ecx*8]
	lea	ecx, DWORD PTR [edx+2]
	test	ecx, ecx
	jle	$L91313
$L88917:

; 1073 : 				{
; 1074 : 					if(CanLighting(x, y, nLightPosX, nLightPosY))

	mov	edx, DWORD PTR _y$88892[esp+112]
	mov	ecx, DWORD PTR _this$[esp+112]
	push	ebp
	push	ebx
	push	edx
	push	edi
	call	?CanLighting@KIpoTree@@AAE_NHHHH@Z	; KIpoTree::CanLighting
	test	al, al
	je	SHORT $L88925

; 1075 : 					{
; 1076 : 						//float f = 65536.0 - ((float)(
; 1077 : 						//    (
; 1078 : 						//        ((float)(x - nLightPosX)) * ((float)(x - nLightPosX)) + 
; 1079 : 						//        ((float)(y - nLightPosY)) * ((float)(y - nLightPosY))
; 1080 : 						//    ) * 
; 1081 : 						//    fLightRadiusDenom *
; 1082 : 						//    65536.0
; 1083 : 						//));
; 1084 : 						float f = 65536.0 - ((float)(
; 1085 : 							sqrt(
; 1086 : 								(x - nLightPosX) * (x - nLightPosX) + 
; 1087 : 								(y - nLightPosY) * (y - nLightPosY)
; 1088 : 							) * 
; 1089 : 							fLightRadiusDenom * 
; 1090 : 							65536.0
; 1091 : 						));

	mov	eax, DWORD PTR _y$88892[esp+112]
	mov	ecx, edi
	sub	ecx, ebx
	sub	eax, ebp
	mov	edx, ecx
	imul	edx, ecx
	mov	ecx, eax
	imul	ecx, eax
	add	edx, ecx

; 1092 : 
; 1093 : 						uDistance65536 = (int)(f);

	mov	DWORD PTR -4+[esp+112], edx
	fild	DWORD PTR -4+[esp+112]
	fsqrt
	fmul	DWORD PTR _fLightRadiusDenom$88896[esp+112]
	fmul	QWORD PTR __real@8@400f8000000000000000
	fsubr	QWORD PTR __real@8@400f8000000000000000
	call	__ftol

; 1094 : 
; 1095 : 						if (((int)uDistance65536) > 0)

	test	eax, eax
	jle	SHORT $L88925

; 1096 : 						{
; 1097 : 							pCurPos->r += (ur * uDistance65536 / 65536);

	mov	edx, eax

; 1098 : 							pCurPos->g += (ug * uDistance65536 / 65536);

	mov	ecx, eax
	imul	edx, DWORD PTR _ur$88887[esp+112]
	imul	ecx, DWORD PTR _ug$88888[esp+112]

; 1099 : 							pCurPos->b += (ub * uDistance65536 / 65536);

	imul	eax, DWORD PTR _ub$88889[esp+112]
	shr	edx, 16					; 00000010H
	add	WORD PTR [esi], dx
	shr	ecx, 16					; 00000010H
	add	WORD PTR [esi+2], cx
	shr	eax, 16					; 00000010H
	add	WORD PTR [esi+4], ax
$L88925:

; 1100 : 						}
; 1101 : 					}
; 1102 : 
; 1103 : 					gx++;

	mov	eax, DWORD PTR _gx$88893[esp+112]
	inc	eax

; 1104 :                     if (gx >= LIGHTING_GRID_WIDTH)

	cmp	eax, 48					; 00000030H
	mov	DWORD PTR _gx$88893[esp+112], eax
	jge	SHORT $L91332
	mov	edx, DWORD PTR -88+[esp+112]
	mov	eax, DWORD PTR _j$88890[esp+112]

; 1105 :                         break;
; 1106 : 
; 1107 : 					x += LIGHTING_GRID_SIZEX;

	add	edi, 32					; 00000020H

; 1108 : 					pCurPos++;

	add	esi, 8
	inc	eax
	lea	ecx, DWORD PTR [edx+2]
	cmp	eax, ecx
	mov	DWORD PTR _j$88890[esp+112], eax
	jl	$L88917
$L91332:

; 1072 : 				for(j=0; j<(r * 2 + 1); j++)

	mov	edx, DWORD PTR -88+[esp+112]
	mov	eax, DWORD PTR -84+[esp+112]
$L91313:

; 1109 : 				}
; 1110 : 			}
; 1111 : 
; 1112 : 			gx = nLightGridX - r;
; 1113 : 			gy = nLightGridY - (r - 1);

	mov	ecx, DWORD PTR -80+[esp+112]

; 1114 : 			if (!
; 1115 : 				((gx < 0) || (gy < 0))
; 1116 : 			)

	test	eax, eax
	mov	DWORD PTR _gy$88894[esp+112], ecx
	jl	$L91314
	test	ecx, ecx
	jl	$L91314

; 1117 : 			{
; 1118 : 				x = nLightGridCenterX - r * LIGHTING_GRID_SIZEX;
; 1119 : 				y = nLightGridCenterY - (r - 1) * LIGHTING_GRID_SIZEY;
; 1120 : 				pCurPos = &pLColor[gy * LIGHTING_GRID_WIDTH + gx];

	mov	esi, DWORD PTR _this$[esp+112]
	mov	ecx, DWORD PTR -64+[esp+112]
	mov	edi, DWORD PTR -72+[esp+112]
	add	ecx, eax
	mov	esi, DWORD PTR [esi+73824]

; 1121 : 				for(j=0; j<(r * 2 - 1); j++)

	mov	DWORD PTR _j$88890[esp+112], 0
	test	edx, edx
	lea	esi, DWORD PTR [esi+ecx*8]
	jle	$L91314
$L88928:

; 1122 : 				{
; 1123 : 					if(CanLighting(x, y, nLightPosX, nLightPosY))

	mov	edx, DWORD PTR -76+[esp+112]
	mov	ecx, DWORD PTR _this$[esp+112]
	push	ebp
	push	ebx
	push	edi
	push	edx
	call	?CanLighting@KIpoTree@@AAE_NHHHH@Z	; KIpoTree::CanLighting
	test	al, al
	je	SHORT $L88936

; 1124 : 					{
; 1125 : 						//float f = 65536.0 - ((float)(
; 1126 : 						//    (
; 1127 : 						//        ((float)(x - nLightPosX)) * ((float)(x - nLightPosX)) + 
; 1128 : 						//        ((float)(y - nLightPosY)) * ((float)(y - nLightPosY))
; 1129 : 						//    ) * 
; 1130 : 						//    fLightRadiusDenom *
; 1131 : 						//    65536.0
; 1132 : 						//));
; 1133 : 						float f = 65536.0 - ((float)(
; 1134 : 							sqrt(
; 1135 : 								(x - nLightPosX) * (x - nLightPosX) + 
; 1136 : 								(y - nLightPosY) * (y - nLightPosY)
; 1137 : 							) * 
; 1138 : 							fLightRadiusDenom * 
; 1139 : 							65536.0
; 1140 : 						));

	mov	ecx, DWORD PTR -76+[esp+112]
	mov	eax, edi
	sub	ecx, ebx
	sub	eax, ebp
	mov	edx, ecx
	imul	edx, ecx
	mov	ecx, eax
	imul	ecx, eax
	add	edx, ecx

; 1141 : 
; 1142 : 						uDistance65536 = (int)(f);

	mov	DWORD PTR -4+[esp+112], edx
	fild	DWORD PTR -4+[esp+112]
	fsqrt
	fmul	DWORD PTR _fLightRadiusDenom$88896[esp+112]
	fmul	QWORD PTR __real@8@400f8000000000000000
	fsubr	QWORD PTR __real@8@400f8000000000000000
	call	__ftol

; 1143 : 
; 1144 : 						if (((int)uDistance65536) > 0)

	test	eax, eax
	jle	SHORT $L88936

; 1145 : 						{
; 1146 : 							pCurPos->r += (ur * uDistance65536 / 65536);

	mov	edx, eax

; 1147 : 							pCurPos->g += (ug * uDistance65536 / 65536);

	mov	ecx, eax
	imul	edx, DWORD PTR _ur$88887[esp+112]
	imul	ecx, DWORD PTR _ug$88888[esp+112]

; 1148 : 							pCurPos->b += (ub * uDistance65536 / 65536);

	imul	eax, DWORD PTR _ub$88889[esp+112]
	shr	edx, 16					; 00000010H
	add	WORD PTR [esi], dx
	shr	ecx, 16					; 00000010H
	add	WORD PTR [esi+2], cx
	shr	eax, 16					; 00000010H
	add	WORD PTR [esi+4], ax
$L88936:

; 1149 : 						}
; 1150 : 					}
; 1151 : 
; 1152 : 					gy++;

	mov	eax, DWORD PTR _gy$88894[esp+112]
	inc	eax

; 1153 :                     if (gy >= LIGHTING_GRID_HEIGHT)

	cmp	eax, 96					; 00000060H
	mov	DWORD PTR _gy$88894[esp+112], eax
	jge	SHORT $L91333
	mov	eax, DWORD PTR _j$88890[esp+112]
	mov	ecx, DWORD PTR -88+[esp+112]

; 1154 :                         break;
; 1155 : 
; 1156 : 					y += LIGHTING_GRID_SIZEX;

	add	edi, 32					; 00000020H

; 1157 : 					pCurPos += LIGHTING_GRID_WIDTH;

	add	esi, 384				; 00000180H
	inc	eax
	cmp	eax, ecx
	mov	DWORD PTR _j$88890[esp+112], eax
	jl	$L88928
$L91333:

; 1121 : 				for(j=0; j<(r * 2 - 1); j++)

	mov	edx, DWORD PTR -88+[esp+112]
	mov	eax, DWORD PTR -84+[esp+112]
$L91314:

; 1158 : 				}
; 1159 : 			}
; 1160 : 
; 1161 : 			
; 1162 : 			gx = nLightGridX + r;
; 1163 : 			gy = nLightGridY - (r - 1);
; 1164 : 			if (!
; 1165 : 				((gx >= LIGHTING_GRID_WIDTH ) || (gy < 0))
; 1166 : 			)

	mov	ecx, DWORD PTR -40+[esp+112]
	mov	esi, DWORD PTR -80+[esp+112]
	cmp	ecx, 48					; 00000030H
	mov	DWORD PTR _gy$88894[esp+112], esi
	jge	$L88902
	test	esi, esi
	jl	$L88902

; 1167 : 			{
; 1168 : 				x = nLightGridCenterX + r * LIGHTING_GRID_SIZEX;
; 1169 : 				y = nLightGridCenterY - (r - 1) * LIGHTING_GRID_SIZEY;
; 1170 : 				pCurPos = &pLColor[gy * LIGHTING_GRID_WIDTH + gx];

	mov	esi, DWORD PTR -64+[esp+112]
	mov	edi, DWORD PTR -72+[esp+112]
	add	esi, ecx
	mov	ecx, DWORD PTR _this$[esp+112]

; 1171 : 				for(j=0; j<(r * 2 - 1); j++)

	test	edx, edx
	mov	ecx, DWORD PTR [ecx+73824]
	mov	DWORD PTR _j$88890[esp+112], 0
	lea	esi, DWORD PTR [ecx+esi*8]
	jle	$L88902
$L88939:

; 1172 : 				{
; 1173 : 					if(CanLighting(x, y, nLightPosX, nLightPosY))

	mov	edx, DWORD PTR -60+[esp+112]
	mov	ecx, DWORD PTR _this$[esp+112]
	push	ebp
	push	ebx
	push	edi
	push	edx
	call	?CanLighting@KIpoTree@@AAE_NHHHH@Z	; KIpoTree::CanLighting
	test	al, al
	je	SHORT $L88947

; 1174 : 					{
; 1175 : 						//float f = 65536.0 - ((float)(
; 1176 : 						//    (
; 1177 : 						//        ((float)(x - nLightPosX)) * ((float)(x - nLightPosX)) + 
; 1178 : 						//        ((float)(y - nLightPosY)) * ((float)(y - nLightPosY))
; 1179 : 						//    ) * 
; 1180 : 						//    fLightRadiusDenom *
; 1181 : 						//    65536.0
; 1182 : 						//));
; 1183 : 						float f = 65536.0 - ((float)(
; 1184 : 							sqrt(
; 1185 : 								(x - nLightPosX) * (x - nLightPosX) + 
; 1186 : 								(y - nLightPosY) * (y - nLightPosY)
; 1187 : 							) * 
; 1188 : 							fLightRadiusDenom * 
; 1189 : 							65536.0
; 1190 : 						));

	mov	ecx, DWORD PTR -60+[esp+112]
	mov	eax, edi
	sub	ecx, ebx
	sub	eax, ebp
	mov	edx, ecx
	imul	edx, ecx
	mov	ecx, eax
	imul	ecx, eax
	add	edx, ecx

; 1191 : 
; 1192 : 						uDistance65536 = (int)(f);

	mov	DWORD PTR -4+[esp+112], edx
	fild	DWORD PTR -4+[esp+112]
	fsqrt
	fmul	DWORD PTR _fLightRadiusDenom$88896[esp+112]
	fmul	QWORD PTR __real@8@400f8000000000000000
	fsubr	QWORD PTR __real@8@400f8000000000000000
	call	__ftol

; 1193 : 
; 1194 : 						if (((int)uDistance65536) > 0)

	test	eax, eax
	jle	SHORT $L88947

; 1195 : 						{
; 1196 : 							pCurPos->r += (ur * uDistance65536 / 65536);

	mov	edx, eax

; 1197 : 							pCurPos->g += (ug * uDistance65536 / 65536);

	mov	ecx, eax
	imul	edx, DWORD PTR _ur$88887[esp+112]
	imul	ecx, DWORD PTR _ug$88888[esp+112]

; 1198 : 							pCurPos->b += (ub * uDistance65536 / 65536);

	imul	eax, DWORD PTR _ub$88889[esp+112]
	shr	edx, 16					; 00000010H
	add	WORD PTR [esi], dx
	shr	ecx, 16					; 00000010H
	add	WORD PTR [esi+2], cx
	shr	eax, 16					; 00000010H
	add	WORD PTR [esi+4], ax
$L88947:

; 1199 : 						}
; 1200 : 					}
; 1201 : 
; 1202 : 					gy++;

	mov	eax, DWORD PTR _gy$88894[esp+112]
	inc	eax

; 1203 :                     if (gy >= LIGHTING_GRID_HEIGHT)

	cmp	eax, 96					; 00000060H
	mov	DWORD PTR _gy$88894[esp+112], eax
	jge	SHORT $L91334
	mov	eax, DWORD PTR _j$88890[esp+112]
	mov	ecx, DWORD PTR -88+[esp+112]

; 1204 :                         break;
; 1205 : 
; 1206 : 					y += LIGHTING_GRID_SIZEX;

	add	edi, 32					; 00000020H

; 1207 : 					pCurPos += LIGHTING_GRID_WIDTH;

	add	esi, 384				; 00000180H
	inc	eax
	cmp	eax, ecx
	mov	DWORD PTR _j$88890[esp+112], eax
	jl	$L88939
$L91334:

; 1171 : 				for(j=0; j<(r * 2 - 1); j++)

	mov	edx, DWORD PTR -88+[esp+112]
	mov	eax, DWORD PTR -84+[esp+112]
$L88902:
	mov	ebx, DWORD PTR -28+[esp+112]
	mov	edi, DWORD PTR -40+[esp+112]
	mov	ecx, 48					; 00000030H
	mov	esi, DWORD PTR -72+[esp+112]
	add	ebx, ecx
	inc	edi
	dec	eax
	mov	ebp, DWORD PTR -64+[esp+112]
	mov	DWORD PTR -84+[esp+112], eax
	mov	eax, DWORD PTR -80+[esp+112]
	dec	eax
	mov	DWORD PTR -28+[esp+112], ebx
	mov	ebx, DWORD PTR -60+[esp+112]
	mov	DWORD PTR -40+[esp+112], edi
	mov	edi, DWORD PTR -76+[esp+112]
	mov	DWORD PTR -80+[esp+112], eax
	mov	eax, 32					; 00000020H
	sub	ebp, ecx
	add	ebx, eax
	sub	edi, eax
	sub	esi, eax
	mov	eax, DWORD PTR -16+[esp+112]
	add	edx, 2
	dec	eax
	mov	DWORD PTR -64+[esp+112], ebp
	mov	DWORD PTR -60+[esp+112], ebx
	mov	DWORD PTR -76+[esp+112], edi
	mov	DWORD PTR -72+[esp+112], esi
	mov	DWORD PTR -88+[esp+112], edx
	mov	DWORD PTR -16+[esp+112], eax
	jne	$L88901
$L91342:

; 1002 :         //fLightRadiusDenom *= fLightRadiusDenom;
; 1003 :         unsigned int uDistance65536 = 0;
; 1004 :         KLColor *pCurPos = NULL;
; 1005 : 
; 1006 : 		
; 1007 : 		gy = nLightGridY - (nGridRadius - 1);
; 1008 : 		// 从中心点向外扩展计算光照强度
; 1009 : 		for(int r = 0; r < nGridRadius; r++)

	mov	esi, DWORD PTR _i$[esp+112]
	mov	ebx, DWORD PTR _this$[esp+112]
$L88876:
	mov	esi, DWORD PTR [esi]
	mov	eax, DWORD PTR [ebx+92276]
	cmp	esi, eax
	mov	DWORD PTR _i$[esp+112], esi
	jne	$L91336
$L88877:

; 1208 : 				}
; 1209 : 			}
; 1210 : 		}
; 1211 : 	}
; 1212 : 
; 1213 :     //memcpy(ptp, pLColor, sizeof(ptp));
; 1214 : 
; 1215 :     
; 1216 : 	// 对光照图进行过滤，使阴影边沿平滑
; 1217 :     
; 1218 : 
; 1219 : 	int m;
; 1220 :     int n;
; 1221 :     DWORD   *pdwLight;
; 1222 :     KLColor *pO, *pL, *pR, *pU, *pD, *pDest;
; 1223 :     int nIdx = 1 * LIGHTING_GRID_WIDTH + 1;

	mov	esi, 392				; 00000188H
	lea	edx, DWORD PTR [ebx+74036]
	mov	DWORD PTR -28+[esp+112], esi
	mov	DWORD PTR -24+[esp+112], edx
	mov	DWORD PTR -40+[esp+112], 94		; 0000005eH
	jmp	SHORT $L88959
$L91335:
	mov	esi, DWORD PTR -28+[esp+112]
	mov	ebx, DWORD PTR _this$[esp+112]
$L88959:

; 1224 : 	for(m=1; m<LIGHTING_GRID_HEIGHT-1;m++)
; 1225 : 	{
; 1226 : 		pDest = ptp + nIdx;
; 1227 :         pdwLight = pLightingArray + nIdx;

	mov	eax, DWORD PTR -24+[esp+112]
	mov	edx, DWORD PTR [ebx+73832]
	mov	ecx, esi
	mov	DWORD PTR _pdwLight$[esp+112], eax

; 1228 : 		pO = pLColor + nIdx;

	mov	eax, DWORD PTR [ebx+73824]
	add	ecx, edx
	add	esi, eax
	mov	DWORD PTR -36+[esp+112], 46		; 0000002eH

; 1229 : 		pL = pO - 1;

	lea	edi, DWORD PTR [esi-8]

; 1230 : 		pR = pO + 1;
; 1231 : 		pU = pO - LIGHTING_GRID_WIDTH;

	lea	ebx, DWORD PTR [esi-384]

; 1232 : 		pD = pO + LIGHTING_GRID_WIDTH;

	lea	ebp, DWORD PTR [esi+384]
$L88962:

; 1233 : 		for(n=1; n<LIGHTING_GRID_WIDTH-1;n++)
; 1234 : 		{
; 1235 : 			//pDest->r = (pO->r + pR->r + pL->r + pU->r + pD->r) / 5;
; 1236 : 			//pDest->g = (pO->g + pR->g + pL->g + pU->g + pD->g) / 5;
; 1237 : 			//pDest->b = (pO->b + pR->b + pL->b + pU->b + pD->b) / 5;
; 1238 : 			//pDest->r = (pO->r + pR->r + pL->r + (pU->r + pD->r) / 2) / 4;
; 1239 : 			//pDest->g = (pO->g + pR->g + pL->g + (pU->g + pD->g) / 2) / 4;
; 1240 : 			//pDest->b = (pO->b + pR->b + pL->b + (pU->b + pD->b) / 2) / 4;
; 1241 : 			//pDest->r = (pO->r + (pR->r + pL->r) / 2 + pU->r + pD->r) / 4;
; 1242 : 			//pDest->g = (pO->g + (pR->g + pL->g) / 2 + pU->g + pD->g) / 4;
; 1243 : 			//pDest->b = (pO->b + (pR->b + pL->b) / 2 + pU->b + pD->b) / 4;
; 1244 : 			
; 1245 :             pDest->r = (pO->r + pL->r + pU->r + pD->r) / 4;

	xor	eax, eax
	xor	edx, edx
	mov	ax, WORD PTR [ebp]
	mov	dx, WORD PTR [ebx]
	add	eax, edx
	xor	edx, edx
	mov	dx, WORD PTR [edi]
	add	eax, edx
	xor	edx, edx
	mov	dx, WORD PTR [esi]
	add	eax, edx
	cdq
	and	edx, 3
	add	eax, edx

; 1246 : 		    pDest->g = (pO->g + pL->g + pU->g + pD->g) / 4;

	xor	edx, edx
	sar	eax, 2
	mov	WORD PTR [ecx], ax
	mov	dx, WORD PTR [ebx+2]
	xor	eax, eax
	mov	ax, WORD PTR [ebp+2]
	add	eax, edx
	xor	edx, edx
	mov	dx, WORD PTR [edi+2]
	add	eax, edx
	xor	edx, edx
	mov	dx, WORD PTR [esi+2]
	add	eax, edx
	cdq
	and	edx, 3
	add	eax, edx

; 1247 : 			pDest->b = (pO->b + pL->b + pU->b + pD->b) / 4;

	xor	edx, edx
	sar	eax, 2
	mov	WORD PTR [ecx+2], ax
	mov	dx, WORD PTR [ebx+4]
	xor	eax, eax
	mov	ax, WORD PTR [ebp+4]
	add	eax, edx
	xor	edx, edx
	mov	dx, WORD PTR [edi+4]
	add	eax, edx
	xor	edx, edx
	mov	dx, WORD PTR [esi+4]
	add	eax, edx
	cdq
	and	edx, 3
	add	eax, edx
	sar	eax, 2

; 1248 : 
; 1249 :             *pdwLight = 0xff000000 |
; 1250 :                 (((pDest->r > 0xff) ? 0xff : pDest->r) << 16) |
; 1251 :                 (((pDest->g > 0xff) ? 0xff : pDest->g) << 8) |
; 1252 :                 (((pDest->b > 0xff) ? 0xff : pDest->b));

	cmp	WORD PTR [ecx], 255			; 000000ffH
	mov	WORD PTR [ecx+4], ax
	jbe	SHORT $L91217
	mov	eax, 255				; 000000ffH
	jmp	SHORT $L91218
$L91217:
	xor	eax, eax
	mov	ax, WORD PTR [ecx]
$L91218:
	mov	dx, WORD PTR [ecx+2]
	cmp	dx, 255					; 000000ffH
	jbe	SHORT $L91219
	mov	DWORD PTR -16+[esp+112], 255		; 000000ffH
	jmp	SHORT $L91220
$L91219:
	and	edx, 65535				; 0000ffffH
	mov	DWORD PTR -16+[esp+112], edx
$L91220:
	mov	dx, WORD PTR [ecx+4]
	cmp	dx, 255					; 000000ffH
	jbe	SHORT $L91221
	mov	DWORD PTR -20+[esp+112], 255		; 000000ffH
	jmp	SHORT $L91222
$L91221:
	and	edx, 65535				; 0000ffffH
	mov	DWORD PTR -20+[esp+112], edx
$L91222:
	mov	edx, DWORD PTR -16+[esp+112]
	or	eax, -256				; ffffff00H
	shl	eax, 8
	or	eax, edx
	mov	edx, DWORD PTR -20+[esp+112]
	shl	eax, 8
	or	eax, edx
	mov	edx, DWORD PTR _pdwLight$[esp+112]

; 1253 : 
; 1254 : 
; 1255 :             //pDest->r = pO->r;
; 1256 : 		    //pDest->g = pO->g;
; 1257 : 			//pDest->b = pO->b;
; 1258 : 
; 1259 : 			pDest++, pO++, pL++, pR++, pU++, pD++, pdwLight++;

	add	ecx, 8
	add	esi, 8
	mov	DWORD PTR [edx], eax
	mov	eax, DWORD PTR -36+[esp+112]
	add	edi, 8
	add	ebx, 8
	add	ebp, 8
	add	edx, 4
	dec	eax
	mov	DWORD PTR _pdwLight$[esp+112], edx
	mov	DWORD PTR -36+[esp+112], eax
	jne	$L88962

; 1260 : 		}
; 1261 : 		nIdx += LIGHTING_GRID_WIDTH;

	mov	edx, DWORD PTR -24+[esp+112]
	mov	ecx, DWORD PTR -28+[esp+112]
	mov	eax, DWORD PTR -40+[esp+112]
	add	edx, 192				; 000000c0H
	add	ecx, 384				; 00000180H
	dec	eax
	mov	DWORD PTR -24+[esp+112], edx
	mov	DWORD PTR -28+[esp+112], ecx
	mov	DWORD PTR -40+[esp+112], eax
	jne	$L91335

; 1262 : 	}
; 1263 :     
; 1264 : 
; 1265 : 	// 将r，g，b的值限制在0xff之内，防止色彩错误
; 1266 : //	for(j = 0; j < LIGHTING_GRID_WIDTH * LIGHTING_GRID_HEIGHT; j++)
; 1267 : //	{
; 1268 : //        //pLightingArray[j] = 0xff000000 |
; 1269 : //        //    ((((-(ptp[j].r > 0xff)) | ptp[j].r) & 0xff) << 16) |
; 1270 : //        //    ((((-(ptp[j].g > 0xff)) | ptp[j].g) & 0xff) << 8) |
; 1271 : //        //    ((((-(ptp[j].b > 0xff)) | ptp[j].b) & 0xff));
; 1272 : //
; 1273 : //		//if(ptp[j].r > 0xff)
; 1274 : //		//	ptp[j].r = 0xff;
; 1275 : //        
; 1276 : //		//if(ptp[j].g > 0xff)
; 1277 : //		//	ptp[j].g = 0xff;
; 1278 : //		//if(ptp[j].b > 0xff)
; 1279 : //		//	ptp[j].b = 0xff;
; 1280 : //		//pLightingArray[j] = 0xff000000 | (ptp[j].r<<16) | (ptp[j].g<<8) | ptp[j].b;
; 1281 : //
; 1282 : //        pLightingArray[j] = 0xff000000 |
; 1283 : //            (((ptp[j].r > 0xff) ? 0xff : ptp[j].r) << 16) |
; 1284 : //            (((ptp[j].g > 0xff) ? 0xff : ptp[j].g) << 8) |
; 1285 : //            (((ptp[j].b > 0xff) ? 0xff : ptp[j].b));
; 1286 : //
; 1287 : //	}
; 1288 :     for (m = 0; m < LIGHTING_GRID_HEIGHT; m++)

	mov	eax, DWORD PTR _this$[esp+112]
	mov	edx, 96					; 00000060H
	lea	ecx, DWORD PTR [eax+74028]
$L88965:

; 1289 :     {
; 1290 :         pLightingArray[m * LIGHTING_GRID_WIDTH + 0] = m_dwAmbient | 0xff000000;

	mov	esi, DWORD PTR [eax+92]
	add	ecx, 192				; 000000c0H
	or	esi, -16777216				; ff000000H
	mov	DWORD PTR [ecx-380], esi

; 1291 :         pLightingArray[m * LIGHTING_GRID_WIDTH + LIGHTING_GRID_WIDTH - 1] = m_dwAmbient | 0xff000000;

	mov	esi, DWORD PTR [eax+92]
	or	esi, -16777216				; ff000000H
	dec	edx
	mov	DWORD PTR [ecx-192], esi
	jne	SHORT $L88965

; 1292 :     }
; 1293 : 
; 1294 :     for (n = 0; n < LIGHTING_GRID_WIDTH; n++)

	lea	ecx, DWORD PTR [eax+92080]
	mov	edx, 48					; 00000030H
$L88968:

; 1295 :     {
; 1296 :         pLightingArray[n] = m_dwAmbient | 0xff000000;

	mov	esi, DWORD PTR [eax+92]
	add	ecx, 4
	or	esi, -16777216				; ff000000H
	mov	DWORD PTR [ecx-18244], esi

; 1297 :         pLightingArray[(LIGHTING_GRID_HEIGHT - 1) * LIGHTING_GRID_WIDTH + n] = m_dwAmbient | 0xff000000;

	mov	esi, DWORD PTR [eax+92]
	or	esi, -16777216				; ff000000H
	dec	edx
	mov	DWORD PTR [ecx-4], esi
	jne	SHORT $L88968
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1298 :     }
; 1299 :     //pLightingArray[0] = m_dwAmbient;    // 设置环境光
; 1300 : }

	add	esp, 96					; 00000060H
	ret	0
?RenderLightMap@KIpoTree@@QAEXXZ ENDP			; KIpoTree::RenderLightMap
_TEXT	ENDS
PUBLIC	__real@4@00000000000000000000
;	COMDAT __real@4@00000000000000000000
; File D:\TINHDUYEN\SwordOnline\SwordOnline\Sources\Core\Src\Scene\KIpoTree.cpp
CONST	SEGMENT
__real@4@00000000000000000000 DD 000000000r	; 0
CONST	ENDS
;	COMDAT ?CanLighting@KIpoTree@@AAE_NHHHH@Z
_TEXT	SEGMENT
_x$ = 8
_y$ = 12
_nLightPosX$ = 16
_nLightPosY$ = 20
_nGridLightPosX$ = -24
_nGridLightPosY$ = -20
_nObstacle$ = -28
_vDir$ = -16
_vLight$ = -8
_fdx$ = -32
_fdy$ = 20
?CanLighting@KIpoTree@@AAE_NHHHH@Z PROC NEAR		; KIpoTree::CanLighting, COMDAT

; 1304 : {

	sub	esp, 32					; 00000020H

; 1305 : 	int nGridX, nGridY, nGridLightPosX, nGridLightPosY, nGridStep, nCurrentGridX, nCurrentGridY, nIdx;
; 1306 : 	nGridX = x / LIGHTING_GRID_SIZEX;

	mov	eax, DWORD PTR _x$[esp+28]
	push	ebx
	cdq
	and	edx, 31					; 0000001fH
	push	ebp
	add	eax, edx
	push	esi
	mov	ebx, eax

; 1307 : 	nGridY = y / LIGHTING_GRID_SIZEX;

	mov	eax, DWORD PTR _y$[esp+40]
	cdq
	and	edx, 31					; 0000001fH
	push	edi
	add	eax, edx
	mov	ebp, ecx
	mov	edi, eax

; 1308 : 	nGridLightPosX = nLightPosX / LIGHTING_GRID_SIZEX;

	mov	eax, DWORD PTR _nLightPosX$[esp+44]
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	mov	ecx, eax

; 1309 : 	nGridLightPosY = nLightPosY / LIGHTING_GRID_SIZEX;

	mov	eax, DWORD PTR _nLightPosY$[esp+44]
	cdq
	and	edx, 31					; 0000001fH
	add	edx, eax
	sar	ebx, 5
	sar	ecx, 5
	sar	edi, 5
	sar	edx, 5

; 1310 : 	if(nGridX == nGridLightPosX && nGridY == nGridLightPosY)

	cmp	ebx, ecx
	mov	DWORD PTR _nGridLightPosX$[esp+48], ecx
	mov	DWORD PTR _nGridLightPosY$[esp+48], edx
	jne	SHORT $L88986
	cmp	edi, edx
	jne	SHORT $L88986
	pop	edi
	pop	esi
	pop	ebp

; 1311 : 		return true;

	mov	al, 1
	pop	ebx

; 1412 : 		}
; 1413 : 	}
; 1414 : }

	add	esp, 32					; 00000020H
	ret	16					; 00000010H
$L88986:

; 1312 : 
; 1313 : 	int nObstacle = pObstacle[nGridY * LIGHTING_GRID_WIDTH + nGridX].nObstacle;

	lea	eax, DWORD PTR [edi+edi*2]
	shl	eax, 4
	add	eax, ebx
	lea	esi, DWORD PTR [eax+6]
	shl	esi, 4

; 1314 : 	KVector2 vDir = pObstacle[nGridY * LIGHTING_GRID_WIDTH + nGridX].vDir;

	shl	eax, 4
	mov	esi, DWORD PTR [esi+ebp]
	mov	DWORD PTR _nObstacle$[esp+48], esi
	mov	esi, DWORD PTR [eax+ebp+100]
	mov	eax, DWORD PTR [eax+ebp+104]
	mov	DWORD PTR _vDir$[esp+48], esi

; 1315 : 	KVector2 vLight;
; 1316 : 	vLight.fX = (float)(x - nLightPosX);

	mov	esi, DWORD PTR _nLightPosX$[esp+44]
	mov	DWORD PTR _vDir$[esp+52], eax
	mov	eax, DWORD PTR _x$[esp+44]
	sub	eax, esi

; 1317 : 	vLight.fY = (float)(y - nLightPosY);

	mov	esi, DWORD PTR _nLightPosY$[esp+44]
	mov	DWORD PTR -32+[esp+48], eax
	mov	eax, DWORD PTR _y$[esp+44]
	fild	DWORD PTR -32+[esp+48]
	sub	eax, esi
	mov	DWORD PTR -32+[esp+48], eax
	fstp	DWORD PTR _vLight$[esp+48]
	fild	DWORD PTR -32+[esp+48]
	fstp	DWORD PTR _vLight$[esp+52]

; 1318 : 
; 1319 : 	float fx, fy, fLightPosX, fLightPosY, fdx, fdy, fStepX, fStepY;
; 1320 : 	fx = (float)x, fy =(float) y, fLightPosX = (float)nLightPosX, fLightPosY = (float)nLightPosY;

	fild	DWORD PTR _x$[esp+44]
	fild	DWORD PTR _y$[esp+44]

; 1321 : 	fdx = (float)fabs(fLightPosX - fx);

	fild	DWORD PTR _nLightPosX$[esp+44]
	fsub	ST(0), ST(2)
	fabs
	fst	DWORD PTR _fdx$[esp+48]

; 1322 : 	fdy = (float)fabs(fLightPosY - fy);

	fild	DWORD PTR _nLightPosY$[esp+44]
	fsub	ST(0), ST(2)
	fabs
	fstp	DWORD PTR _fdy$[esp+44]

; 1323 : 
; 1324 : 	// 斜率小于45度的情况
; 1325 : 	if(fdx >= fdy)

	fcomp	DWORD PTR _fdy$[esp+44]
	fnstsw	ax
	test	ah, 1
	jne	$L89006

; 1326 : 	{
; 1327 : 		// 计算线段前进累加值
; 1328 : 		if(nLightPosX > x)

	mov	edx, DWORD PTR _x$[esp+44]
	mov	eax, DWORD PTR _nLightPosX$[esp+44]
	cmp	eax, edx
	jle	SHORT $L89007

; 1329 : 		{
; 1330 : 			nGridStep = 1;
; 1331 : 			fStepX = LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@40048000000000000000
	mov	edi, 1

; 1332 : 		}
; 1333 : 		else

	jmp	SHORT $L89008
$L89007:

; 1334 : 		{
; 1335 : 			nGridStep = -1;
; 1336 : 			fStepX = -LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@c0048000000000000000
	or	edi, -1
$L89008:

; 1337 : 		}
; 1338 : 		if(nLightPosY > y)

	mov	eax, DWORD PTR _y$[esp+44]

; 1339 : 			fStepY = fdy / fdx * LIGHTING_GRID_SIZEX;

	fld	DWORD PTR _fdy$[esp+44]
	fdiv	DWORD PTR _fdx$[esp+48]
	cmp	esi, eax
	jle	SHORT $L89009
	fmul	DWORD PTR __real@4@40048000000000000000

; 1340 : 		else

	jmp	SHORT $L89012
$L89009:

; 1341 : 			fStepY = -fdy / fdx * LIGHTING_GRID_SIZEX;

	fmul	DWORD PTR __real@4@c0048000000000000000

; 1342 : 
; 1343 : 		while(1)

	jmp	SHORT $L89012
$L91370:
	mov	ecx, DWORD PTR _nGridLightPosX$[esp+48]
$L89012:

; 1344 : 		{
; 1345 : 			fx += fStepX;

	fld	ST(1)
	faddp	ST(4), ST(0)

; 1346 : 			fy += fStepY;
; 1347 : 			nGridX += nGridStep;

	add	ebx, edi

; 1348 : 			if(nGridX == nGridLightPosX)

	cmp	ebx, ecx
	fld	ST(0)
	faddp	ST(3), ST(0)
	je	SHORT $L91362

; 1355 : 					else
; 1356 : 						return false;
; 1357 : 				}
; 1358 : 				else
; 1359 : 					return true;
; 1360 : 			}
; 1361 : 
; 1362 : 			nCurrentGridX = ((int)fx) / LIGHTING_GRID_SIZEX;
; 1363 : 			nCurrentGridY = ((int)fy) / LIGHTING_GRID_SIZEX;
; 1364 : 			nIdx = nCurrentGridY * LIGHTING_GRID_WIDTH + nCurrentGridX;

	fld	ST(2)
	call	__ftol
	cdq
	fld	ST(3)
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	lea	esi, DWORD PTR [eax+eax*2]
	shl	esi, 4
	call	__ftol
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	add	esi, eax

; 1365 : 			// 检查是否碰到挡光障碍
; 1366 : 			if(pObstacle[nIdx].nObstacle != 0 && pObstacle[nIdx].nObstacle != nObstacle)

	add	esi, 6
	shl	esi, 4
	mov	eax, DWORD PTR [esi+ebp]
	test	eax, eax
	je	SHORT $L91370
	cmp	eax, DWORD PTR _nObstacle$[esp+48]
	jne	$L91361

; 1342 : 
; 1343 : 		while(1)

	jmp	SHORT $L91370
$L91362:

; 1349 : 			{
; 1350 : 				// 没有碰到障碍，根据这点本身是否障碍和障碍方向判断是否受光
; 1351 : 				if(nObstacle && vDir.fX != 0.0f)

	mov	eax, DWORD PTR _nObstacle$[esp+48]
	fstp	ST(0)
	fstp	ST(0)
	fstp	ST(0)
	test	eax, eax
	fstp	ST(0)
	je	SHORT $L89015
	fld	DWORD PTR _vDir$[esp+48]
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 64					; 00000040H
	jne	SHORT $L89015

; 1352 : 				{
; 1353 : 					if(vDir.fX * vLight.fY - vDir.fY * vLight.fX < 0.0f)

	fld	DWORD PTR _vDir$[esp+48]
	fmul	DWORD PTR _vLight$[esp+52]
	fld	DWORD PTR _vDir$[esp+52]
	fmul	DWORD PTR _vLight$[esp+48]
	fsubp	ST(1), ST(0)
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 1
	je	$L89032
$L89015:
	pop	edi
	pop	esi
	pop	ebp

; 1354 : 						return true;

	mov	al, 1
	pop	ebx

; 1412 : 		}
; 1413 : 	}
; 1414 : }

	add	esp, 32					; 00000020H
	ret	16					; 00000010H
$L89006:

; 1367 : 				return false;
; 1368 : 		}
; 1369 : 	}
; 1370 : 	// 斜率大于45度的情况
; 1371 : 	else
; 1372 : 	{
; 1373 : 		if(nLightPosY > y)

	cmp	esi, DWORD PTR _y$[esp+44]
	jle	SHORT $L89023

; 1374 : 		{
; 1375 : 			nGridStep = 1;
; 1376 : 			fStepY = LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@40048000000000000000
	mov	ebx, 1

; 1377 : 		}
; 1378 : 		else

	jmp	SHORT $L89024
$L89023:

; 1379 : 		{
; 1380 : 			nGridStep = -1;
; 1381 : 			fStepY = -LIGHTING_GRID_SIZEX;

	fld	DWORD PTR __real@4@c0048000000000000000
	or	ebx, -1
$L89024:

; 1382 : 		}
; 1383 : 		if(nLightPosX > x)

	mov	eax, DWORD PTR _x$[esp+44]
	mov	ecx, DWORD PTR _nLightPosX$[esp+44]

; 1384 : 			fStepX = fdx / fdy * LIGHTING_GRID_SIZEX;

	fld	DWORD PTR _fdx$[esp+48]
	fdiv	DWORD PTR _fdy$[esp+44]
	cmp	ecx, eax
	jle	SHORT $L89025
	fmul	DWORD PTR __real@4@40048000000000000000

; 1385 : 		else

	jmp	SHORT $L89028
$L89025:

; 1386 : 			fStepX = -fdx / fdy * LIGHTING_GRID_SIZEX;

	fmul	DWORD PTR __real@4@c0048000000000000000

; 1387 : 		while(1)

	jmp	SHORT $L89028
$L91371:
	mov	edx, DWORD PTR _nGridLightPosY$[esp+48]
$L89028:

; 1388 : 		{
; 1389 : 			fx += fStepX;

	fld	ST(0)
	faddp	ST(4), ST(0)

; 1390 : 			fy += fStepY;
; 1391 : 			nGridY += nGridStep;

	add	edi, ebx
	fld	ST(1)

; 1392 : 			if(nGridY == nGridLightPosY)

	cmp	edi, edx
	faddp	ST(3), ST(0)
	je	$L91362

; 1393 : 			{
; 1394 : 				// 没有碰到障碍，根据这点本身是否障碍和障碍方向判断是否受光
; 1395 : 				if(nObstacle && vDir.fX != 0.0f)
; 1396 : 				{
; 1397 : 					if(vDir.fX * vLight.fY - vDir.fY * vLight.fX < 0.0f)
; 1398 : 						return true;
; 1399 : 					else
; 1400 : 						return false;
; 1401 : 				}
; 1402 : 				else
; 1403 : 					return true;
; 1404 : 			}
; 1405 : 
; 1406 : 			nCurrentGridX = ((int)fx) / LIGHTING_GRID_SIZEX;
; 1407 : 			nCurrentGridY = ((int)fy) / LIGHTING_GRID_SIZEX;
; 1408 : 			nIdx = nCurrentGridY * LIGHTING_GRID_WIDTH + nCurrentGridX;

	fld	ST(2)
	call	__ftol
	cdq
	fld	ST(3)
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	lea	esi, DWORD PTR [eax+eax*2]
	shl	esi, 4
	call	__ftol
	cdq
	and	edx, 31					; 0000001fH
	add	eax, edx
	sar	eax, 5
	add	esi, eax

; 1409 : 			// 检查是否碰到挡光障碍
; 1410 : 			if(pObstacle[nIdx].nObstacle != 0 && pObstacle[nIdx].nObstacle != nObstacle)

	add	esi, 6
	shl	esi, 4
	mov	eax, DWORD PTR [esi+ebp]
	test	eax, eax
	je	SHORT $L91371
	cmp	eax, DWORD PTR _nObstacle$[esp+48]
	je	SHORT $L91371
$L91361:

; 1393 : 			{
; 1394 : 				// 没有碰到障碍，根据这点本身是否障碍和障碍方向判断是否受光
; 1395 : 				if(nObstacle && vDir.fX != 0.0f)
; 1396 : 				{
; 1397 : 					if(vDir.fX * vLight.fY - vDir.fY * vLight.fX < 0.0f)
; 1398 : 						return true;
; 1399 : 					else
; 1400 : 						return false;
; 1401 : 				}
; 1402 : 				else
; 1403 : 					return true;
; 1404 : 			}
; 1405 : 
; 1406 : 			nCurrentGridX = ((int)fx) / LIGHTING_GRID_SIZEX;
; 1407 : 			nCurrentGridY = ((int)fy) / LIGHTING_GRID_SIZEX;
; 1408 : 			nIdx = nCurrentGridY * LIGHTING_GRID_WIDTH + nCurrentGridX;

	fstp	ST(0)
	fstp	ST(0)
	fstp	ST(0)
	fstp	ST(0)
$L89032:
	pop	edi
	pop	esi
	pop	ebp

; 1411 : 				return false;

	xor	al, al
	pop	ebx

; 1412 : 		}
; 1413 : 	}
; 1414 : }

	add	esp, 32					; 00000020H
	ret	16					; 00000010H
?CanLighting@KIpoTree@@AAE_NHHHH@Z ENDP			; KIpoTree::CanLighting
_TEXT	ENDS
PUBLIC	?AddBuildinLight@KIpoTree@@QAEXPAUKBuildInLightInfo@@H@Z ; KIpoTree::AddBuildinLight
PUBLIC	__real@4@4005af00000000000000
EXTRN	__imp_?g_Random@@YAII@Z:NEAR
;	COMDAT __real@4@4005af00000000000000
; File C:\Program Files\Microsoft Visual Studio\VC98\INCLUDE\list
CONST	SEGMENT
__real@4@4005af00000000000000 DD 042af0000r	; 87.5
CONST	ENDS
;	COMDAT ?AddBuildinLight@KIpoTree@@QAEXPAUKBuildInLightInfo@@H@Z
_TEXT	SEGMENT
_pLights$ = 8
_nCount$ = 12
_this$ = -20
_nMinRange$ = -12
_nMaxRange$ = 12
?AddBuildinLight@KIpoTree@@QAEXPAUKBuildInLightInfo@@H@Z PROC NEAR ; KIpoTree::AddBuildinLight, COMDAT

; 1531 : {

	sub	esp, 20					; 00000014H

; 1532 : 	if(!m_bProcessBioLights)

	mov	al, BYTE PTR [ecx+89]
	mov	DWORD PTR _this$[esp+20], ecx
	test	al, al
	je	$L89049

; 1533 : 		return;
; 1534 : 
; 1535 : 	int nMinRange, nMaxRange;
; 1536 : 
; 1537 : 	for(int i=0; i<nCount; i++)

	mov	eax, DWORD PTR _nCount$[esp+16]
	test	eax, eax
	jle	$L89049
	mov	ecx, DWORD PTR _pLights$[esp+16]
	push	ebx
	push	ebp
	push	esi
	push	edi
	lea	edi, DWORD PTR [ecx+20]
	mov	DWORD PTR 8+[esp+32], eax
$L89047:

; 1538 : 	{
; 1539 : 		// 增加一个光源
; 1540 : 		KSelfBreathLight *pLight = new KSelfBreathLight;

	push	52					; 00000034H
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	test	eax, eax
	je	$L89048
	mov	ecx, eax
	call	??0KSelfBreathLight@@QAE@XZ		; KSelfBreathLight::KSelfBreathLight
	mov	esi, eax

; 1541 : 		if(pLight)

	test	esi, esi
	je	$L89048

; 1542 : 		{
; 1543 : 			// 保证nMaxRange大于nMinRange
; 1544 : 			if(pLights[i].nMaxRange >= pLights[i].nMinRange)

	mov	ecx, DWORD PTR [edi]
	mov	edx, DWORD PTR [edi-4]
	cmp	ecx, edx
	jl	SHORT $L89056

; 1545 : 				nMaxRange = pLights[i].nMaxRange, nMinRange = pLights[i].nMinRange;

	mov	eax, ecx
	mov	ecx, edx
	mov	DWORD PTR _nMaxRange$[esp+32], eax

; 1546 : 			else

	jmp	SHORT $L89057
$L89056:

; 1547 : 				nMaxRange = pLights[i].nMinRange, nMinRange = pLights[i].nMaxRange;

	mov	eax, edx
	mov	DWORD PTR _nMaxRange$[esp+32], eax
$L89057:

; 1548 : 			// 半径为负值，忽略这个光源
; 1549 : 			if(nMinRange < 0)

	test	ecx, ecx
	jl	$L89048

; 1550 : 			{
; 1551 : 				_ASSERT(0);
; 1552 : 				continue;
; 1553 : 			}
; 1554 : 
; 1555 : 			nMinRange = nMaxRange - 8;
; 1556 : 
; 1557 : 			pLight->m_dwColor = pLights[i].dwColor;

	mov	edx, DWORD PTR [edi-8]

; 1558 : 			pLight->m_oPosition.nX = pLights[i].oPos.x;
; 1559 : 			pLight->m_oPosition.nY = pLights[i].oPos.y;
; 1560 : 			pLight->m_oPosition.nZ = pLights[i].oPos.z;
; 1561 : 			pLight->m_nRadius = nMinRange + g_Random(nMaxRange - nMinRange);

	mov	ebx, DWORD PTR __imp_?g_Random@@YAII@Z
	mov	DWORD PTR [esi+20], edx
	mov	ecx, DWORD PTR [edi-20]
	mov	DWORD PTR [esi+8], ecx
	mov	edx, DWORD PTR [edi-16]
	lea	ebp, DWORD PTR [eax-8]
	mov	DWORD PTR [esi+12], edx
	mov	ecx, DWORD PTR [edi-12]
	sub	eax, ebp
	push	eax
	mov	DWORD PTR _nMinRange$[esp+40], ebp
	mov	DWORD PTR [esi+16], ecx
	call	ebx
	add	eax, ebp

; 1562 : 			pLight->fRadius = (float)(pLight->m_nRadius);
; 1563 : 			pLight->m_pParent = NULL;

	xor	ebp, ebp
	mov	DWORD PTR -16+[esp+40], eax

; 1564 : 			pLight->fMaxRange = (float)nMaxRange;
; 1565 : 			pLight->fMinRange = (float)nMinRange;
; 1566 : 			pLight->fCycle = 100 * 0.875f + g_Random(60 / 4);

	push	15					; 0000000fH
	fild	DWORD PTR -16+[esp+44]
	mov	DWORD PTR [esi+24], eax
	mov	DWORD PTR [esi+4], ebp
	fstp	DWORD PTR [esi+36]
	fild	DWORD PTR _nMaxRange$[esp+40]
	fstp	DWORD PTR [esi+32]
	fild	DWORD PTR _nMinRange$[esp+44]
	fstp	DWORD PTR [esi+28]
	call	ebx
	mov	DWORD PTR -8+[esp+44], eax
	mov	DWORD PTR -8+[esp+48], ebp
	fild	QWORD PTR -8+[esp+44]

; 1567 : 			m_LightList.push_back(pLight);

	mov	edx, DWORD PTR _this$[esp+44]
	push	12					; 0000000cH
	fadd	DWORD PTR __real@4@4005af00000000000000
	fstp	DWORD PTR [esi+40]
	mov	ebp, DWORD PTR [edx+92276]
	mov	ebx, DWORD PTR [ebp+4]
	call	??2@YAPAXI@Z				; operator new
	add	esp, 12					; 0000000cH
	mov	ecx, ebp
	test	ebp, ebp
	jne	SHORT $L91572
	mov	ecx, eax
$L91572:
	test	ebx, ebx
	mov	DWORD PTR [eax], ecx
	jne	SHORT $L91574
	mov	ebx, eax
$L91574:
	mov	DWORD PTR [eax+4], ebx
	mov	DWORD PTR [ebp+4], eax
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx], eax
	add	eax, 8
	test	eax, eax
	je	SHORT $L91599
	mov	DWORD PTR [eax], esi
$L91599:
	mov	eax, DWORD PTR _this$[esp+36]
	inc	DWORD PTR [eax+92280]
$L89048:
	mov	eax, DWORD PTR 8+[esp+32]
	add	edi, 28					; 0000001cH
	dec	eax
	mov	DWORD PTR 8+[esp+32], eax
	jne	$L89047
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx
$L89049:

; 1568 : 		}
; 1569 : 	}
; 1570 : }

	add	esp, 20					; 00000014H
	ret	8
?AddBuildinLight@KIpoTree@@QAEXPAUKBuildInLightInfo@@H@Z ENDP ; KIpoTree::AddBuildinLight
_TEXT	ENDS
PUBLIC	?EnableBioLights@KIpoTree@@QAEX_N@Z		; KIpoTree::EnableBioLights
;	COMDAT ?EnableBioLights@KIpoTree@@QAEX_N@Z
_TEXT	SEGMENT
_bEnable$ = 8
?EnableBioLights@KIpoTree@@QAEX_N@Z PROC NEAR		; KIpoTree::EnableBioLights, COMDAT

; 1574 : 	m_bProcessBioLights = bEnable;

	mov	al, BYTE PTR _bEnable$[esp-4]
	push	esi
	push	edi
	mov	edi, ecx

; 1575 : 
; 1576 : 	if(!bEnable)

	test	al, al
	mov	BYTE PTR [edi+89], al
	jne	SHORT $L89075

; 1577 : 	{
; 1578 : 		//清除所有内建对象的光源
; 1579 : 		list<KLightBase*>::iterator i;
; 1580 : 		for (i = m_LightList.begin(); i != m_LightList.end(); )

	mov	eax, DWORD PTR [edi+92276]
	mov	esi, DWORD PTR [eax]
	cmp	esi, eax
	je	SHORT $L89075
	push	ebx
$L89074:

; 1581 : 		{
; 1582 : 			if((*i)->m_pParent == NULL)

	mov	eax, DWORD PTR [esi+8]
	mov	ecx, DWORD PTR [eax+4]
	test	ecx, ecx
	jne	SHORT $L89076

; 1583 : 			{
; 1584 : 				delete (*i);

	push	eax
	call	??3@YAXPAX@Z				; operator delete

; 1585 : 				i = m_LightList.erase(i);

	mov	eax, DWORD PTR [esi+4]
	mov	ebx, DWORD PTR [esi]
	push	esi
	mov	DWORD PTR [eax], ebx
	mov	ecx, DWORD PTR [esi]
	mov	edx, DWORD PTR [esi+4]
	mov	DWORD PTR [ecx+4], edx
	call	??3@YAXPAX@Z				; operator delete
	mov	eax, DWORD PTR [edi+92280]
	add	esp, 8
	dec	eax
	mov	esi, ebx
	mov	DWORD PTR [edi+92280], eax

; 1586 : 				continue;

	jmp	SHORT $L91794
$L89076:

; 1587 : 			}
; 1588 : 			++i;

	mov	esi, DWORD PTR [esi]
$L91794:
	cmp	esi, DWORD PTR [edi+92276]
	jne	SHORT $L89074
	pop	ebx
$L89075:
	pop	edi
	pop	esi

; 1589 : 		}
; 1590 : 	}
; 1591 : }

	ret	4
?EnableBioLights@KIpoTree@@QAEX_N@Z ENDP		; KIpoTree::EnableBioLights
_TEXT	ENDS
PUBLIC	?EnableDynamicLights@KIpoTree@@QAEX_N@Z		; KIpoTree::EnableDynamicLights
;	COMDAT ?EnableDynamicLights@KIpoTree@@QAEX_N@Z
_TEXT	SEGMENT
_bEnable$ = 8
?EnableDynamicLights@KIpoTree@@QAEX_N@Z PROC NEAR	; KIpoTree::EnableDynamicLights, COMDAT

; 1595 : 	m_bDynamicLighting = bEnable;

	mov	al, BYTE PTR _bEnable$[esp-4]
	mov	BYTE PTR [ecx+90], al

; 1596 : }

	ret	4
?EnableDynamicLights@KIpoTree@@QAEX_N@Z ENDP		; KIpoTree::EnableDynamicLights
_TEXT	ENDS
;	COMDAT ??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ
_TEXT	SEGMENT
??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ PROC NEAR ; std::list<KLightBase *,std::allocator<KLightBase *> >::~list<KLightBase *,std::allocator<KLightBase *> >, COMDAT

; 151  : 		{erase(begin(), end());

	push	ebx
	push	esi
	push	edi
	mov	edi, ecx
	mov	ebx, DWORD PTR [edi+4]
	mov	esi, DWORD PTR [ebx]
	cmp	esi, ebx
	je	SHORT $L91974
$L91973:
	mov	eax, esi
	mov	esi, DWORD PTR [esi]
	push	eax
	mov	ecx, DWORD PTR [eax+4]
	mov	edx, DWORD PTR [eax]
	mov	DWORD PTR [ecx], edx
	mov	ecx, DWORD PTR [eax]
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+4], edx
	call	??3@YAXPAX@Z				; operator delete
	mov	ecx, DWORD PTR [edi+8]
	add	esp, 4
	dec	ecx
	cmp	esi, ebx
	mov	DWORD PTR [edi+8], ecx
	jne	SHORT $L91973
$L91974:

; 152  : 		_Freenode(_Head);

	mov	eax, DWORD PTR [edi+4]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4

; 153  : 		_Head = 0, _Size = 0; }

	xor	eax, eax
	mov	DWORD PTR [edi+4], eax
	mov	DWORD PTR [edi+8], eax
	pop	edi
	pop	esi
	pop	ebx
	ret	0
??1?$list@PAVKLightBase@@V?$allocator@PAVKLightBase@@@std@@@std@@QAE@XZ ENDP ; std::list<KLightBase *,std::allocator<KLightBase *> >::~list<KLightBase *,std::allocator<KLightBase *> >
_TEXT	ENDS
END
