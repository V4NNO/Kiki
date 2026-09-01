/*
 * tlsservice.exe - pseudocod decompilat cu Ghidra MCP
 * Data analizei: 2026-09-01
 *
 * IMPORTANT:
 * - Acesta nu este codul-sursa original și nu este garantat compilabil.
 * - Tipurile undefined*, numele FUN_/DAT_/LAB_ și prototipurile sunt cele
 *   produse de decompilatorul Ghidra.
 * - Sunt incluse numai funcții existente în programul analizat; nu au fost
 *   inventate funcții și nu au fost rescrise implementări.
 * - Selecția acoperă pornirea aplicației și subsistemele relevante pentru
 *   profilul tlsservice: focus/input, desktop și captură de ecran.
 */

// =============================================================================
// Ghidra: entry @ 0x14000d32d
// =============================================================================
void entry(void)

{
  __security_init_cookie();
  FUN_1401900a0();
  return;
}

// =============================================================================
// Ghidra: FUN_1401900a0 @ 0x1401900a0
// =============================================================================
int FUN_1401900a0(void)

{
  code *pcVar1;
  bool bVar2;
  char cVar3;
  undefined1 uVar4;
  undefined2 uVar5;
  int iVar6;
  longlong *plVar7;
  undefined8 uVar8;
  
  cVar3 = __scrt_initialize_crt(1);
  if (cVar3 == '\0') {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  bVar2 = false;
  uVar4 = __scrt_acquire_startup_lock();
  if (DAT_1403703e8 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_1403703e8 == 0) {
    DAT_1403703e8 = 1;
    iVar6 = _initterm_e(&DAT_14026d9d0,&DAT_14026de10);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_14026c000,&DAT_14026d590);
    DAT_1403703e8 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_1401916ac();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_1401916b8();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1401edf00(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
  cVar3 = __scrt_is_managed_app();
  if (cVar3 != '\0') {
    if (!bVar2) {
      _cexit();
    }
    __scrt_uninitialize_crt(1,0);
    return iVar6;
  }
                    /* WARNING: Subroutine does not return */
  exit(iVar6);
}

// =============================================================================
// Ghidra: FUN_140030ae0 @ 0x140030ae0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140030ae0(int param_1,char **param_2)

{
  char *pcVar1;
  bool bVar2;
  char cVar3;
  bool bVar4;
  int iVar5;
  DWORD dwProcessId;
  undefined8 uVar6;
  undefined8 uVar7;
  QChar *pQVar8;
  longlong lVar9;
  QString *pQVar10;
  QVariant *this;
  QFileInfo *this_00;
  undefined1 *puVar11;
  basic_ostream<char,std::char_traits<char>_> *pbVar12;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar13;
  QTypedArrayData<unsigned_short> *pQVar14;
  ulonglong uVar15;
  uint uVar16;
  uint uVar17;
  undefined1 auStackY_2d8 [32];
  QTypedArrayData<unsigned_short> *local_2a8;
  QTypedArrayData<unsigned_short> *local_2a0;
  undefined8 local_298;
  undefined ***local_288;
  undefined8 uStack_280;
  QTypedArrayData<unsigned_short> *local_278;
  DWORD local_270 [2];
  int local_268 [2];
  longlong local_260;
  QString local_258 [12];
  int iStack_24c;
  undefined *local_248;
  undefined **local_240;
  basic_ostream<char,std::char_traits<char>_> local_238 [96];
  undefined8 local_1d8;
  undefined4 local_1d0;
  basic_ios<char,std::char_traits<char>_> local_1c0 [104];
  QChar local_158 [8];
  QTypedArrayData<unsigned_short> *local_150;
  QString local_148 [16];
  undefined ***local_138;
  DWORD DStack_130;
  undefined4 uStack_12c;
  undefined8 local_128;
  undefined8 local_120;
  QSettings local_118 [16];
  undefined8 local_108;
  QApplication local_100 [24];
  undefined1 local_e8 [96];
  ulonglong local_88;
  QString local_80 [8];
  QString local_78 [8];
  ulonglong local_70;
  QString local_68 [8];
  QTypedArrayData<unsigned_short> *local_60 [3];
  ulonglong local_48;
  ulonglong local_40;
  
  local_108 = 0xfffffffffffffffe;
  local_40 = DAT_140364720 ^ (ulonglong)auStackY_2d8;
  bVar4 = false;
  local_298 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_298 & 0xffffffff00000000);
  local_268[0] = param_1;
  local_2a8 = QString::fromAscii_helper("KickidlerGrabber",0x10);
  QCoreApplication::setApplicationName((QString *)&local_2a8);
  QString::~QString((QString *)&local_2a8);
  local_60[0] = (QTypedArrayData<unsigned_short> *)&local_2a0;
  uVar6 = QFile::decodeName((char *)&local_2a0);
  uVar7 = thunk_FUN_14002f790();
  thunk_FUN_1400bc480(uVar7,1,uVar6);
  uVar6 = thunk_FUN_140030380(&local_2a0);
  thunk_FUN_1400bd5d0(uVar6);
  QString::~QString((QString *)&local_2a0);
  QString::QString(local_258);
  QString::QString((QString *)&local_260);
  if (local_268[0] == 3) {
    local_2a8 = QString::fromAscii_helper("-logging-postfix",0x10);
    bVar4 = true;
    local_298 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_298._4_4_,1);
    pcVar1 = param_2[1];
    iVar5 = *(int *)(local_2a8 + 4);
    pQVar8 = QString::constData((QString *)&local_2a8);
    iVar5 = QString::compare_helper(pQVar8,iVar5,pcVar1,-1,1);
    if (iVar5 == 0) {
      bVar2 = true;
      goto LAB_140030c21;
    }
  }
  bVar2 = false;
LAB_140030c21:
  if (bVar4) {
    local_298 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_298 & 0xffffffff00000000);
    QString::~QString((QString *)&local_2a8);
  }
  if (bVar2) {
    pcVar1 = param_2[2];
    if (pcVar1 == (char *)0x0) {
      lVar9 = 0xffffffff;
    }
    else {
      lVar9 = -1;
      do {
        lVar9 = lVar9 + 1;
      } while (pcVar1[lVar9] != '\0');
    }
    local_2a8 = QString::fromAscii_helper(pcVar1,(int)lVar9);
    QString::operator=((QString *)&local_260,(QString *)&local_2a8);
    QString::~QString((QString *)&local_2a8);
  }
  SetProcessShutdownParameters(0,0);
  local_270[0] = 0xffffffff;
  dwProcessId = GetCurrentProcessId();
  ProcessIdToSessionId(dwProcessId,local_270);
  local_138 = (undefined ***)0xffffffffffffffff;
  DStack_130 = local_270[0];
  local_2a8 = QString::fromAscii_helper("agent",5);
  pQVar10 = (QString *)thunk_FUN_14002d620(&local_278,&local_2a8,&DAT_14026ebdc);
  QChar::QChar(local_158,0x20);
  pQVar10 = (QString *)QString::arg(pQVar10,&local_2a0,local_270[0],0);
  QString::operator=(local_258,pQVar10);
  QString::~QString((QString *)&local_2a0);
  QString::~QString((QString *)&local_278);
  QString::~QString((QString *)&local_2a8);
  if (*(int *)(local_260 + 4) == 0) {
    local_2a8 = QString::fromAscii_helper("",0);
    pQVar10 = (QString *)&local_2a8;
    uVar16 = 2;
  }
  else {
    pQVar10 = (QString *)thunk_FUN_14002d6b0(&local_2a0,&DAT_14026ebe0,&local_260);
    uVar16 = 4;
  }
  local_298 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_298._4_4_,uVar16);
  QString::append(local_258,pQVar10);
  if ((uVar16 & 4) != 0) {
    uVar16 = uVar16 & 0xfffffffb;
    QString::~QString((QString *)&local_2a0);
  }
  if ((uVar16 & 2) != 0) {
    uVar16 = uVar16 & 0xfffffffd;
    QString::~QString((QString *)&local_2a8);
  }
  local_2a0 = QString::fromAscii_helper("_",1);
  local_278 = QString::fromAscii_helper(".",1);
  local_298 = QString::fromAscii_helper("_",1);
  local_2a8 = QString::fromAscii_helper(":",1);
  pQVar10 = QString::replace(local_258,(QString *)&local_2a8,(QString *)&local_298,1);
  QString::replace(pQVar10,(QString *)&local_278,(QString *)&local_2a0,1);
  QString::~QString((QString *)&local_2a8);
  QString::~QString((QString *)&local_298);
  QString::~QString((QString *)&local_278);
  QString::~QString((QString *)&local_2a0);
  uVar6 = thunk_FUN_140030380(&local_278,8);
  uVar6 = thunk_FUN_14002d620(&local_2a0,uVar6,&DAT_14026ebe8);
  thunk_FUN_14002d5b0(local_148,uVar6,local_258);
  QString::~QString((QString *)&local_2a0);
  QString::~QString((QString *)&local_278);
  uVar6 = QString::QString((QString *)&local_2a8,local_148);
  thunk_FUN_1400c63a0(uVar6,0xa00000,10);
  qInstallMessageHandler((_func_void_QtMsgType_QMessageLogContext_ptr_QString_ptr *)&LAB_1400057c2);
  QApplication::QApplication(local_100,local_268,param_2,0x50602);
  uVar6 = thunk_FUN_1400c5db0(&local_288);
  cVar3 = thunk_FUN_1400c5970(uVar6,20000);
  thunk_FUN_1400c47d0(&local_288);
  iVar5 = 4;
  uVar17 = uVar16;
  if (cVar3 != '\0') {
    local_248 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1c0)
    ;
    uVar17 = uVar16 | 8;
    local_298 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_298._4_4_,uVar16) | 8);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_248,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_240,false);
    *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
         *(int *)(local_248 + 4) + -0x88;
    local_60[0] = (QTypedArrayData<unsigned_short> *)&local_240;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
    local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1d8 = 0;
    local_1d0 = 4;
    thunk_FUN_14002c8b0(&local_248,"start agent");
    uVar6 = thunk_FUN_140030720(&local_248,local_60);
    uVar7 = thunk_FUN_1400c5db0(&local_288);
    thunk_FUN_1400c4fa0(uVar7,20000,uVar6);
    thunk_FUN_1400c47d0(&local_288);
    if (0xf < local_48) {
      pQVar14 = local_60[0];
      if (0xfff < local_48 + 1) {
        if (((ulonglong)local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        pQVar14 = *(QTypedArrayData<unsigned_short> **)(local_60[0] + -8);
        if (local_60[0] <= pQVar14) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_60[0] - (longlong)pQVar14) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_60[0] - (longlong)pQVar14)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(pQVar14);
    }
    *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
         *(int *)(local_248 + 4) + -0x88;
    local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e320(&local_240);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_238);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_1c0);
  }
  uVar6 = thunk_FUN_140030380(&local_2a8,10);
  pQVar10 = (QString *)thunk_FUN_14002d620(&local_278,uVar6,"/grabber.ini");
  QSettings::QSettings(local_118,pQVar10,1,(QObject *)0x0);
  QString::~QString((QString *)&local_278);
  QString::~QString((QString *)&local_2a8);
  local_2a0 = QString::fromAscii_helper("misc/stealthMode",0x10);
  QVariant::QVariant((QVariant *)local_60);
  this = (QVariant *)QSettings::value(local_118,(QString *)&local_288,(QVariant *)&local_2a0);
  pQVar10 = (QString *)QVariant::toString(this);
  QString::operator==(pQVar10,"yes");
  QString::~QString((QString *)&local_298);
  QVariant::~QVariant((QVariant *)&local_288);
  QVariant::~QVariant((QVariant *)local_60);
  QString::~QString((QString *)&local_2a0);
  thunk_FUN_14004c590();
  local_150 = QString::fromAscii_helper("grabberSubAgent",0xf);
  pQVar10 = (QString *)QCoreApplication::applicationFilePath();
  this_00 = (QFileInfo *)QFileInfo::QFileInfo((QFileInfo *)&local_278,pQVar10);
  uVar6 = QFileInfo::fileName(this_00);
  lVar9 = thunk_FUN_14002ec90(&local_88,uVar6);
  QString::operator=((QString *)&local_150,(QString *)(lVar9 + 0x10));
  QString::~QString(local_68);
  QString::~QString((QString *)&local_70);
  QString::~QString(local_78);
  QString::~QString(local_80);
  QString::~QString((QString *)&local_88);
  QFileInfo::~QFileInfo((QFileInfo *)&local_278);
  QString::~QString((QString *)&local_2a8);
  local_298 = (QTypedArrayData<unsigned_short> *)&local_2a0;
  uVar6 = QString::QString((QString *)&local_2a0,(QString *)&local_260);
  uStack_280 = CONCAT44(uStack_12c,DStack_130);
  local_288 = local_138;
  uVar7 = QDir::currentPath();
  uVar7 = thunk_FUN_14002d620(&local_2a8,uVar7,&DAT_14026ebe8);
  uVar7 = thunk_FUN_14002d5b0(&local_278,uVar7,&local_150);
  thunk_FUN_14008b8c0(local_e8,uVar7,&local_288,uVar6);
  QString::~QString((QString *)&local_2a8);
  QString::~QString((QString *)local_60);
  cVar3 = thunk_FUN_14008dd20(local_e8);
  uVar16 = uVar17;
  if ((cVar3 == '\0') || (puVar11 = (undefined1 *)thunk_FUN_14008d9a0(), puVar11 != local_e8)) {
    uVar6 = thunk_FUN_1400c5db0(&local_288);
    cVar3 = thunk_FUN_1400c5970(uVar6,40000);
    thunk_FUN_1400c47d0(&local_288);
    if (cVar3 != '\0') {
      local_248 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1c0);
      uVar16 = uVar17 | 0x10;
      local_298 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_298._4_4_,uVar17) | 0x10);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_248,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_240,false);
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_288 = &local_240;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1d8 = 0;
      local_1d0 = 4;
      puVar11 = (undefined1 *)thunk_FUN_14008d9a0();
      bVar4 = (bool)thunk_FUN_14008dd20(local_e8);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002c8b0(&local_248,"Failed to create sub agent IO routines: isValid = ")
      ;
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,bVar4);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002c8b0(pbVar13,", is singletone = ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,puVar11 == local_e8);
      uVar6 = thunk_FUN_140030720(&local_248,&local_88);
      uVar7 = thunk_FUN_1400c5db0(&local_288);
      thunk_FUN_1400c4fa0(uVar7,40000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp")
      ;
      thunk_FUN_1400c47d0(&local_288);
      if (0xf < local_70) {
        uVar15 = local_88;
        if (0xfff < local_70 + 1) {
          if ((local_88 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_88 - 8);
          if (local_88 <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_88 - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_88 - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar15);
      }
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_240);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_238);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1c0);
    }
  }
  uVar6 = thunk_FUN_1400c5db0(&local_288);
  cVar3 = thunk_FUN_1400c5970(uVar6,20000);
  thunk_FUN_1400c47d0(&local_288);
  uVar17 = uVar16;
  if (cVar3 != '\0') {
    local_248 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1c0)
    ;
    uVar17 = uVar16 | 0x20;
    local_298 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_298._4_4_,uVar16) | 0x20);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_248,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_240,false);
    *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
         *(int *)(local_248 + 4) + -0x88;
    local_288 = &local_240;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
    local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1d8 = 0;
    local_1d0 = 4;
    thunk_FUN_14002c8b0(&local_248,"This build using brand new subAgentIo system!");
    uVar6 = thunk_FUN_140030720(&local_248,&local_88);
    uVar7 = thunk_FUN_1400c5db0(&local_288);
    thunk_FUN_1400c4fa0(uVar7,20000,uVar6,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c47d0(&local_288);
    if (0xf < local_70) {
      uVar15 = local_88;
      if (0xfff < local_70 + 1) {
        if ((local_88 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar15 = *(ulonglong *)(local_88 - 8);
        if (local_88 <= uVar15) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_88 - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_88 - uVar15) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar15);
    }
    *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
         *(int *)(local_248 + 4) + -0x88;
    local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e320(&local_240);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_238);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_1c0);
  }
  thunk_FUN_140032590(&local_128);
  cVar3 = thunk_FUN_140035b40(local_128);
  if (cVar3 == '\0') {
    uVar6 = thunk_FUN_1400c5db0(&local_288);
    cVar3 = thunk_FUN_1400c5970(uVar6,20000);
    thunk_FUN_1400c47d0(&local_288);
    if (cVar3 != '\0') {
      local_248 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1c0);
      local_298 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_298._4_4_,uVar17) | 0x40);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_248,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_240,false);
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_288 = &local_240;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1d8 = 0;
      local_1d0 = 4;
      thunk_FUN_14002c8b0(&local_248,"communicator initialization failed");
      uVar6 = thunk_FUN_140030720(&local_248,&local_88);
      uVar7 = thunk_FUN_1400c5db0(&local_288);
      thunk_FUN_1400c4fa0(uVar7,20000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp")
      ;
      thunk_FUN_1400c47d0(&local_288);
      if (0xf < local_70) {
        uVar15 = local_88;
        if (0xfff < local_70 + 1) {
          if ((local_88 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_88 - 8);
          if (local_88 <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_88 - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_88 - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar15);
      }
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_240);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_238);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1c0);
    }
  }
  else {
    iVar5 = QApplication::exec();
    uVar6 = thunk_FUN_1400c5db0(&local_288);
    cVar3 = thunk_FUN_1400c5970(uVar6,20000);
    thunk_FUN_1400c47d0(&local_288);
    if (cVar3 != '\0') {
      local_248 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1c0);
      local_298 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_298._4_4_,uVar17) | 0x80);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_248,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_240,false);
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_288 = &local_240;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1d8 = 0;
      local_1d0 = 4;
      thunk_FUN_14002c8b0(&local_248,&DAT_14026ece4);
      uVar6 = thunk_FUN_140030720(&local_248,&local_88);
      uVar7 = thunk_FUN_1400c5db0(&local_288);
      thunk_FUN_1400c4fa0(uVar7,20000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp")
      ;
      thunk_FUN_1400c47d0(&local_288);
      if (0xf < local_70) {
        uVar15 = local_88;
        if (0xfff < local_70 + 1) {
          if ((local_88 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_88 - 8);
          if (local_88 <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_88 - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_88 - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar15);
      }
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_240);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_238);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1c0);
    }
  }
  thunk_FUN_14002e9e0(local_120);
  thunk_FUN_14008c5b0(local_e8);
  QString::~QString((QString *)&local_150);
  QSettings::~QSettings(local_118);
  QApplication::~QApplication(local_100);
  QString::~QString(local_148);
  QString::~QString((QString *)&local_260);
  QString::~QString(local_258);
  return iVar5;
}

// =============================================================================
// Ghidra: FUN_14002ec90 @ 0x14002ec90
// =============================================================================
undefined8 * FUN_14002ec90(undefined8 *param_1,QString *param_2)

{
  int iVar1;
  QString *pQVar2;
  QTypedArrayData<unsigned_short> *pQVar3;
  QString local_res18 [8];
  
  pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026e968);
  iVar1 = QString::compare(param_2,pQVar2,0);
  QString::~QString(local_res18);
  if (iVar1 != 0) {
    pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026e978);
    iVar1 = QString::compare(param_2,pQVar2,0);
    QString::~QString(local_res18);
    if (iVar1 != 0) {
      pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026e988);
      iVar1 = QString::compare(param_2,pQVar2,0);
      QString::~QString(local_res18);
      if (iVar1 != 0) {
        pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026ea10);
        iVar1 = QString::compare(param_2,pQVar2,0);
        QString::~QString(local_res18);
        if (iVar1 != 0) {
          pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026ea20);
          iVar1 = QString::compare(param_2,pQVar2,0);
          QString::~QString(local_res18);
          if (iVar1 != 0) {
            pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026ea30);
            iVar1 = QString::compare(param_2,pQVar2,0);
            QString::~QString(local_res18);
            if (iVar1 != 0) {
              pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026ea78);
              iVar1 = QString::compare(param_2,pQVar2,0);
              QString::~QString(local_res18);
              if (iVar1 != 0) {
                pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026ea88);
                iVar1 = QString::compare(param_2,pQVar2,0);
                QString::~QString(local_res18);
                if (iVar1 != 0) {
                  pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026eaa0);
                  iVar1 = QString::compare(param_2,pQVar2,0);
                  QString::~QString(local_res18);
                  if (iVar1 != 0) {
                    pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026eb10);
                    iVar1 = QString::compare(param_2,pQVar2,0);
                    QString::~QString(local_res18);
                    if (iVar1 != 0) {
                      pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026eb20);
                      iVar1 = QString::compare(param_2,pQVar2,0);
                      QString::~QString(local_res18);
                      if (iVar1 != 0) {
                        pQVar2 = (QString *)QString::fromLatin1((char *)local_res18,0x4026eb38);
                        iVar1 = QString::compare(param_2,pQVar2,0);
                        QString::~QString(local_res18);
                        if (iVar1 != 0) {
                          pQVar3 = QString::fromAscii_helper("grabber",7);
                          *param_1 = pQVar3;
                          pQVar3 = QString::fromAscii_helper("grabberAgent",0xc);
                          param_1[1] = pQVar3;
                          pQVar3 = QString::fromAscii_helper("grabberSubAgent",0xf);
                          param_1[2] = pQVar3;
                          pQVar3 = QString::fromAscii_helper("ngs",3);
                          param_1[3] = pQVar3;
                          pQVar3 = QString::fromAscii_helper("Tele Link Soft Helper Service",0x1d);
                          param_1[4] = pQVar3;
                          goto LAB_14002f1bc;
                        }
                      }
                    }
                    pQVar3 = QString::fromAscii_helper("grabber",7);
                    *param_1 = pQVar3;
                    pQVar3 = QString::fromAscii_helper("grabberAgent",0xc);
                    param_1[1] = pQVar3;
                    pQVar3 = QString::fromAscii_helper("grabberSubAgent",0xf);
                    param_1[2] = pQVar3;
                    pQVar3 = QString::fromAscii_helper("ngs",3);
                    param_1[3] = pQVar3;
                    pQVar3 = QString::fromAscii_helper("Tele Link Soft Helper Service",0x1d);
                    param_1[4] = pQVar3;
                    goto LAB_14002f1bc;
                  }
                }
              }
              pQVar3 = QString::fromAscii_helper("tlshost",7);
              *param_1 = pQVar3;
              pQVar3 = QString::fromAscii_helper("tlsservice",10);
              param_1[1] = pQVar3;
              pQVar3 = QString::fromAscii_helper("tlssubservice",0xd);
              param_1[2] = pQVar3;
              pQVar3 = QString::fromAscii_helper("tls",3);
              param_1[3] = pQVar3;
              pQVar3 = QString::fromAscii_helper("Telemetry Service Host",0x16);
              param_1[4] = pQVar3;
              goto LAB_14002f1bc;
            }
          }
        }
        pQVar3 = QString::fromAscii_helper("grabber2",8);
        *param_1 = pQVar3;
        pQVar3 = QString::fromAscii_helper("ngsAgent",8);
        param_1[1] = pQVar3;
        pQVar3 = QString::fromAscii_helper("ngsSubAgent",0xb);
        param_1[2] = pQVar3;
        pQVar3 = QString::fromAscii_helper("ngs",3);
        param_1[3] = pQVar3;
        pQVar3 = QString::fromAscii_helper("Tele Link Soft Helper Service",0x1d);
        param_1[4] = pQVar3;
        goto LAB_14002f1bc;
      }
    }
  }
  pQVar3 = QString::fromAscii_helper("svchost",7);
  *param_1 = pQVar3;
  pQVar3 = QString::fromAscii_helper("WmiPrvSE",8);
  param_1[1] = pQVar3;
  pQVar3 = QString::fromAscii_helper("RuntimeBroker",0xd);
  param_1[2] = pQVar3;
  pQVar3 = QString::fromAscii_helper("WmiProviderSE",0xd);
  param_1[3] = pQVar3;
  pQVar3 = QString::fromAscii_helper("Windows Management Provider Service",0x23);
  param_1[4] = pQVar3;
LAB_14002f1bc:
  QString::~QString(param_2);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_14002f830 @ 0x14002f830
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14002f830(int param_1,undefined8 param_2,QString *param_3)

{
  char cVar1;
  undefined8 uVar2;
  QByteArray *pQVar3;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_168 [32];
  undefined4 local_148;
  undefined4 local_138 [3];
  int iStack_12c;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  undefined8 local_38;
  ulonglong local_30 [3];
  ulonglong local_18;
  ulonglong local_10;
  
  local_38 = 0xfffffffffffffffe;
  local_10 = DAT_140364720 ^ (ulonglong)auStack_168;
  local_138[0] = 0;
  if (param_1 == 0) {
    uVar2 = thunk_FUN_1400c5db0(local_138);
    cVar1 = thunk_FUN_1400c5970(uVar2,10000);
    thunk_FUN_1400c47d0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002ce40(local_120,2);
    uVar2 = thunk_FUN_14002c8b0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002c8b0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002c8b0(uVar2,&DAT_14026e924);
    uVar2 = thunk_FUN_140030720(&local_128,local_30);
    uVar5 = thunk_FUN_1400c5db0(local_138);
    local_148 = 0x4e;
    thunk_FUN_1400c4fa0(uVar5,10000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c47d0(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar6);
    }
  }
  else if (param_1 == 1) {
    uVar2 = thunk_FUN_1400c5db0(local_138);
    cVar1 = thunk_FUN_1400c5970(uVar2,30000);
    thunk_FUN_1400c47d0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002ce40(local_120,2);
    uVar2 = thunk_FUN_14002c8b0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002c8b0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002c8b0(uVar2,&DAT_14026e924);
    uVar2 = thunk_FUN_140030720(&local_128,local_30);
    uVar5 = thunk_FUN_1400c5db0(local_138);
    local_148 = 0x51;
    thunk_FUN_1400c4fa0(uVar5,30000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c47d0(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar6);
    }
  }
  else if (param_1 == 2) {
    uVar2 = thunk_FUN_1400c5db0(local_138);
    cVar1 = thunk_FUN_1400c5970(uVar2,40000);
    thunk_FUN_1400c47d0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002ce40(local_120,2);
    uVar2 = thunk_FUN_14002c8b0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002c8b0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002c8b0(uVar2,&DAT_14026e924);
    uVar2 = thunk_FUN_140030720(&local_128,local_30);
    uVar5 = thunk_FUN_1400c5db0(local_138);
    local_148 = 0x54;
    thunk_FUN_1400c4fa0(uVar5,40000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c47d0(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar6);
    }
  }
  else if (param_1 == 3) {
    uVar2 = thunk_FUN_1400c5db0(local_138);
    cVar1 = thunk_FUN_1400c5970(uVar2,50000);
    thunk_FUN_1400c47d0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002ce40(local_120,2);
    uVar2 = thunk_FUN_14002c8b0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002c8b0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002c8b0(uVar2,&DAT_14026e924);
    uVar2 = thunk_FUN_140030720(&local_128,local_30);
    uVar5 = thunk_FUN_1400c5db0(local_138);
    local_148 = 0x57;
    thunk_FUN_1400c4fa0(uVar5,50000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c47d0(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar6);
    }
  }
  else {
    uVar2 = thunk_FUN_1400c5db0(local_138);
    cVar1 = thunk_FUN_1400c5970(uVar2,20000);
    thunk_FUN_1400c47d0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 0x10;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002ce40(local_120,2);
    uVar2 = thunk_FUN_14002c8b0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002c8b0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002c8b0(uVar2,&DAT_14026e924);
    uVar2 = thunk_FUN_140030720(&local_128,local_30);
    uVar5 = thunk_FUN_1400c5db0(local_138);
    local_148 = 0x5a;
    thunk_FUN_1400c4fa0(uVar5,20000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c47d0(local_138);
    if (0xf < local_18) {
      uVar6 = local_30[0];
      if (0xfff < local_18 + 1) {
        if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_30[0] - 8);
        if (local_30[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_30[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_30[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar6);
    }
  }
  thunk_FUN_14002d0f0(local_a0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0);
  return;
}

// =============================================================================
// Ghidra: __dyn_tls_init @ 0x140220b24
// =============================================================================
/* Library Function - Single Match
    __dyn_tls_init
   
   Library: Visual Studio 2015 Release */

void __dyn_tls_init(undefined8 param_1,int param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (param_2 == 2) {
    for (puVar2 = &DAT_14026d6a8; puVar2 != (undefined8 *)&DAT_14026d8c0; puVar2 = puVar2 + 1) {
      pcVar1 = (code *)*puVar2;
      if (pcVar1 != (code *)0x0) {
        _guard_check_icall(pcVar1);
        (*pcVar1)();
      }
    }
  }
  return;
}

// =============================================================================
// Ghidra: tls_callback_0 @ 0x140011257
// =============================================================================
void tls_callback_0(undefined8 param_1,int param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (param_2 == 2) {
    for (puVar2 = &DAT_14026d6a8; puVar2 != (undefined8 *)&DAT_14026d8c0; puVar2 = puVar2 + 1) {
      pcVar1 = (code *)*puVar2;
      if (pcVar1 != (code *)0x0) {
        _guard_check_icall(pcVar1);
        (*pcVar1)();
      }
    }
  }
  return;
}

// =============================================================================
// Ghidra: tls_callback_1 @ 0x140220b9c
// =============================================================================
void tls_callback_1(undefined8 param_1,int param_2)

{
  longlong lVar1;
  code *pcVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  
  if ((param_2 == 3) || (param_2 == 0)) {
    lVar1 = *(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8);
    piVar3 = *(int **)(lVar1 + 0x130);
    while (piVar3 != (int *)0x0) {
      iVar4 = *piVar3 + -1;
      if (-1 < iVar4) {
        piVar5 = piVar3 + ((longlong)iVar4 + 2) * 2;
        do {
          pcVar2 = *(code **)piVar5;
          if (pcVar2 != (code *)0x0) {
            _guard_check_icall(pcVar2);
            (*pcVar2)();
          }
          piVar5 = piVar5 + -2;
          iVar4 = iVar4 + -1;
        } while (-1 < iVar4);
      }
      piVar5 = *(int **)(piVar3 + 2);
      if (piVar5 != (int *)0x0) {
        _free_base(piVar3);
      }
      *(int **)(lVar1 + 0x130) = piVar5;
      piVar3 = piVar5;
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14003e8e0 @ 0x14003e8e0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14003e8e0(longlong param_1)

{
  char cVar1;
  DWORD DVar2;
  HMODULE pHVar3;
  HHOOK pHVar4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *pbVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  uint uVar9;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  uint local_168;
  undefined ***local_160 [2];
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  undefined8 local_58;
  ulonglong local_50 [3];
  ulonglong local_38;
  ulonglong local_30;
  
  local_58 = 0xfffffffffffffffe;
  local_30 = DAT_140364720 ^ (ulonglong)auStack_198;
  local_168 = 0;
  pHVar3 = GetModuleHandleA((LPCSTR)0x0);
  pHVar4 = SetWindowsHookExA(0xd,thunk_FUN_1400405d0,pHVar3,0);
  *(HHOOK *)(param_1 + 0x78) = pHVar4;
  uVar9 = 0;
  if (pHVar4 == (HHOOK)0x0) {
    uVar5 = thunk_FUN_1400c5db0(local_160);
    cVar1 = thunk_FUN_1400c5970(uVar5,40000);
    thunk_FUN_1400c47d0();
    uVar9 = 0;
    if (cVar1 != '\0') {
      local_148 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      uVar9 = 1;
      local_168 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_160[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      DVar2 = GetLastError();
      pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_148,"SetWindowsHookEx (kbd): ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,DVar2);
      uVar5 = thunk_FUN_140030720(&local_148,local_50);
      uVar7 = thunk_FUN_1400c5db0(local_160);
      local_178 = 0x7c;
      thunk_FUN_1400c4fa0(uVar7,40000,uVar5);
      thunk_FUN_1400c47d0(local_160);
      if (0xf < local_38) {
        uVar8 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar8);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  pHVar3 = GetModuleHandleA((LPCSTR)0x0);
  pHVar4 = SetWindowsHookExA(0xe,thunk_FUN_1400405d0,pHVar3,0);
  *(HHOOK *)(param_1 + 0x80) = pHVar4;
  if (pHVar4 == (HHOOK)0x0) {
    uVar5 = thunk_FUN_1400c5db0(local_160);
    cVar1 = thunk_FUN_1400c5970(uVar5,40000);
    thunk_FUN_1400c47d0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = uVar9 | 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_160[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      DVar2 = GetLastError();
      pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_148,"SetWindowsHookEx (mice): ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,DVar2);
      uVar5 = thunk_FUN_140030720(&local_148,local_50);
      uVar7 = thunk_FUN_1400c5db0(local_160);
      local_178 = 0x86;
      thunk_FUN_1400c4fa0(uVar7,40000,uVar5,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTrackerImpl_win.cpp"
                         );
      thunk_FUN_1400c47d0(local_160);
      if (0xf < local_38) {
        uVar8 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar8);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14003ee60 @ 0x14003ee60
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14003ee60(longlong param_1)

{
  char cVar1;
  DWORD DVar2;
  BOOL BVar3;
  HMODULE hmodWinEventProc;
  HWINEVENTHOOK pHVar4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *this;
  ulonglong uVar6;
  undefined1 auStackY_1c8 [32];
  undefined ***local_180 [2];
  undefined8 local_170;
  undefined *local_168;
  undefined **local_160;
  basic_ostream<char,std::char_traits<char>_> local_158 [96];
  undefined8 local_f8;
  undefined4 local_f0;
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  tagMSG local_78;
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_170 = 0xfffffffffffffffe;
  local_28 = DAT_140364720 ^ (ulonglong)auStackY_1c8;
  thunk_FUN_14003e8e0();
  hmodWinEventProc = GetModuleHandleA((LPCSTR)0x0);
  pHVar4 = SetWinEventHook(0x20,0x8005,hmodWinEventProc,thunk_FUN_140040dd0,0,0,0);
  *(HWINEVENTHOOK *)(param_1 + 0x88) = pHVar4;
  if (pHVar4 == (HWINEVENTHOOK)0x0) {
    uVar5 = thunk_FUN_1400c5db0(local_180);
    cVar1 = thunk_FUN_1400c5970(uVar5);
    thunk_FUN_1400c47d0(local_180);
    if (cVar1 != '\0') {
      local_168 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_e0);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_168,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_160,false);
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_170 + (longlong)*(int *)(local_168 + 4) + 4) =
           *(int *)(local_168 + 4) + -0x88;
      local_180[0] = &local_160;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_f8 = 0;
      local_f0 = 4;
      DVar2 = GetLastError();
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002c8b0(&local_168,"SetWinEventHook: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,DVar2);
      thunk_FUN_140030720(&local_168,local_48);
      uVar5 = thunk_FUN_1400c5db0(local_180);
      thunk_FUN_1400c4fa0(uVar5,40000);
      thunk_FUN_1400c47d0(local_180);
      if (0xf < local_30) {
        uVar6 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar6);
      }
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_170 + (longlong)*(int *)(local_168 + 4) + 4) =
           *(int *)(local_168 + 4) + -0x88;
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_160);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_158);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_e0);
    }
  }
  do {
    BVar3 = GetMessageA(&local_78,(HWND)0x0,0,0);
  } while (BVar3 != 0);
  if (*(HHOOK *)(param_1 + 0x78) != (HHOOK)0x0) {
    UnhookWindowsHookEx(*(HHOOK *)(param_1 + 0x78));
    *(undefined8 *)(param_1 + 0x78) = 0;
  }
  if (*(HHOOK *)(param_1 + 0x80) != (HHOOK)0x0) {
    UnhookWindowsHookEx(*(HHOOK *)(param_1 + 0x80));
    *(undefined8 *)(param_1 + 0x80) = 0;
  }
  if (*(HWINEVENTHOOK *)(param_1 + 0x88) != (HWINEVENTHOOK)0x0) {
    UnhookWinEvent(*(HWINEVENTHOOK *)(param_1 + 0x88));
    *(undefined8 *)(param_1 + 0x88) = 0;
  }
  return 0;
}

// =============================================================================
// Ghidra: FUN_140040dd0 @ 0x140040dd0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140040dd0(undefined8 param_1,int param_2,undefined8 param_3)

{
  longlong lVar1;
  undefined1 auStack_88 [32];
  tagGUITHREADINFO local_68;
  ulonglong local_18;
  
  local_18 = DAT_140364720 ^ (ulonglong)auStack_88;
  if (param_2 == 0x8005) {
    lVar1 = thunk_FUN_1400404f0();
    local_68.cbSize = 0x48;
    local_68._4_8_ = 0;
    local_68.hwndActive._4_4_ = 0;
    local_68.hwndFocus._0_4_ = 0;
    local_68.hwndFocus._4_4_ = 0;
    local_68.hwndCapture._0_4_ = 0;
    local_68._28_8_ = 0;
    local_68._36_8_ = 0;
    local_68._44_8_ = 0;
    local_68._52_8_ = 0;
    local_68.rcCaret.top = 0;
    local_68.rcCaret.right = 0;
    local_68.rcCaret.bottom = 0;
    GetGUIThreadInfo(0,&local_68);
    *(ulonglong *)(lVar1 + 0x90) = CONCAT44(local_68.hwndFocus._4_4_,local_68.hwndFocus._0_4_);
    if (*(HHOOK *)(lVar1 + 0x78) != (HHOOK)0x0) {
      UnhookWindowsHookEx(*(HHOOK *)(lVar1 + 0x78));
      *(undefined8 *)(lVar1 + 0x78) = 0;
    }
    if (*(HHOOK *)(lVar1 + 0x80) != (HHOOK)0x0) {
      UnhookWindowsHookEx(*(HHOOK *)(lVar1 + 0x80));
      *(undefined8 *)(lVar1 + 0x80) = 0;
    }
  }
  else {
    if (param_2 != 0x20) {
      return;
    }
    lVar1 = thunk_FUN_1400404f0();
    *(undefined8 *)(lVar1 + 0x90) = param_3;
    if (*(HHOOK *)(lVar1 + 0x78) != (HHOOK)0x0) {
      UnhookWindowsHookEx(*(HHOOK *)(lVar1 + 0x78));
      *(undefined8 *)(lVar1 + 0x78) = 0;
    }
    if (*(HHOOK *)(lVar1 + 0x80) != (HHOOK)0x0) {
      UnhookWindowsHookEx(*(HHOOK *)(lVar1 + 0x80));
      *(undefined8 *)(lVar1 + 0x80) = 0;
    }
    thunk_FUN_14003fa20(lVar1);
  }
  thunk_FUN_14003e8e0(lVar1);
  return;
}

// =============================================================================
// Ghidra: FUN_1400405d0 @ 0x1400405d0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400405d0(int param_1,ulonglong param_2,UINT *param_3)

{
  bool bVar1;
  BYTE *pBVar2;
  SHORT SVar3;
  BOOL BVar4;
  DWORD idThread;
  int iVar5;
  longlong lVar6;
  QDateTime *pQVar7;
  HWND hWnd;
  HKL dwhkl;
  QString *pQVar8;
  UINT UVar9;
  UINT wVirtKey;
  undefined8 uVar10;
  uint uVar11;
  BYTE *lpKeyState;
  undefined8 uVar13;
  ulonglong uVar14;
  undefined1 auStackY_118 [32];
  undefined8 local_d8;
  undefined8 local_d0;
  tagGUITHREADINFO local_c8;
  tagRECT local_78;
  QString local_68 [8];
  WCHAR local_60 [16];
  ulonglong local_40;
  ulonglong uVar12;
  
  local_d0 = 0xfffffffffffffffe;
  local_40 = DAT_140364720 ^ (ulonglong)auStackY_118;
  lVar6 = thunk_FUN_1400404f0();
  uVar12 = 0;
  if (DAT_14036f290 == (BYTE *)0x0) {
    DAT_14036f290 = operator_new(0x100);
    uVar14 = uVar12;
    do {
      SVar3 = GetKeyState((int)uVar12);
      DAT_14036f290[uVar14] = (byte)((ushort)SVar3 >> 8) & 0x80 | (byte)SVar3 & 1;
      uVar11 = (int)uVar12 + 1;
      uVar12 = (ulonglong)uVar11;
      uVar14 = uVar14 + 1;
    } while ((int)uVar11 < 0x100);
    DAT_14036f298 = operator_new(10);
  }
  if (param_1 != 0) goto LAB_140040aa9;
  if (param_2 < 0x202) {
    if (param_2 != 0x201) {
      if (param_2 < 0x100) goto LAB_140040aa9;
      if (0x101 < param_2) {
        if (param_2 < 0x104) goto LAB_140040aa9;
        if (param_2 < 0x106) goto LAB_1400406dc;
        if (param_2 != 0x200) goto LAB_140040aa9;
        uVar10 = 0;
        goto LAB_140040a8f;
      }
LAB_1400406dc:
      if ((param_2 - 0x101 & 0xfffffffffffffffb) == 0) {
        local_d8 = 0xffffffffffffffff;
        local_c8.cbSize = 0x48;
        BVar4 = GetGUIThreadInfo(0,&local_c8);
        if ((BVar4 != 0) && (BVar4 = GetWindowRect(local_c8.hwndFocus,&local_78), BVar4 != 0)) {
          local_d8 = CONCAT44((local_78.top + local_78.bottom) / 2,
                              (local_78.right + local_78.left) / 2);
        }
        thunk_FUN_14003d660(lVar6,1,&local_d8);
      }
      QDateTime::QDateTime((QDateTime *)&local_78);
      QString::QString(local_68);
      UVar9 = *param_3;
      if ((param_2 - 0x100 & 0xfffffffffffffffb) == 0) {
        if (param_3[1] == DAT_14036413c) {
          local_78.right = 2;
          switch(UVar9) {
          case 9:
          case 0x10:
          case 0x11:
          case 0x12:
          case 0x13:
          case 0x14:
          case 0x2a:
          case 0x2c:
          case 0x5b:
          case 0x5c:
          case 0x5d:
          case 0x5f:
          case 0x90:
          case 0x91:
          case 0xa0:
          case 0xa1:
          case 0xa2:
          case 0xa3:
          case 0xa4:
          case 0xa5:
            goto switchD_1400407b7_caseD_9;
          default:
            goto switchD_1400407b7_caseD_a;
          }
        }
        local_78.right = 1;
      }
      else {
        local_78.right = 3;
      }
switchD_1400407b7_caseD_a:
      if (UVar9 != 0xff) {
        local_78.bottom = thunk_FUN_140107c40(UVar9);
        pQVar7 = (QDateTime *)QDateTime::currentDateTimeUtc();
        QDateTime::operator=((QDateTime *)&local_78,pQVar7);
        QDateTime::~QDateTime((QDateTime *)&local_d8);
        bVar1 = false;
        switch(*param_3) {
        case 8:
        case 9:
        case 0xd:
        case 0x1b:
          DAT_14036f288 = '\0';
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
          bVar1 = true;
        }
        if ((local_78.right != 3) && (!bVar1)) {
          hWnd = *(HWND *)(lVar6 + 0x90);
          if (hWnd == (HWND)0x0) {
            hWnd = GetForegroundWindow();
          }
          idThread = GetWindowThreadProcessId(hWnd,(LPDWORD)0x0);
          dwhkl = GetKeyboardLayout(idThread);
          FUN_14003e330(0);
          UVar9 = 4;
          if (*(char *)(lVar6 + 0x98) != '\0') {
            UVar9 = 0;
          }
          iVar5 = ToUnicodeEx(*param_3,param_3[1],DAT_14036f290,local_60,0x10,UVar9,dwhkl);
          if (0 < iVar5) {
            pQVar8 = (QString *)QString::fromUtf16((ushort *)&local_d8,(int)local_60);
            QString::operator=(local_68,pQVar8);
            QString::~QString((QString *)&local_d8);
          }
          pBVar2 = DAT_14036f298;
          lpKeyState = DAT_14036f290;
          if (*(char *)(lVar6 + 0x98) != '\0') {
            if (iVar5 < 0) {
              QString::clear(local_68);
              pBVar2 = DAT_14036f298;
              lpKeyState = DAT_14036f290;
              DAT_14036f288 = '\x01';
              DAT_14036f2b0 = *param_3;
              DAT_14036f2b4 = param_3[1];
              *DAT_14036f298 = DAT_14036f290[0x10];
              pBVar2[1] = lpKeyState[0xa0];
              pBVar2[2] = lpKeyState[0xa1];
              pBVar2[3] = lpKeyState[0x11];
              pBVar2[4] = lpKeyState[0xa2];
              pBVar2[5] = lpKeyState[0xa3];
              pBVar2[6] = lpKeyState[0x12];
              pBVar2[7] = lpKeyState[0xa4];
              pBVar2[8] = lpKeyState[0xa5];
              pBVar2[9] = lpKeyState[0x14];
              UVar9 = param_3[1];
              wVirtKey = *param_3;
            }
            else {
              if ((DAT_14036f288 == '\0') || (iVar5 < 1)) goto LAB_140040a2b;
              DAT_14036f288 = '\0';
              DAT_14036f290[0x10] = *DAT_14036f298;
              lpKeyState[0xa0] = pBVar2[1];
              lpKeyState[0xa1] = pBVar2[2];
              lpKeyState[0x11] = pBVar2[3];
              lpKeyState[0xa2] = pBVar2[4];
              lpKeyState[0xa3] = pBVar2[5];
              lpKeyState[0x12] = pBVar2[6];
              lpKeyState[0xa4] = pBVar2[7];
              lpKeyState[0xa5] = pBVar2[8];
              lpKeyState[0x14] = pBVar2[9];
              UVar9 = DAT_14036f2b4;
              wVirtKey = DAT_14036f2b0;
            }
            ToUnicodeEx(wVirtKey,UVar9,lpKeyState,local_60,0x10,0,dwhkl);
          }
        }
LAB_140040a2b:
        thunk_FUN_14003d5d0(lVar6,&local_78);
      }
switchD_1400407b7_caseD_9:
      QString::~QString(local_68);
      QDateTime::~QDateTime((QDateTime *)&local_78);
      if ((param_2 - 0x100 & 0xfffffffffffffffb) == 0) {
        DAT_14036413c = param_3[1];
      }
      else {
        DAT_14036413c = 0xffffffff;
      }
      goto LAB_140040aa9;
    }
LAB_140040a8a:
    uVar10 = 1;
LAB_140040a8f:
    uVar13 = 0;
  }
  else {
    if (param_2 == 0x204) goto LAB_140040a8a;
    if (param_2 != 0x20a) goto LAB_140040aa9;
    uVar13 = 1;
    uVar10 = 0;
  }
  local_d8 = *(undefined8 *)param_3;
  thunk_FUN_14003d760(lVar6,uVar10,&local_d8,uVar13);
LAB_140040aa9:
  CallNextHookEx((HHOOK)0x0,param_1,param_2,(LPARAM)param_3);
  return;
}

// =============================================================================
// Ghidra: FUN_140047d80 @ 0x140047d80
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140047d80(undefined8 *param_1)

{
  QString *pQVar1;
  longlong lVar2;
  Data *pDVar3;
  bool bVar4;
  char cVar5;
  DWORD DVar6;
  BOOL BVar7;
  HDESK hDesktop;
  HDESK hDesktop_00;
  undefined8 uVar8;
  basic_ostream<char,std::char_traits<char>_> *pbVar9;
  undefined8 uVar10;
  HWND pHVar11;
  ulonglong uVar12;
  basic_ios<char,std::char_traits<char>_> *this;
  longlong lVar13;
  longlong lVar14;
  QTypedArrayData<unsigned_short> **ppQVar15;
  QString *this_00;
  QMutex *pQVar16;
  QWaitCondition *this_01;
  undefined1 auStackY_7a8 [32];
  Data *local_770;
  undefined ***local_768;
  QMutexLocker local_760 [8];
  QTypedArrayData<unsigned_short> *local_758;
  QTypedArrayData<unsigned_short> *local_750;
  QTypedArrayData<unsigned_short> *local_748;
  QTypedArrayData<unsigned_short> *local_740;
  undefined8 *local_738;
  undefined8 local_730;
  undefined1 local_728 [16];
  undefined1 local_718 [16];
  undefined1 local_708 [16];
  undefined1 local_6f8 [16];
  undefined1 local_6e8 [16];
  undefined1 local_6d8 [16];
  undefined1 local_6c8 [16];
  undefined1 local_6b8 [16];
  undefined1 local_6a8 [16];
  undefined1 local_698 [12];
  int iStack_68c;
  undefined *local_688;
  undefined **local_680;
  basic_ostream<char,std::char_traits<char>_> local_678 [96];
  undefined8 local_618;
  undefined4 local_610;
  basic_ios<char,std::char_traits<char>_> local_600 [100];
  int iStack_59c;
  undefined *local_598;
  undefined **local_590;
  basic_ostream<char,std::char_traits<char>_> local_588 [96];
  undefined8 local_528;
  undefined4 local_520;
  basic_ios<char,std::char_traits<char>_> local_510 [100];
  int iStack_4ac;
  undefined *local_4a8;
  undefined **local_4a0;
  basic_ostream<char,std::char_traits<char>_> local_498 [96];
  undefined8 local_438;
  undefined4 local_430;
  basic_ios<char,std::char_traits<char>_> local_420 [100];
  int iStack_3bc;
  undefined *local_3b8;
  undefined **local_3b0;
  basic_ostream<char,std::char_traits<char>_> local_3a8 [96];
  undefined8 local_348;
  undefined4 local_340;
  basic_ios<char,std::char_traits<char>_> local_330 [100];
  int iStack_2cc;
  undefined *local_2c8;
  undefined **local_2c0;
  basic_ostream<char,std::char_traits<char>_> local_2b8 [96];
  undefined8 local_258;
  undefined4 local_250;
  basic_ios<char,std::char_traits<char>_> local_240 [104];
  ulonglong local_1d8 [3];
  ulonglong local_1c0;
  ulonglong local_1b8 [3];
  ulonglong local_1a0;
  ulonglong local_198 [3];
  ulonglong local_180;
  ulonglong local_178 [3];
  ulonglong local_160;
  ulonglong local_158 [3];
  ulonglong local_140;
  char local_138 [256];
  ulonglong local_38;
  
  local_730 = 0xfffffffffffffffe;
  local_38 = DAT_140364720 ^ (ulonglong)auStackY_7a8;
  pQVar16 = (QMutex *)(param_1 + 3);
  local_738 = param_1;
  QMutexLocker::QMutexLocker(local_760,(QBasicMutex *)pQVar16);
  this_01 = (QWaitCondition *)(param_1 + 4);
  do {
    QWaitCondition::wait(this_01,pQVar16,0xffffffff);
    if (*(char *)(param_1 + 2) != '\0') {
      QMutexLocker::~QMutexLocker(local_760);
      return 0;
    }
    DVar6 = GetCurrentThreadId();
    hDesktop = GetThreadDesktop(DVar6);
    hDesktop_00 = OpenInputDesktop(0,0,1);
    if (hDesktop_00 == (HDESK)0x0) {
      uVar8 = thunk_FUN_1400c5db0(local_6a8);
      cVar5 = thunk_FUN_1400c5970(uVar8,30000);
      thunk_FUN_1400c47d0(local_6a8);
      if (cVar5 != '\0') {
        local_2c8 = &DAT_14026e900;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_240);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_2c8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_2c0,false);
        *(undefined ***)((longlong)&local_2c8 + (longlong)*(int *)(local_2c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_2cc + (longlong)*(int *)(local_2c8 + 4)) =
             *(int *)(local_2c8 + 4) + -0x88;
        local_768 = &local_2c0;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2c0);
        local_2c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_258 = 0;
        local_250 = 4;
        DVar6 = GetLastError();
        pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002c8b0(&local_2c8,"(pe) OpenInputDesktop failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
        uVar8 = thunk_FUN_140030720(&local_2c8,local_158);
        uVar10 = thunk_FUN_1400c5db0(local_698);
        thunk_FUN_1400c4fa0(uVar10,30000,uVar8,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                           );
        thunk_FUN_1400c47d0(local_698);
        if (0xf < local_140) {
          uVar12 = local_158[0];
          if (0xfff < local_140 + 1) {
            if ((local_158[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar12 = *(ulonglong *)(local_158[0] - 8);
            if (local_158[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_158[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_158[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002dab(uVar12);
        }
        *(undefined ***)((longlong)&local_2c8 + (longlong)*(int *)(local_2c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_2cc + (longlong)*(int *)(local_2c8 + 4)) =
             *(int *)(local_2c8 + 4) + -0x88;
        local_2c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e320(&local_2c0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2c0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_2b8);
        this = local_240;
LAB_140048a2a:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this)
        ;
      }
    }
    else {
      BVar7 = GetUserObjectInformationA(hDesktop_00,2,local_138,0x100,(LPDWORD)&local_740);
      if (BVar7 == 0) {
        uVar8 = thunk_FUN_1400c5db0(local_6e8);
        cVar5 = thunk_FUN_1400c5970(uVar8,30000);
        thunk_FUN_1400c47d0(local_6e8);
        if (cVar5 != '\0') {
          local_4a8 = &DAT_14026e900;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_420);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_4a8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_4a0,false);
          *(undefined ***)((longlong)&local_4a8 + (longlong)*(int *)(local_4a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_4ac + (longlong)*(int *)(local_4a8 + 4)) =
               *(int *)(local_4a8 + 4) + -0x88;
          local_768 = &local_4a0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_4a0);
          local_4a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_438 = 0;
          local_430 = 4;
          DVar6 = GetLastError();
          pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(&local_4a8,"(pe) GetUserObjectInformation failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
          uVar8 = thunk_FUN_140030720(&local_4a8,local_198);
          uVar10 = thunk_FUN_1400c5db0(local_6d8);
          thunk_FUN_1400c4fa0(uVar10,30000,uVar8,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c47d0(local_6d8);
          if (0xf < local_180) {
            uVar12 = local_198[0];
            if (0xfff < local_180 + 1) {
              if ((local_198[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar12 = *(ulonglong *)(local_198[0] - 8);
              if (local_198[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_198[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_198[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002dab(uVar12);
          }
          *(undefined ***)((longlong)&local_4a8 + (longlong)*(int *)(local_4a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_4ac + (longlong)*(int *)(local_4a8 + 4)) =
               *(int *)(local_4a8 + 4) + -0x88;
          local_4a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14002e320(&local_4a0);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_4a0);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_498);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_420);
        }
      }
      else {
        lVar14 = -1;
        do {
          lVar13 = lVar14 + 1;
          lVar2 = lVar14 + 1;
          lVar14 = lVar13;
        } while (local_138[lVar2] != '\0');
        local_758 = QString::fromAscii_helper(local_138,(int)lVar13);
        lVar14 = -1;
        do {
          lVar13 = lVar14 + 1;
          lVar2 = lVar14 + 1;
          lVar14 = lVar13;
        } while ("Winlogon"[lVar2] != '\0');
        local_750 = QString::fromAscii_helper("Winlogon",(int)lVar13);
        lVar14 = -1;
        do {
          lVar13 = lVar14 + 1;
          lVar2 = lVar14 + 1;
          lVar14 = lVar13;
        } while ("Screen-saver"[lVar2] != '\0');
        local_748 = QString::fromAscii_helper("Screen-saver",(int)lVar13);
        local_770 = (Data *)shared_null_exref;
        if (*(int *)(shared_null_exref + 4) < 2) {
          if (*(uint *)shared_null_exref < 2) {
            QListData::realloc((QListData *)&local_770,2);
          }
          else {
            thunk_FUN_14003e780();
          }
        }
        ppQVar15 = &local_750;
        do {
          thunk_FUN_140039760(&local_770,ppQVar15);
          ppQVar15 = ppQVar15 + 1;
        } while (ppQVar15 != &local_740);
        bVar4 = QtPrivate::QStringList_contains((QStringList *)&local_770,(QString *)&local_758,1);
        pDVar3 = local_770;
        if (*(int *)local_770 == 0) {
LAB_140047f5c:
          this_00 = (QString *)(local_770 + ((longlong)*(int *)(local_770 + 0xc) + 2) * 8);
          pQVar1 = (QString *)(local_770 + ((longlong)*(int *)(local_770 + 8) + 2) * 8);
          while (pQVar1 != this_00) {
            this_00 = this_00 + -8;
            QString::~QString(this_00);
          }
          QListData::dispose(pDVar3);
          param_1 = local_738;
        }
        else if (*(int *)local_770 != -1) {
          LOCK();
          *(int *)local_770 = *(int *)local_770 + -1;
          UNLOCK();
          if (*(int *)local_770 == 0) goto LAB_140047f5c;
        }
        _eh_vector_destructor_iterator_(&local_750,8,2,~QString_exref);
        QString::~QString((QString *)&local_758);
        if (!bVar4) {
          BVar7 = SetThreadDesktop(hDesktop_00);
          if (BVar7 == 0) {
            uVar8 = thunk_FUN_1400c5db0(local_728);
            cVar5 = thunk_FUN_1400c5970(uVar8,30000);
            thunk_FUN_1400c47d0(local_728);
            if (cVar5 != '\0') {
              local_688 = &DAT_14026e900;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_600);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_688,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_680,false);
              *(undefined ***)((longlong)&local_688 + (longlong)*(int *)(local_688 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_68c + (longlong)*(int *)(local_688 + 4)) =
                   *(int *)(local_688 + 4) + -0x88;
              local_768 = &local_680;
              std::basic_streambuf<char,std::char_traits<char>_>::
              basic_streambuf<char,std::char_traits<char>_>
                        ((basic_streambuf<char,std::char_traits<char>_> *)&local_680);
              local_680 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                          vftable;
              local_618 = 0;
              local_610 = 4;
              DVar6 = GetLastError();
              uVar8 = thunk_FUN_14002c8b0(&local_688,"(pe) SetThreadDesktop to ");
              uVar8 = thunk_FUN_14002c8b0(uVar8,local_138);
              pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_14002c8b0(uVar8," failed: ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
              uVar8 = thunk_FUN_140030720(&local_688,local_1d8);
              uVar10 = thunk_FUN_1400c5db0(local_718);
              thunk_FUN_1400c4fa0(uVar10,30000,uVar8,
                                  "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                                 );
              thunk_FUN_1400c47d0(local_718);
              if (0xf < local_1c0) {
                uVar12 = local_1d8[0];
                if (0xfff < local_1c0 + 1) {
                  if ((local_1d8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  uVar12 = *(ulonglong *)(local_1d8[0] - 8);
                  if (local_1d8[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (local_1d8[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (0x27 < local_1d8[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                }
                FUN_140002dab(uVar12);
              }
              *(undefined ***)((longlong)&local_688 + (longlong)*(int *)(local_688 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_68c + (longlong)*(int *)(local_688 + 4)) =
                   *(int *)(local_688 + 4) + -0x88;
              local_680 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                          vftable;
              thunk_FUN_14002e320(&local_680);
              std::basic_streambuf<char,std::char_traits<char>_>::
              ~basic_streambuf<char,std::char_traits<char>_>
                        ((basic_streambuf<char,std::char_traits<char>_> *)&local_680);
              std::basic_ostream<char,std::char_traits<char>_>::
              ~basic_ostream<char,std::char_traits<char>_>(local_678);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_600);
            }
          }
          pHVar11 = GetForegroundWindow();
          *param_1 = pHVar11;
        }
        if (hDesktop != (HDESK)0x0) {
          BVar7 = SetThreadDesktop(hDesktop);
          if (BVar7 != 0) {
            pQVar16 = (QMutex *)(param_1 + 3);
            this_01 = (QWaitCondition *)(param_1 + 4);
            goto LAB_14004861f;
          }
          uVar8 = thunk_FUN_1400c5db0(local_708);
          cVar5 = thunk_FUN_1400c5970(uVar8,30000);
          thunk_FUN_1400c47d0(local_708);
          if (cVar5 != '\0') {
            local_598 = &DAT_14026e900;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_510);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_598,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_590,false);
            *(undefined ***)((longlong)&local_598 + (longlong)*(int *)(local_598 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_59c + (longlong)*(int *)(local_598 + 4)) =
                 *(int *)(local_598 + 4) + -0x88;
            local_768 = &local_590;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_590);
            local_590 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_528 = 0;
            local_520 = 4;
            DVar6 = GetLastError();
            pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14002c8b0(&local_598,"(pe) SetThreadDesktop(prevDesk) failed: ");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
            uVar8 = thunk_FUN_140030720(&local_598,local_1b8);
            uVar10 = thunk_FUN_1400c5db0(local_6f8);
            thunk_FUN_1400c4fa0(uVar10,30000,uVar8,
                                "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                               );
            thunk_FUN_1400c47d0(local_6f8);
            if (0xf < local_1a0) {
              uVar12 = local_1b8[0];
              if (0xfff < local_1a0 + 1) {
                if ((local_1b8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar12 = *(ulonglong *)(local_1b8[0] - 8);
                if (local_1b8[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_1b8[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_1b8[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002dab(uVar12);
            }
            *(undefined ***)((longlong)&local_598 + (longlong)*(int *)(local_598 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_59c + (longlong)*(int *)(local_598 + 4)) =
                 *(int *)(local_598 + 4) + -0x88;
            local_590 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14002e320(&local_590);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_590);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_588);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_510);
          }
        }
        pQVar16 = (QMutex *)(param_1 + 3);
        this_01 = (QWaitCondition *)(param_1 + 4);
      }
LAB_14004861f:
      BVar7 = CloseDesktop(hDesktop_00);
      if (BVar7 == 0) {
        uVar8 = thunk_FUN_1400c5db0(local_6c8);
        cVar5 = thunk_FUN_1400c5970(uVar8,30000);
        thunk_FUN_1400c47d0(local_6c8);
        if (cVar5 != '\0') {
          local_3b8 = &DAT_14026e900;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_330);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_3b8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_3b0,false);
          *(undefined ***)((longlong)&local_3b8 + (longlong)*(int *)(local_3b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_3bc + (longlong)*(int *)(local_3b8 + 4)) =
               *(int *)(local_3b8 + 4) + -0x88;
          local_768 = &local_3b0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_3b0);
          local_3b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_348 = 0;
          local_340 = 4;
          DVar6 = GetLastError();
          pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(&local_3b8,"(pe) CloseDesktop failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
          uVar8 = thunk_FUN_140030720(&local_3b8,local_178);
          uVar10 = thunk_FUN_1400c5db0(local_6b8);
          thunk_FUN_1400c4fa0(uVar10,30000,uVar8,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c47d0(local_6b8);
          if (0xf < local_160) {
            uVar12 = local_178[0];
            if (0xfff < local_160 + 1) {
              if ((local_178[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar12 = *(ulonglong *)(local_178[0] - 8);
              if (local_178[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_178[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_178[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002dab(uVar12);
          }
          *(undefined ***)((longlong)&local_3b8 + (longlong)*(int *)(local_3b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_3bc + (longlong)*(int *)(local_3b8 + 4)) =
               *(int *)(local_3b8 + 4) + -0x88;
          local_3b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14002e320(&local_3b0);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_3b0);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_3a8);
          this = local_330;
          goto LAB_140048a2a;
        }
      }
    }
    QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 5));
    QMutexLocker::~QMutexLocker(local_760);
    QMutexLocker::QMutexLocker(local_760,(QBasicMutex *)pQVar16);
  } while( true );
}

// =============================================================================
// Ghidra: FUN_1400471d0 @ 0x1400471d0
// =============================================================================
/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400471d0(longlong param_1,HWND param_2)

{
  HWND pHVar1;
  uint *puVar2;
  char cVar3;
  BOOL BVar4;
  DWORD DVar5;
  int iVar6;
  HWND pHVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  HANDLE hProcess;
  uint *puVar10;
  uint *puVar11;
  QString *pQVar12;
  uint *puVar13;
  undefined1 auStack_2918 [32];
  longlong *local_28f8;
  longlong local_28e8;
  longlong local_28e0;
  longlong local_28d8;
  uint local_28d0;
  DWORD local_28cc;
  QString local_28c8 [8];
  HWND local_28c0;
  QString local_28b8 [8];
  QString local_28b0 [8];
  QString local_28a8 [8];
  QString local_28a0 [8];
  undefined8 local_2898;
  undefined8 local_2890;
  undefined8 local_2888;
  tagWINDOWINFO local_2880;
  WCHAR local_2838 [1024];
  WCHAR local_2038 [4096];
  ulonglong local_38;
  undefined8 local_30;
  
  local_30 = 0x1400471eb;
  local_2888 = 0xfffffffffffffffe;
  local_38 = DAT_140364720 ^ (ulonglong)auStack_2918;
  if (*(longlong *)(param_1 + 0xd8) == 0) {
    qt_assert("d","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qscopedpointer.h",
              0x70);
  }
  pHVar1 = (HWND)**(longlong **)(param_1 + 0xd8);
  if (pHVar1 != param_2) {
    BVar4 = IsWindowVisible(param_2);
    if (BVar4 == 0) {
      return;
    }
    pHVar7 = GetWindow(param_2,4);
    if (pHVar7 != (HWND)0x0) {
      return;
    }
  }
  uVar8 = GetWindowLongPtrW(param_2,-0x10);
  uVar9 = GetWindowLongPtrW(param_2,-0x14);
  if ((((pHVar1 != param_2) && (*(char *)(param_1 + 0x80) != '\0')) && ((uVar9 & 0x40000) == 0)) &&
     (((uVar8 & 0xc0000) == 0 && (((uint)uVar9 & 0x88) != 8)))) {
    return;
  }
  local_2880.rcWindow.top = 0;
  local_2880.rcWindow.right = 0;
  local_2880.rcWindow.bottom = 0;
  local_2880.rcClient.left = 0;
  local_2880.rcClient.top = 0;
  local_2880.rcClient.right = 0;
  local_2880.rcClient.bottom = 0;
  local_2880.dwStyle = 0;
  local_2880.dwExStyle = 0;
  local_2880.dwWindowStatus = 0;
  local_2880.cxWindowBorders = 0;
  local_2880.cyWindowBorders = 0;
  local_2880.atomWindowType = 0;
  local_2880.wCreatorVersion = 0;
  local_2880.cbSize = 0x3c;
  local_2880.rcWindow.left = 0;
  BVar4 = GetWindowInfo(param_2,&local_2880);
  if (BVar4 == 0) {
    return;
  }
  DVar5 = GetWindowThreadProcessId(param_2,&local_28cc);
  if (DVar5 == 0) {
    return;
  }
  hProcess = OpenProcess(0x410,0,local_28cc);
  if (hProcess == (HANDLE)0x0) {
    return;
  }
  QString::QString((QString *)&local_28d8);
  QString::QString((QString *)&local_28e0);
  QString::QString((QString *)&local_28e8);
  local_28f8 = &local_28d8;
  cVar3 = thunk_FUN_14004be80(param_1 + 0x88,param_2,hProcess,&local_28e8);
  puVar13 = (uint *)0x0;
  if (cVar3 == '\0') {
    if (((*(char *)(param_1 + 0x80) == '\0') || (((uint)uVar8 & 0x80c00000) != 0x80000000)) ||
       ((local_2880._40_8_ & 0x100000000) != 0)) {
      if (1 < **(uint **)(param_1 + 0x38)) {
        thunk_FUN_140042da0(param_1 + 0x38);
      }
      puVar2 = *(uint **)(param_1 + 0x38);
      puVar10 = *(uint **)(puVar2 + 4);
      if (puVar10 == (uint *)0x0) {
LAB_1400473f3:
        puVar13 = puVar2 + 2;
      }
      else {
        do {
          if (*(HWND *)(puVar10 + 6) < param_2) {
            puVar11 = *(uint **)(puVar10 + 4);
          }
          else {
            puVar11 = *(uint **)(puVar10 + 2);
            puVar13 = puVar10;
          }
          puVar10 = puVar11;
        } while (puVar11 != (uint *)0x0);
        if ((puVar13 == (uint *)0x0) || (param_2 < *(HWND *)(puVar13 + 6))) goto LAB_1400473f3;
      }
      if (1 < *puVar2) {
        thunk_FUN_140042da0(param_1 + 0x38);
      }
      if ((uint *)(*(longlong *)(param_1 + 0x38) + 8) != puVar13) {
        QString::operator=((QString *)&local_28e0,(QString *)(puVar13 + 0xe));
      }
      if (*(int *)(local_28e0 + 4) == 0) {
        local_28d0 = 0x3ff;
        DVar5 = GetProcessImageFileNameW(hProcess,local_2838,(DWORD)&local_28d0);
        if (DVar5 == 0) goto LAB_1400474fe;
        if (0x7ff < (ulonglong)local_28d0 * 2) {
                    /* WARNING: Subroutine does not return */
          __report_rangecheckfailure();
        }
        local_2838[local_28d0] = L'\0';
        pQVar12 = (QString *)QString::fromWCharArray((wchar_t *)local_28c8,(int)local_2838);
        QString::operator=((QString *)&local_28e0,pQVar12);
        QString::~QString(local_28c8);
      }
      iVar6 = GetWindowTextW(param_2,local_2038,0x1000);
      if (0x1fff < (ulonglong)((longlong)iVar6 * 2)) {
                    /* WARNING: Subroutine does not return */
        __report_rangecheckfailure();
      }
      local_2038[iVar6] = L'\0';
      pQVar12 = (QString *)QString::fromWCharArray((wchar_t *)local_28c8,(int)local_2038);
      QString::operator=((QString *)&local_28d8,pQVar12);
      QString::~QString(local_28c8);
      if (*(int *)(local_28d8 + 4) != 0) goto LAB_140047512;
    }
LAB_1400474fe:
    CloseHandle(hProcess);
  }
  else {
LAB_140047512:
    CloseHandle(hProcess);
    local_28c0 = (HWND)0x0;
    QString::QString(local_28b8);
    QString::QString(local_28b0);
    QString::QString(local_28a8);
    QString::QString(local_28a0);
    local_2898 = 0;
    local_2890 = 0;
    local_28c0 = param_2;
    QString::operator=(local_28b8,(QString *)&local_28d8);
    pQVar12 = (QString *)QString::number((__int64)local_28c8,local_28cc);
    QString::operator=(local_28b0,pQVar12);
    QString::~QString(local_28c8);
    pQVar12 = (QString *)&local_28e0;
    if (*(int *)(local_28e8 + 4) != 0) {
      pQVar12 = (QString *)&local_28e8;
    }
    QString::operator=(local_28a8,pQVar12);
    local_2898 = CONCAT44(local_2880.rcWindow.top,local_2880.rcWindow.left);
    local_2890 = CONCAT44(local_2880.rcWindow.bottom - local_2880.rcWindow.top,
                          local_2880.rcWindow.right - local_2880.rcWindow.left);
    thunk_FUN_140046190(param_1,&local_28c0,*(int *)(local_28e8 + 4) == 0);
    QString::~QString(local_28a0);
    QString::~QString(local_28a8);
    QString::~QString(local_28b0);
    QString::~QString(local_28b8);
  }
  QString::~QString((QString *)&local_28e8);
  QString::~QString((QString *)&local_28e0);
  QString::~QString((QString *)&local_28d8);
  return;
}

// =============================================================================
// Ghidra: FUN_1400515a0 @ 0x1400515a0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400515a0(undefined8 param_1,HWND param_2)

{
  char cVar1;
  BOOL BVar2;
  DWORD DVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  HDC hDC;
  HWND pHVar7;
  ulonglong uVar8;
  HWND pHVar9;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined4 local_168;
  undefined ***local_160 [2];
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  undefined8 local_58;
  tagRECT local_50;
  ulonglong local_40 [3];
  ulonglong local_28;
  ulonglong local_20;
  
  local_58 = 0xfffffffffffffffe;
  local_20 = DAT_140364720 ^ (ulonglong)auStack_198;
  local_168 = 0;
  BVar2 = GetWindowRect(param_2,&local_50);
  if (BVar2 == 0) {
    uVar4 = thunk_FUN_1400c5db0(local_160);
    cVar1 = thunk_FUN_1400c5970(uVar4,30000);
    thunk_FUN_1400c47d0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_160[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      DVar3 = GetLastError();
      pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_148,"GetWindowRect: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar3);
      uVar4 = thunk_FUN_140030720(&local_148,local_40);
      uVar6 = thunk_FUN_1400c5db0(local_160);
      local_178 = 0x17e;
      thunk_FUN_1400c4fa0(uVar6,30000,uVar4,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c47d0(local_160);
      if (0xf < local_28) {
        uVar8 = local_40[0];
        if (0xfff < local_28 + 1) {
          if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_40[0] - 8);
          if (local_40[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_40[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_40[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar8);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e320(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  else {
    local_50.right = local_50.right - local_50.left;
    local_50.left = 0;
    local_50.bottom = local_50.bottom - local_50.top;
    local_50.top = 0;
    hDC = GetWindowDC(param_2);
    if (hDC == (HDC)0x0) {
      uVar4 = thunk_FUN_1400c5db0(local_160);
      cVar1 = thunk_FUN_1400c5970(uVar4,30000);
      thunk_FUN_1400c47d0(local_160);
      if (cVar1 != '\0') {
        local_148 = &DAT_14026e900;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_168 = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_14002ce40(&local_140,2);
        DVar3 = GetLastError();
        pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002c8b0(&local_148,"GetWindowDC: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar3);
        uVar4 = thunk_FUN_140030720(&local_148,local_40);
        uVar6 = thunk_FUN_1400c5db0(local_160);
        local_178 = 0x18a;
        thunk_FUN_1400c4fa0(uVar6,30000,uVar4,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c47d0(local_160);
        if (0xf < local_28) {
          uVar8 = local_40[0];
          if (0xfff < local_28 + 1) {
            if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar8 = *(ulonglong *)(local_40[0] - 8);
            if (local_40[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_40[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_40[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002dab(uVar8);
        }
        thunk_FUN_14002d0f0(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
    }
    else {
      pHVar7 = GetForegroundWindow();
      pHVar9 = param_2;
      if (pHVar7 == param_2) {
        pHVar9 = (HWND)0x0;
      }
      thunk_FUN_1400501d0(param_1,hDC,pHVar9,&local_50);
      ReleaseDC(param_2,hDC);
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_1400501d0 @ 0x1400501d0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400501d0(longlong param_1,HDC param_2,HWND param_3,int *param_4)

{
  undefined1 uVar1;
  longlong lVar2;
  char cVar3;
  DWORD DVar4;
  BOOL BVar5;
  HBITMAP hbm;
  undefined8 uVar6;
  basic_ostream<char,std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  HDC hdc;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar9;
  ulonglong uVar10;
  UINT cLines;
  int iVar11;
  undefined1 auStackY_628 [32];
  HWND local_5d0 [2];
  HGDIOBJ local_5c0;
  int local_5b8;
  int iStack_5ac;
  undefined *local_5a8;
  undefined **local_5a0;
  basic_ostream<char,std::char_traits<char>_> local_598 [96];
  undefined8 local_538;
  undefined4 local_530;
  basic_ios<char,std::char_traits<char>_> local_520 [104];
  undefined8 local_4b8;
  ulonglong local_4b0 [3];
  ulonglong local_498;
  tagBITMAPINFO local_488 [24];
  ulonglong local_58;
  
  local_4b8 = 0xfffffffffffffffe;
  local_58 = DAT_140364720 ^ (ulonglong)auStackY_628;
  iVar11 = param_4[2] - *param_4;
  cLines = param_4[3] - param_4[1];
  local_5d0[0] = param_3;
  local_5b8 = iVar11;
  hbm = CreateCompatibleBitmap(param_2,iVar11,cLines);
  if (hbm == (HBITMAP)0x0) {
    uVar6 = thunk_FUN_1400c5db0(local_5d0);
    cVar3 = thunk_FUN_1400c5970(uVar6,40000);
    thunk_FUN_1400c47d0(local_5d0);
    if (cVar3 == '\0') {
      return;
    }
    local_5a8 = &DAT_14026e900;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_520)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
    *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
         *(int *)(local_5a8 + 4) + -0x88;
    local_5d0[0] = (HWND)&local_5a0;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_5a0);
    local_5a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_538 = 0;
    local_530 = 4;
    DVar4 = GetLastError();
    pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002c8b0(&local_5a8,"CreateCompatibleBitmap: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
    uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
    uVar8 = thunk_FUN_1400c5db0(local_5d0);
    thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                       );
    thunk_FUN_1400c47d0(local_5d0);
    if (0xf < local_498) {
      uVar10 = local_4b0[0];
      if (0xfff < local_498 + 1) {
        if ((local_4b0[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_4b0[0] - 8);
        if (local_4b0[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_4b0[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_4b0[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002dab(uVar10);
    }
    *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
         *(int *)(local_5a8 + 4) + -0x88;
    local_5a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e320(&local_5a0);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_5a0);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_598);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_520);
    return;
  }
  hdc = CreateCompatibleDC(param_2);
  if (hdc == (HDC)0x0) {
    uVar6 = thunk_FUN_1400c5db0(local_5d0);
    cVar3 = thunk_FUN_1400c5970(uVar6,40000);
    thunk_FUN_1400c47d0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002ce40(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_5a8,"CreateCompatibleDC: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
      uVar8 = thunk_FUN_1400c5db0(local_5d0);
      thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c47d0(local_5d0);
      if (0xf < local_498) {
        uVar10 = local_4b0[0];
        if (0xfff < local_498 + 1) {
          if ((local_4b0[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_4b0[0] - 8);
          if (local_4b0[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_4b0[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_4b0[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar10);
      }
      thunk_FUN_14002d0f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
    }
    DeleteObject(hbm);
    return;
  }
  local_5c0 = SelectObject(hdc,hbm);
  if (0xfffffffffffffffd < (longlong)local_5c0 - 1U) {
    uVar6 = thunk_FUN_1400c5db0(local_5d0);
    cVar3 = thunk_FUN_1400c5970(uVar6,40000);
    thunk_FUN_1400c47d0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002ce40(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_5a8,"SelectObject: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
      uVar8 = thunk_FUN_1400c5db0(local_5d0);
      thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c47d0(local_5d0);
      if (0xf < local_498) {
        uVar10 = local_4b0[0];
        if (0xfff < local_498 + 1) {
          if ((local_4b0[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_4b0[0] - 8);
          if (local_4b0[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_4b0[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_4b0[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002dab(uVar10);
      }
      thunk_FUN_14002d0f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
    }
    goto LAB_14005116e;
  }
  if (local_5d0[0] == (HWND)0x0) {
    BVar5 = BitBlt(hdc,0,0,iVar11,cLines,param_2,*param_4,param_4[1],0xcc0020);
    if (BVar5 != 0) goto LAB_140050859;
    uVar6 = thunk_FUN_1400c5db0(local_5d0);
    cVar3 = thunk_FUN_1400c5970(uVar6,40000);
    thunk_FUN_1400c47d0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002ce40(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_5a8,"BitBlt: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
      uVar8 = thunk_FUN_1400c5db0(local_5d0);
      thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c47d0(local_5d0);
      thunk_FUN_14002d190(local_4b0);
      goto LAB_1400506f1;
    }
  }
  else {
    BVar5 = PrintWindow(local_5d0[0],hdc,*(UINT *)(param_1 + 0x8c));
    if (BVar5 == 0) {
      uVar6 = thunk_FUN_1400c5db0(local_5d0);
      cVar3 = thunk_FUN_1400c5970(uVar6,40000);
      thunk_FUN_1400c47d0(local_5d0);
      if (cVar3 == '\0') goto LAB_140050704;
      local_5a8 = &DAT_14026e900;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002ce40(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002c8b0(&local_5a8,"PrintWindow: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
      uVar8 = thunk_FUN_1400c5db0(local_5d0);
      thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c47d0(local_5d0);
      thunk_FUN_14002d190(local_4b0);
LAB_1400506f1:
      thunk_FUN_14002d0f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
    }
    else {
LAB_140050859:
      memset((void *)((longlong)&local_488[0].bmiHeader.biSize + 1),0,0x427);
      local_488[0].bmiHeader.biSize = 0x28;
      local_488[0].bmiHeader.biPlanes = 1;
      local_488[0].bmiHeader.biBitCount = 0;
      local_488[0].bmiHeader.biCompression = 0;
      local_488[0].bmiHeader.biWidth = iVar11;
      local_488[0].bmiHeader.biHeight = cLines;
      iVar11 = GetDIBits(hdc,hbm,0,cLines,(LPVOID)0x0,local_488,0);
      if (iVar11 == 0) {
        uVar6 = thunk_FUN_1400c5db0(local_5d0);
        cVar3 = thunk_FUN_1400c5970(uVar6,40000);
        thunk_FUN_1400c47d0(local_5d0);
        if (cVar3 != '\0') {
          local_5a8 = &DAT_14026e900;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_520);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
          *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
               *(int *)(local_5a8 + 4) + -0x88;
          thunk_FUN_14002ce40(&local_5a0,2);
          DVar4 = GetLastError();
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(&local_5a8,"GetDIBits: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
          uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
          uVar8 = thunk_FUN_1400c5db0(local_5d0);
          thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                             );
          thunk_FUN_1400c47d0(local_5d0);
          thunk_FUN_14002d190(local_4b0);
          goto LAB_1400506f1;
        }
      }
      else {
        if ((local_488[0].bmiHeader.biBitCount == 0x10) ||
           ((local_488[0].bmiHeader.biBitCount - 0x18 & 0xfffffff7) == 0)) {
LAB_140050e24:
          if (**(uint **)(param_1 + 0x18) <= local_488[0].bmiHeader.biSizeImage + 0x41c) {
            *(DWORD *)(param_1 + 0x20) = local_488[0].bmiHeader.biSizeImage + 0x1041c;
            **(uint **)(param_1 + 0x18) = 0;
            return;
          }
          local_488[0].bmiHeader.biHeight = -cLines;
          local_488[0].bmiHeader._12_8_ = local_488[0].bmiHeader._12_8_ & 0xffffffff;
          iVar11 = GetDIBits(hdc,hbm,0,cLines,(LPVOID)(*(longlong *)(param_1 + 0x10) + 0x418),
                             local_488,0);
          if (iVar11 == 0) {
            uVar6 = thunk_FUN_1400c5db0(local_5d0);
            cVar3 = thunk_FUN_1400c5970(uVar6,40000);
            thunk_FUN_1400c47d0(local_5d0);
            if (cVar3 != '\0') {
              local_5a8 = &DAT_14026e900;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_520);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
              *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
                   *(int *)(local_5a8 + 4) + -0x88;
              thunk_FUN_14002ce40(&local_5a0,2);
              DVar4 = GetLastError();
              pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_14002c8b0(&local_5a8,"GetDIBits: ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
              uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
              uVar8 = thunk_FUN_1400c5db0(local_5d0);
              thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                                  "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                                 );
              thunk_FUN_1400c47d0(local_5d0);
              thunk_FUN_14002d190(local_4b0);
              thunk_FUN_14002d0f0(local_520);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_520);
            }
          }
          else {
            thunk_FUN_14004dcc0(param_1,local_5b8,cLines,local_488[0].bmiHeader.biBitCount);
            **(int **)(param_1 + 0x18) =
                 **(int **)(param_1 + 0x18) - (local_488[0].bmiHeader.biSizeImage + 0x41c);
          }
          SelectObject(hdc,local_5c0);
          DeleteDC(hdc);
          DeleteObject(hbm);
          return;
        }
        uVar6 = thunk_FUN_1400c5db0(local_5d0);
        cVar3 = thunk_FUN_1400c5970(uVar6,30000);
        thunk_FUN_1400c47d0();
        if (cVar3 != '\0') {
          local_5a8 = &DAT_14026e900;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_520);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
          *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
               *(int *)(local_5a8 + 4) + -0x88;
          thunk_FUN_14002ce40(&local_5a0,2);
          uVar1 = *(undefined1 *)(*(longlong *)(param_1 + 0x10) + 0x400);
          uVar6 = thunk_FUN_14002c8b0(&local_5a8,"unexpected gdi biBitCount before fallback");
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(uVar6,", biBitCount: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                             (pbVar7,local_488[0].bmiHeader.biBitCount);
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(pbVar9,", biCompression: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                             (pbVar7,local_488[0].bmiHeader.biCompression);
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(pbVar9,", biSizeImage: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                             (pbVar7,local_488[0].bmiHeader.biSizeImage);
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(pbVar9,", width: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,local_5b8);
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(pbVar9,", height: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,cLines);
          uVar6 = thunk_FUN_14002c8b0(pbVar9,", isMonitorId: ");
          uVar6 = thunk_FUN_14004c5d0(uVar6,uVar1);
          lVar2 = *(longlong *)(param_1 + 0x10);
          pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002c8b0(uVar6,", maxArea: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                             (pbVar7,*(uint *)(lVar2 + 0x404));
          uVar6 = thunk_FUN_14002c8b0(pbVar9,", monitorOrWindowId: ");
          thunk_FUN_14002c8b0(uVar6,lVar2);
          uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
          uVar8 = thunk_FUN_1400c5db0(local_5d0);
          thunk_FUN_1400c4fa0(uVar8,30000,uVar6,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                             );
          thunk_FUN_1400c47d0(local_5d0);
          thunk_FUN_14002d190(local_4b0);
          thunk_FUN_14002d0f0(local_520);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_520);
        }
        local_488[0].bmiHeader.biBitCount = 0x10;
        local_488[0].bmiHeader.biCompression = 0;
        iVar11 = GetDIBits(hdc,hbm,0,cLines,(LPVOID)0x0,local_488,0);
        if (iVar11 == 0) {
          uVar6 = thunk_FUN_1400c5db0(local_5d0);
          cVar3 = thunk_FUN_1400c5970(uVar6,40000);
          thunk_FUN_1400c47d0(local_5d0);
          if (cVar3 != '\0') {
            local_5a8 = &DAT_14026e900;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_520);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
            *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
                 *(int *)(local_5a8 + 4) + -0x88;
            thunk_FUN_14002ce40(&local_5a0,2);
            DVar4 = GetLastError();
            pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14002c8b0(&local_5a8,"GetDIBits: ");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
            uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
            uVar8 = thunk_FUN_1400c5db0(local_5d0);
            thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                                "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                               );
            thunk_FUN_1400c47d0(local_5d0);
            thunk_FUN_14002d190(local_4b0);
            goto LAB_1400506f1;
          }
        }
        else {
          if (local_488[0].bmiHeader.biBitCount == 0x10) goto LAB_140050e24;
          uVar6 = thunk_FUN_1400c5db0(local_5d0);
          cVar3 = thunk_FUN_1400c5970(uVar6,40000);
          thunk_FUN_1400c47d0(local_5d0);
          if (cVar3 != '\0') {
            local_5a8 = &DAT_14026e900;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_520);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
            *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_5ac + (longlong)*(int *)(local_5a8 + 4)) =
                 *(int *)(local_5a8 + 4) + -0x88;
            thunk_FUN_14002ce40(&local_5a0,2);
            thunk_FUN_14002c8b0(&local_5a8,"incompatible device");
            uVar6 = thunk_FUN_140030720(&local_5a8,local_4b0);
            uVar8 = thunk_FUN_1400c5db0(local_5d0);
            thunk_FUN_1400c4fa0(uVar8,40000,uVar6,
                                "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                               );
            thunk_FUN_1400c47d0(local_5d0);
            thunk_FUN_14002d190(local_4b0);
            goto LAB_1400506f1;
          }
        }
      }
    }
  }
LAB_140050704:
  SelectObject(hdc,local_5c0);
LAB_14005116e:
  DeleteObject(hbm);
  DeleteDC(hdc);
  return;
}

// =============================================================================
// Ghidra: FUN_140098a10 @ 0x140098a10
// =============================================================================
undefined8 FUN_140098a10(void)

{
  BOOL BVar1;
  HMODULE hmodWinEventProc;
  HWINEVENTHOOK hWinEventHook;
  tagMSG local_38;
  
  if (DAT_14036f64b == '\0') {
    DAT_14036f64b = 1;
    hmodWinEventProc = GetModuleHandleA((LPCSTR)0x0);
    hWinEventHook = SetWinEventHook(2,2,hmodWinEventProc,(WINEVENTPROC)&LAB_140009142,0,0,0);
    do {
      BVar1 = GetMessageA(&local_38,(HWND)0x0,0,0);
    } while (BVar1 != 0);
    UnhookWinEvent(hWinEventHook);
    DAT_14036f64b = '\0';
  }
  return 0;
}
