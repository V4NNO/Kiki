/*
 * tlsservice_FULL CODE.cpp
 * tlsservice.exe / Kickidler Grabber Agent - pseudocod extras cu Ghidra MCP
 * Data analizei: 2026-09-21
 *
 * IMPORTANT:
 * - Acesta este pseudocod C/C++ produs de Ghidra, nu codul-sursa original.
 * - Nu este garantat compilabil. Tipurile undefined*, numele FUN_/DAT_/LAB_,
 *   prototipurile si conventiile de apel sunt inferentele decompilatorului.
 * - Adresele sunt pastrate pentru verificare directa in proiectul Ghidra.
 * - Sunt reunite traseele relevante ale aplicatiei. Binarul are 20.617 functii;
 *   Qt/CRT/STL, PortAudio si instantiarile repetitive nu sunt reproduse integral.
 */


// =============================================================================
// STARTUP SI INITIALIZARE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14000d431
// -----------------------------------------------------------------------------
void entry(void)

{
  __security_init_cookie();
  FUN_140193f60();
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140193f60
// -----------------------------------------------------------------------------
int FUN_140193f60(void)

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
  if (DAT_140376558 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_140376558 == 0) {
    DAT_140376558 = 1;
    iVar6 = _initterm_e(&DAT_1402729c0,&DAT_140272e00);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_140271000,&DAT_140272580);
    DAT_140376558 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_14019556c();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_140195578();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1401f24d0(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140030ce0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140030ce0(int param_1,char **param_2)

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
  local_40 = DAT_14036a720 ^ (ulonglong)auStackY_2d8;
  bVar4 = false;
  local_298 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_298 & 0xffffffff00000000);
  local_268[0] = param_1;
  local_2a8 = QString::fromAscii_helper("KickidlerGrabber",0x10);
  QCoreApplication::setApplicationName((QString *)&local_2a8);
  QString::~QString((QString *)&local_2a8);
  local_60[0] = (QTypedArrayData<unsigned_short> *)&local_2a0;
  uVar6 = QFile::decodeName((char *)&local_2a0);
  uVar7 = thunk_FUN_14002f990();
  thunk_FUN_1400bf980(uVar7,1,uVar6);
  uVar6 = thunk_FUN_140030580(&local_2a0);
  thunk_FUN_1400c0ba0(uVar6);
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
      goto LAB_140030e21;
    }
  }
  bVar2 = false;
LAB_140030e21:
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
  pQVar10 = (QString *)thunk_FUN_14002d820(&local_278,&local_2a8,&DAT_140273bcc);
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
    pQVar10 = (QString *)thunk_FUN_14002d8b0(&local_2a0,&DAT_140273bd0,&local_260);
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
  uVar6 = thunk_FUN_140030580(&local_278,8);
  uVar6 = thunk_FUN_14002d820(&local_2a0,uVar6,&DAT_140273bd8);
  thunk_FUN_14002d7b0(local_148,uVar6,local_258);
  QString::~QString((QString *)&local_2a0);
  QString::~QString((QString *)&local_278);
  uVar6 = QString::QString((QString *)&local_2a8,local_148);
  thunk_FUN_1400c9970(uVar6,0xa00000,10);
  qInstallMessageHandler(thunk_FUN_14002fa30);
  QApplication::QApplication(local_100,local_268,param_2,0x50602);
  uVar6 = thunk_FUN_1400c9380(&local_288);
  cVar3 = thunk_FUN_1400c8f40(uVar6,20000);
  thunk_FUN_1400c7da0(&local_288);
  iVar5 = 4;
  uVar17 = uVar16;
  if (cVar3 != '\0') {
    local_248 = &DAT_1402738f0;
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
    thunk_FUN_14002cab0(&local_248,"start agent");
    uVar6 = thunk_FUN_140030920(&local_248,local_60);
    uVar7 = thunk_FUN_1400c9380(&local_288);
    thunk_FUN_1400c8570(uVar7,20000,uVar6);
    thunk_FUN_1400c7da0(&local_288);
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
      FUN_140002d06(pQVar14);
    }
    *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
         *(int *)(local_248 + 4) + -0x88;
    local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_240);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_238);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_1c0);
  }
  uVar6 = thunk_FUN_140030580(&local_2a8,10);
  pQVar10 = (QString *)thunk_FUN_14002d820(&local_278,uVar6,"/grabber.ini");
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
  thunk_FUN_14004c780();
  local_150 = QString::fromAscii_helper("grabberSubAgent",0xf);
  pQVar10 = (QString *)QCoreApplication::applicationFilePath();
  this_00 = (QFileInfo *)QFileInfo::QFileInfo((QFileInfo *)&local_278,pQVar10);
  uVar6 = QFileInfo::fileName(this_00);
  lVar9 = thunk_FUN_14002ee90(&local_88,uVar6);
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
  uVar7 = thunk_FUN_14002d820(&local_2a8,uVar7,&DAT_140273bd8);
  uVar7 = thunk_FUN_14002d7b0(&local_278,uVar7,&local_150);
  thunk_FUN_14008f0e0(local_e8,uVar7,&local_288,uVar6);
  QString::~QString((QString *)&local_2a8);
  QString::~QString((QString *)local_60);
  cVar3 = thunk_FUN_140091540(local_e8);
  uVar16 = uVar17;
  if ((cVar3 == '\0') || (puVar11 = (undefined1 *)thunk_FUN_1400911c0(), puVar11 != local_e8)) {
    uVar6 = thunk_FUN_1400c9380(&local_288);
    cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
    thunk_FUN_1400c7da0(&local_288);
    if (cVar3 != '\0') {
      local_248 = &DAT_1402738f0;
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
      puVar11 = (undefined1 *)thunk_FUN_1400911c0();
      bVar4 = (bool)thunk_FUN_140091540(local_e8);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(&local_248,"Failed to create sub agent IO routines: isValid = ")
      ;
      pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,bVar4);
      pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(pbVar13,", is singletone = ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,puVar11 == local_e8);
      uVar6 = thunk_FUN_140030920(&local_248,&local_88);
      uVar7 = thunk_FUN_1400c9380(&local_288);
      thunk_FUN_1400c8570(uVar7,40000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp")
      ;
      thunk_FUN_1400c7da0(&local_288);
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
        FUN_140002d06(uVar15);
      }
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_240);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_238);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1c0);
    }
  }
  uVar6 = thunk_FUN_1400c9380(&local_288);
  cVar3 = thunk_FUN_1400c8f40(uVar6,20000);
  thunk_FUN_1400c7da0(&local_288);
  uVar17 = uVar16;
  if (cVar3 != '\0') {
    local_248 = &DAT_1402738f0;
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
    thunk_FUN_14002cab0(&local_248,"This build using brand new subAgentIo system!");
    uVar6 = thunk_FUN_140030920(&local_248,&local_88);
    uVar7 = thunk_FUN_1400c9380(&local_288);
    thunk_FUN_1400c8570(uVar7,20000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c7da0(&local_288);
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
      FUN_140002d06(uVar15);
    }
    *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
         *(int *)(local_248 + 4) + -0x88;
    local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_240);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_238);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_1c0);
  }
  thunk_FUN_140032790(&local_128);
  cVar3 = thunk_FUN_140035d40(local_128);
  if (cVar3 == '\0') {
    uVar6 = thunk_FUN_1400c9380(&local_288);
    cVar3 = thunk_FUN_1400c8f40(uVar6,20000);
    thunk_FUN_1400c7da0(&local_288);
    if (cVar3 != '\0') {
      local_248 = &DAT_1402738f0;
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
      thunk_FUN_14002cab0(&local_248,"communicator initialization failed");
      uVar6 = thunk_FUN_140030920(&local_248,&local_88);
      uVar7 = thunk_FUN_1400c9380(&local_288);
      thunk_FUN_1400c8570(uVar7,20000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp")
      ;
      thunk_FUN_1400c7da0(&local_288);
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
        FUN_140002d06(uVar15);
      }
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_240);
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
    uVar6 = thunk_FUN_1400c9380(&local_288);
    cVar3 = thunk_FUN_1400c8f40(uVar6,20000);
    thunk_FUN_1400c7da0(&local_288);
    if (cVar3 != '\0') {
      local_248 = &DAT_1402738f0;
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
      thunk_FUN_14002cab0(&local_248,&DAT_140273cd4);
      uVar6 = thunk_FUN_140030920(&local_248,&local_88);
      uVar7 = thunk_FUN_1400c9380(&local_288);
      thunk_FUN_1400c8570(uVar7,20000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp")
      ;
      thunk_FUN_1400c7da0(&local_288);
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
        FUN_140002d06(uVar15);
      }
      *(undefined ***)((longlong)&local_248 + (longlong)*(int *)(local_248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_24c + (longlong)*(int *)(local_248 + 4)) =
           *(int *)(local_248 + 4) + -0x88;
      local_240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_240);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_240);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_238);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1c0);
    }
  }
  thunk_FUN_14002ebe0(local_120);
  thunk_FUN_14008fdd0(local_e8);
  QString::~QString((QString *)&local_150);
  QSettings::~QSettings(local_118);
  QApplication::~QApplication(local_100);
  QString::~QString(local_148);
  QString::~QString((QString *)&local_260);
  QString::~QString(local_258);
  return iVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14002fa30
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14002fa30(int param_1,undefined8 param_2,QString *param_3)

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
  local_10 = DAT_14036a720 ^ (ulonglong)auStack_168;
  local_138[0] = 0;
  if (param_1 == 0) {
    uVar2 = thunk_FUN_1400c9380(local_138);
    cVar1 = thunk_FUN_1400c8f40(uVar2,10000);
    thunk_FUN_1400c7da0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002d040(local_120,2);
    uVar2 = thunk_FUN_14002cab0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002cab0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002cab0(uVar2,&DAT_140273914);
    uVar2 = thunk_FUN_140030920(&local_128,local_30);
    uVar5 = thunk_FUN_1400c9380(local_138);
    local_148 = 0x4e;
    thunk_FUN_1400c8570(uVar5,10000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c7da0(local_138);
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
      FUN_140002d06(uVar6);
    }
  }
  else if (param_1 == 1) {
    uVar2 = thunk_FUN_1400c9380(local_138);
    cVar1 = thunk_FUN_1400c8f40(uVar2,30000);
    thunk_FUN_1400c7da0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002d040(local_120,2);
    uVar2 = thunk_FUN_14002cab0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002cab0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002cab0(uVar2,&DAT_140273914);
    uVar2 = thunk_FUN_140030920(&local_128,local_30);
    uVar5 = thunk_FUN_1400c9380(local_138);
    local_148 = 0x51;
    thunk_FUN_1400c8570(uVar5,30000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c7da0(local_138);
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
      FUN_140002d06(uVar6);
    }
  }
  else if (param_1 == 2) {
    uVar2 = thunk_FUN_1400c9380(local_138);
    cVar1 = thunk_FUN_1400c8f40(uVar2,40000);
    thunk_FUN_1400c7da0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002d040(local_120,2);
    uVar2 = thunk_FUN_14002cab0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002cab0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002cab0(uVar2,&DAT_140273914);
    uVar2 = thunk_FUN_140030920(&local_128,local_30);
    uVar5 = thunk_FUN_1400c9380(local_138);
    local_148 = 0x54;
    thunk_FUN_1400c8570(uVar5,40000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c7da0(local_138);
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
      FUN_140002d06(uVar6);
    }
  }
  else if (param_1 == 3) {
    uVar2 = thunk_FUN_1400c9380(local_138);
    cVar1 = thunk_FUN_1400c8f40(uVar2,50000);
    thunk_FUN_1400c7da0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002d040(local_120,2);
    uVar2 = thunk_FUN_14002cab0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002cab0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002cab0(uVar2,&DAT_140273914);
    uVar2 = thunk_FUN_140030920(&local_128,local_30);
    uVar5 = thunk_FUN_1400c9380(local_138);
    local_148 = 0x57;
    thunk_FUN_1400c8570(uVar5,50000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c7da0(local_138);
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
      FUN_140002d06(uVar6);
    }
  }
  else {
    uVar2 = thunk_FUN_1400c9380(local_138);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 0x10;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_14002d040(local_120,2);
    uVar2 = thunk_FUN_14002cab0(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_14002cab0(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_14002cab0(uVar2,&DAT_140273914);
    uVar2 = thunk_FUN_140030920(&local_128,local_30);
    uVar5 = thunk_FUN_1400c9380(local_138);
    local_148 = 0x5a;
    thunk_FUN_1400c8570(uVar5,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberAgent\\main.cpp");
    thunk_FUN_1400c7da0(local_138);
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
      FUN_140002d06(uVar6);
    }
  }
  thunk_FUN_14002d2f0(local_a0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140030580
// -----------------------------------------------------------------------------
undefined8
FUN_140030580(undefined8 param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  uVar1 = thunk_FUN_14002f990();
  thunk_FUN_1400bf870(uVar1,param_1,param_2,param_4,uVar2);
  return param_1;
}

// =============================================================================
// COMUNICATOR
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400321e0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 * FUN_1400321e0(undefined8 *param_1)

{
  char cVar1;
  int iVar2;
  HANDLE pvVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158;
  undefined1 local_150 [16];
  DWORD local_140;
  undefined8 local_138;
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_138 = 0xfffffffffffffffe;
  local_18 = DAT_14036a720 ^ (ulonglong)auStack_188;
  local_158 = 0;
  local_130 = param_1;
  thunk_FUN_140033be0();
  *param_1 = grabberAgent::CommunicatorImpl::vftable;
  param_1[0x72b3] = 0xffffffffffffffff;
  iVar2 = FUN_140032600(local_150);
  if (iVar2 != 0) {
    pvVar3 = OpenProcess(0x400,0,local_140);
    param_1[0x72b3] = pvVar3;
    if (pvVar3 == (HANDLE)0x0) {
      uVar4 = thunk_FUN_1400c9380(local_150);
      cVar1 = thunk_FUN_1400c8f40(uVar4,40000);
      thunk_FUN_1400c7da0(local_150);
      if (cVar1 != '\0') {
        local_128 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_a0);
        local_158 = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
        *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
             *(int *)(local_128 + 4) + -0x88;
        thunk_FUN_14002d040(local_120,2);
        this = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_128,"OpenProcess: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,(void *)param_1[0x72b3]);
        uVar4 = thunk_FUN_140030920(&local_128,local_38);
        uVar5 = thunk_FUN_1400c9380(local_150);
        local_168 = 99;
        thunk_FUN_1400c8570(uVar5,40000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicatorImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_150);
        if (0xf < local_20) {
          uVar6 = local_38[0];
          if (0xfff < local_20 + 1) {
            if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_38[0] - 8);
            if (local_38[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_38[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_38[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar6);
        }
        thunk_FUN_14002d2f0(local_a0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_a0);
      }
      param_1[0x72b3] = 0xffffffffffffffff;
    }
  }
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400328d0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1400328d0(longlong param_1)

{
  char cVar1;
  BOOL BVar2;
  DWORD DVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_178 [32];
  undefined4 local_158;
  DWORD local_148 [2];
  undefined1 local_140 [16];
  undefined8 local_130;
  undefined *local_128;
  basic_streambuf<char,std::char_traits<char>_> local_120 [128];
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_14036a720 ^ (ulonglong)auStack_178;
  local_148[1] = 0;
  if (*(HANDLE *)(param_1 + 0x39598) != (HANDLE)0xffffffffffffffff) {
    BVar2 = GetExitCodeProcess(*(HANDLE *)(param_1 + 0x39598),local_148);
    if (BVar2 == 0) {
      uVar4 = thunk_FUN_1400c9380(local_140);
      cVar1 = thunk_FUN_1400c8f40(uVar4,40000);
      thunk_FUN_1400c7da0(local_140);
      if (cVar1 != '\0') {
        local_128 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_a0);
        local_148[1] = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
        *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
             *(int *)(local_128 + 4) + -0x88;
        thunk_FUN_14002d040(local_120,2);
        DVar3 = GetLastError();
        this = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_128,"GetExitCodeProcess: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,DVar3);
        uVar4 = thunk_FUN_140030920(&local_128,local_38);
        uVar5 = thunk_FUN_1400c9380(local_140);
        local_158 = 0x7c;
        thunk_FUN_1400c8570(uVar5,40000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicatorImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_140);
        if (0xf < local_20) {
          uVar6 = local_38[0];
          if (0xfff < local_20 + 1) {
            if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_38[0] - 8);
            if (local_38[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_38[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_38[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar6);
        }
        thunk_FUN_14002d2f0(local_a0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_a0);
      }
    }
    else if (local_148[0] != 0x103) {
      return 0;
    }
  }
  return 1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140035f90
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140035f90(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  undefined1 uVar3;
  byte bVar4;
  byte bVar5;
  undefined8 uVar6;
  QString *pQVar7;
  QByteArray *pQVar8;
  char *pcVar9;
  undefined8 uVar10;
  ulonglong uVar11;
  uint uVar12;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  uint local_168 [2];
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
  local_30 = DAT_14036a720 ^ (ulonglong)auStack_198;
  uVar12 = 0;
  local_168[0] = 0;
  uVar6 = thunk_FUN_1400c9380(local_160);
  cVar2 = thunk_FUN_1400c8f40(uVar6,20000);
  thunk_FUN_1400c7da0(local_160);
  if (cVar2 != '\0') {
    local_148 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    uVar12 = 1;
    local_168[0] = 1;
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
    pQVar7 = (QString *)thunk_FUN_1400385e0(param_1 + 0x39568,local_160);
    uVar6 = thunk_FUN_14002cab0(&local_148,"session data state = ");
    pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar7);
    pcVar9 = QByteArray::data(pQVar8);
    thunk_FUN_14002cab0(uVar6,pcVar9);
    QByteArray::~QByteArray((QByteArray *)local_168);
    QString::~QString((QString *)local_160);
    uVar6 = thunk_FUN_140030920(&local_148,local_50);
    uVar10 = thunk_FUN_1400c9380(local_160);
    local_178 = 0x4d;
    thunk_FUN_1400c8570(uVar10,20000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicator.cpp"
                       );
    thunk_FUN_1400c7da0(local_160);
    if (0xf < local_38) {
      uVar11 = local_50[0];
      if (0xfff < local_38 + 1) {
        if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar11 = *(ulonglong *)(local_50[0] - 8);
        if (local_50[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_50[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_50[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar11);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  uVar6 = thunk_FUN_1400c9380(local_160);
  cVar2 = thunk_FUN_1400c8f40(uVar6,20000);
  thunk_FUN_1400c7da0(local_160);
  if (cVar2 != '\0') {
    local_148 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    local_168[0] = uVar12 | 2;
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
    pQVar7 = (QString *)thunk_FUN_140038ac0(param_1 + 0x39580,local_168);
    uVar6 = thunk_FUN_14002cab0(&local_148,"data capture settings = ");
    pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar7);
    pcVar9 = QByteArray::data(pQVar8);
    thunk_FUN_14002cab0(uVar6,pcVar9);
    QByteArray::~QByteArray((QByteArray *)local_160);
    QString::~QString((QString *)local_168);
    uVar6 = thunk_FUN_140030920(&local_148,local_50);
    uVar10 = thunk_FUN_1400c9380(local_160);
    local_178 = 0x4e;
    thunk_FUN_1400c8570(uVar10,20000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicator.cpp"
                       );
    thunk_FUN_1400c7da0(local_160);
    if (0xf < local_38) {
      uVar11 = local_50[0];
      if (0xfff < local_38 + 1) {
        if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar11 = *(ulonglong *)(local_50[0] - 8);
        if (local_50[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_50[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_50[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar11);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  cVar2 = thunk_FUN_1400384e0(param_1 + 0x39568);
  if (cVar2 != '\0') {
    lVar1 = param_1 + 0x39580;
    cVar2 = thunk_FUN_140038a00(lVar1);
    if (cVar2 != '\0') {
      cVar2 = thunk_FUN_140038510(param_1 + 0x39568);
      if (cVar2 == '\0') {
        cVar2 = thunk_FUN_14003a380(*(undefined8 *)(param_1 + 0x58));
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x58) + 0x58))();
        }
        uVar3 = thunk_FUN_140038cc0(lVar1);
        thunk_FUN_14003aba0(*(undefined8 *)(param_1 + 0x58),uVar3);
        if (*(char *)(*(undefined8 **)(param_1 + 0x60) + 1) == '\0') {
          (**(code **)**(undefined8 **)(param_1 + 0x60))();
        }
        uVar3 = thunk_FUN_1400389f0(lVar1);
        thunk_FUN_1401b5f30(*(undefined8 *)(param_1 + 0x60),uVar3);
        cVar2 = thunk_FUN_14003d4b0(*(undefined8 *)(param_1 + 0x50));
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x50) + 0x58))();
        }
        uVar3 = thunk_FUN_140038a10(lVar1);
        thunk_FUN_14003d780(*(undefined8 *)(param_1 + 0x50),uVar3);
        cVar2 = thunk_FUN_140044770(*(undefined8 *)(param_1 + 0x68));
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x68) + 0x58))();
        }
        cVar2 = thunk_FUN_14004fb60(*(undefined8 *)(param_1 + 0x78));
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x78) + 8))();
        }
        uVar3 = thunk_FUN_140038cc0(lVar1);
        thunk_FUN_140050250(*(undefined8 *)(param_1 + 0x78),uVar3);
        cVar2 = thunk_FUN_14005c730(*(undefined8 *)(param_1 + 0x70));
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x70) + 0x58))();
        }
        cVar2 = thunk_FUN_140096c50(*(undefined8 *)(param_1 + 0x39558));
        if (cVar2 == '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x39558) + 0x58))();
        }
        param_1 = param_1 + 0x80;
        cVar2 = thunk_FUN_14006fe90(param_1);
        if (cVar2 == '\0') {
          thunk_FUN_14006fe40(param_1);
        }
        bVar4 = thunk_FUN_140038cc0(lVar1);
        bVar5 = thunk_FUN_140038ab0(lVar1);
        thunk_FUN_140071eb0(param_1,bVar4 & bVar5);
      }
      else {
        cVar2 = thunk_FUN_14003a380(*(undefined8 *)(param_1 + 0x58));
        if (cVar2 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x58) + 0x60))();
        }
        if ((char)(*(longlong **)(param_1 + 0x60))[1] != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x60) + 8))();
        }
        cVar2 = thunk_FUN_14003d4b0(*(undefined8 *)(param_1 + 0x50));
        if (cVar2 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x50) + 0x60))();
        }
        cVar2 = thunk_FUN_140044770(*(undefined8 *)(param_1 + 0x68));
        if (cVar2 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x68) + 0x60))();
        }
        cVar2 = thunk_FUN_14004fb60(*(undefined8 *)(param_1 + 0x78));
        if (cVar2 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x78) + 0x10))();
        }
        cVar2 = thunk_FUN_14005c730(*(undefined8 *)(param_1 + 0x70));
        if (cVar2 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x70) + 0x60))();
        }
        cVar2 = thunk_FUN_140096c50(*(undefined8 *)(param_1 + 0x39558));
        if (cVar2 != '\0') {
          (**(code **)(**(longlong **)(param_1 + 0x39558) + 0x60))();
        }
        cVar2 = thunk_FUN_14006fe90(param_1 + 0x80);
        if (cVar2 != '\0') {
          thunk_FUN_14006c500(param_1 + 0x80);
        }
      }
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140036fa0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140036fa0(undefined8 param_1,uint *param_2)

{
  QNetworkProxy *pQVar1;
  int iVar2;
  Data *pDVar3;
  char cVar4;
  undefined8 uVar5;
  QNetworkProxyFactory *this;
  undefined8 uVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  QNetworkProxyFactory *pQVar9;
  QNetworkProxy *this_00;
  uint uVar10;
  undefined1 auStack_348 [32];
  undefined4 local_328;
  uint local_318;
  QNetworkProxyFactory *local_308;
  QEventLoop *pQStack_300;
  Data *local_2f8;
  QNetworkProxyFactory *local_2f0;
  QNetworkProxyFactory *local_2e8;
  QTypedArrayData<unsigned_short> *local_2e0;
  undefined4 local_2d8;
  Connection local_2d0 [8];
  QString local_2c8 [8];
  QEventLoop local_2c0 [16];
  undefined8 local_2b0;
  undefined8 local_2a8;
  undefined8 uStack_2a0;
  undefined1 local_298 [16];
  undefined1 local_288 [16];
  undefined1 local_278 [16];
  undefined1 local_268 [12];
  int iStack_25c;
  undefined *local_258;
  undefined **local_250;
  basic_ostream<char,std::char_traits<char>_> local_248 [96];
  undefined8 local_1e8;
  undefined4 local_1e0;
  basic_ios<char,std::char_traits<char>_> local_1d0 [100];
  int iStack_16c;
  undefined *local_168;
  undefined **local_160;
  basic_ostream<char,std::char_traits<char>_> local_158 [96];
  undefined8 local_f8;
  undefined4 local_f0;
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_2b0 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_348;
  pQVar9 = (QNetworkProxyFactory *)0x0;
  local_318 = 0;
  local_2f8 = (Data *)shared_null_exref;
  uVar5 = thunk_FUN_1400911c0();
  cVar4 = thunk_FUN_140091540(uVar5);
  if (cVar4 != '\0') {
    QEventLoop::QEventLoop(local_2c0,(QObject *)0x0);
    local_308 = (QNetworkProxyFactory *)&local_2f8;
    pQStack_300 = local_2c0;
    local_2a8 = local_308;
    uStack_2a0 = pQStack_300;
    uVar5 = thunk_FUN_1400911c0();
    thunk_FUN_140032ed0(local_2d0,uVar5,thunk_FUN_140095a70,&local_2a8);
    QMetaObject::Connection::~Connection(local_2d0);
    uVar5 = thunk_FUN_1400911c0();
    thunk_FUN_140091040(uVar5);
    local_2d8 = 0;
    QEventLoop::exec(local_2c0,0);
    QEventLoop::~QEventLoop(local_2c0);
  }
  this = operator_new(0x20);
  local_308 = this;
  if (this != (QNetworkProxyFactory *)0x0) {
    QNetworkProxyFactory::QNetworkProxyFactory(this);
    *(undefined ***)this = utils::AccumulatedProxyFactory::vftable;
    *(code **)(this + 8) = shared_null_exref;
    *(undefined8 *)(this + 0x10) = 0;
    *(undefined8 *)(this + 0x18) = 0;
    pQVar9 = this;
  }
  local_2f0 = pQVar9;
  if (pQVar9 == (QNetworkProxyFactory *)0x0) {
    local_2e8 = (QNetworkProxyFactory *)0x0;
  }
  else {
    local_308 = operator_new(0x18);
    if (local_308 + 0x10 != (QNetworkProxyFactory *)0x0) {
      *(QNetworkProxyFactory **)(local_308 + 0x10) = pQVar9;
    }
    if (local_308 != (QNetworkProxyFactory *)0x0) {
      *(undefined1 **)(local_308 + 8) = &LAB_14000e985;
      *(undefined4 *)(local_308 + 4) = 1;
      *(undefined4 *)local_308 = 1;
    }
    local_2e8 = local_308;
    thunk_FUN_140036f80(local_308,pQVar9,1);
    FUN_14000554c(&local_2f0,pQVar9);
  }
  local_2e0 = QString::fromAscii_helper("single",6);
  thunk_FUN_140108ca0(local_2f0,&local_2e0);
  QString::~QString((QString *)&local_2e0);
  uVar10 = 0;
  if (0xfff < *param_2) {
    uVar5 = thunk_FUN_1400c9380(local_298);
    cVar4 = thunk_FUN_1400c8f40(uVar5,30000);
    thunk_FUN_1400c7da0(local_298);
    uVar10 = 0;
    if (cVar4 != '\0') {
      local_258 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1d0);
      uVar10 = 1;
      local_318 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_308 = (QNetworkProxyFactory *)&local_250;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1e8 = 0;
      local_1e0 = 4;
      thunk_FUN_14002cab0(&local_258,"buffer overflow for _netProxy detected");
      uVar5 = thunk_FUN_140030920(&local_258,local_78);
      uVar6 = thunk_FUN_1400c9380(local_288);
      local_328 = 0x311;
      thunk_FUN_1400c8570(uVar6,30000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicator.cpp"
                         );
      thunk_FUN_1400c7da0(local_288);
      if (0xf < local_60) {
        uVar8 = local_78[0];
        if (0xfff < local_60 + 1) {
          if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_78[0] - 8);
          if (local_78[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_78[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_78[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar8);
      }
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_250);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_248);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1d0);
    }
  }
  if (0x7fff < param_2[0x401]) {
    uVar5 = thunk_FUN_1400c9380(local_278);
    cVar4 = thunk_FUN_1400c8f40(uVar5,30000);
    thunk_FUN_1400c7da0(local_278);
    if (cVar4 != '\0') {
      local_168 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_e0);
      local_318 = uVar10 | 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_168,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_160,false);
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
           *(int *)(local_168 + 4) + -0x88;
      local_308 = (QNetworkProxyFactory *)&local_160;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_f8 = 0;
      local_f0 = 4;
      thunk_FUN_14002cab0(&local_168,"buffer overflow for _netProxyByPass detected");
      uVar5 = thunk_FUN_140030920(&local_168,local_58);
      uVar6 = thunk_FUN_1400c9380(local_268);
      local_328 = 0x316;
      thunk_FUN_1400c8570(uVar6,30000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicator.cpp"
                         );
      thunk_FUN_1400c7da0(local_268);
      if (0xf < local_40) {
        uVar8 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar8 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar8 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar8);
      }
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
           *(int *)(local_168 + 4) + -0x88;
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_160);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_158);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_e0);
    }
  }
  uVar5 = thunk_FUN_1400367e0();
  thunk_FUN_140144170(uVar5,param_2[0x2402]);
  uVar5 = QString::fromUtf8((char *)&local_308,(int)param_2 + 0x1008);
  uVar6 = QString::fromUtf8((char *)local_2c8,(int)param_2 + 4);
  uVar7 = thunk_FUN_1400367e0();
  thunk_FUN_140143350(uVar7,uVar6,uVar5,&local_2f0);
  QString::~QString(local_2c8);
  QString::~QString((QString *)&local_308);
  thunk_FUN_140034fc0(local_2e8);
  pDVar3 = local_2f8;
  if (*(int *)local_2f8 != 0) {
    if (*(int *)local_2f8 == -1) {
      return;
    }
    LOCK();
    iVar2 = *(int *)local_2f8;
    *(int *)local_2f8 = *(int *)local_2f8 + -1;
    UNLOCK();
    if (iVar2 != 1) {
      return;
    }
  }
  this_00 = (QNetworkProxy *)(local_2f8 + ((longlong)*(int *)(local_2f8 + 0xc) + 2) * 8);
  pQVar1 = (QNetworkProxy *)(local_2f8 + ((longlong)*(int *)(local_2f8 + 8) + 2) * 8);
  while (pQVar1 != this_00) {
    this_00 = this_00 + -8;
    QNetworkProxy::~QNetworkProxy(this_00);
  }
  QListData::dispose(pDVar3);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400377f0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400377f0(longlong *param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong lVar5;
  ulonglong uVar6;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158;
  undefined ***local_150 [2];
  undefined8 local_140;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_140 = 0xfffffffffffffffe;
  local_28 = DAT_14036a720 ^ (ulonglong)auStack_188;
  local_158 = 0;
  cVar1 = (**(code **)(*param_1 + 0x58))();
  if (cVar1 == '\0') {
    uVar3 = thunk_FUN_1400c9380(local_150);
    cVar1 = thunk_FUN_1400c8f40(uVar3,30000);
    thunk_FUN_1400c7da0(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_150[0] = &local_130;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_c8 = 0;
      local_c0 = 4;
      thunk_FUN_14002cab0(&local_138,"grabber has gone");
      uVar3 = thunk_FUN_140030920(&local_138,local_48);
      uVar4 = thunk_FUN_1400c9380(local_150);
      local_168 = 0x9d;
      thunk_FUN_1400c8570(uVar4,30000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-communicator\\communicator.cpp"
                         );
      thunk_FUN_1400c7da0(local_150);
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
        FUN_140002d06(uVar6);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    goto LAB_140037a00;
  }
  cVar1 = thunk_FUN_14016f910(param_1 + 6);
  if (cVar1 == '\0') {
    thunk_FUN_14016f460(param_1 + 6);
    iVar2 = thunk_FUN_140034c30(param_1);
    if (iVar2 != 0) {
      if (iVar2 == 1) {
        return;
      }
      if (iVar2 == 2) {
        return;
      }
      goto LAB_140037a5d;
    }
    cVar1 = thunk_FUN_14016f910(param_1 + 6);
    if (cVar1 != '\0') goto LAB_140037a5d;
    goto LAB_140037cc9;
  }
LAB_140037a5d:
  cVar1 = thunk_FUN_14016f8e0(param_1 + 6);
  if (cVar1 == '\0') goto LAB_140037cbc;
  lVar5 = thunk_FUN_14016e240(param_1 + 6);
  switch(*(undefined4 *)(lVar5 + 0x10)) {
  case 0:
    cVar1 = thunk_FUN_140035a90(param_1);
    if ((cVar1 != '\0') &&
       ((lVar5 = thunk_FUN_14016e240(param_1 + 6), *(int *)(lVar5 + 0x10) != 0 ||
        (cVar1 = thunk_FUN_140035940(param_1), cVar1 != '\0')))) goto switchD_140037a9c_caseD_1;
    goto LAB_140037ae6;
  default:
switchD_140037a9c_caseD_1:
    thunk_FUN_14016f970(param_1 + 6);
    return;
  case 10:
    goto switchD_140037a9c_caseD_a;
  case 0xb:
    thunk_FUN_14016f970(param_1 + 6);
LAB_140037a00:
    thunk_FUN_14016f460(param_1 + 6);
    thunk_FUN_1400380e0(param_1);
    return;
  case 0x15:
    cVar1 = thunk_FUN_1400351c0(param_1);
    goto LAB_140037ada;
  case 0x16:
    cVar1 = thunk_FUN_140035570(param_1);
    goto LAB_140037ada;
  case 0x17:
    cVar1 = thunk_FUN_140035790(param_1);
    goto LAB_140037ada;
  case 0x18:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    cVar1 = thunk_FUN_140035870(param_1,lVar5 + 0x14);
    goto LAB_140037ada;
  case 0x1f:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140074080(param_1 + 0x10,lVar5 + 0x14);
    break;
  case 0x20:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_1400724a0(param_1 + 0x10,lVar5 + 0x14);
    break;
  case 0x21:
    thunk_FUN_140073070(param_1 + 0x10);
    break;
  case 0x22:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140036fa0(param_1,lVar5 + 0x14);
    break;
  case 0xaa:
    cVar1 = thunk_FUN_140035b80(param_1);
    goto LAB_140037ada;
  case 0xb4:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140191a00(param_1[0x72ac],lVar5 + 0x14);
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140038520(param_1 + 0x72ad,lVar5 + 0x14);
    break;
  case 0xb6:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140191710(param_1[0x72ac],lVar5 + 0x14);
    break;
  case 0xb7:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140191880(param_1[0x72ac],lVar5 + 0x14);
    break;
  case 0xb8:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140191a30(param_1[0x72ac],lVar5 + 0x14);
    break;
  case 0xb9:
    thunk_FUN_140190a20(param_1[0x72ac]);
    break;
  case 0xbe:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_1401918b0(param_1[0x72ac],lVar5 + 0x14);
    break;
  case 0xbf:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140038a20(param_1 + 0x72b0,lVar5 + 0x14);
    break;
  case 200:
    cVar1 = thunk_FUN_1400352d0(param_1);
    goto LAB_140037ada;
  case 0xca:
    cVar1 = thunk_FUN_140035490(param_1);
    goto LAB_140037ada;
  case 0xcc:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    thunk_FUN_140191ab0(param_1[0x72ac],lVar5 + 0x14);
    break;
  case 0xd4:
    cVar1 = thunk_FUN_1400353b0(param_1);
LAB_140037ada:
    if (cVar1 != '\0') goto LAB_140037cbc;
LAB_140037ae6:
    lVar5 = thunk_FUN_14016e240(param_1 + 6);
    *(undefined4 *)(lVar5 + 0x10) = 10;
switchD_140037a9c_caseD_a:
    thunk_FUN_14016f970(param_1 + 6);
    goto LAB_140037cc9;
  }
  lVar5 = thunk_FUN_14016e240(param_1 + 6);
  *(undefined4 *)(lVar5 + 0x10) = 0;
LAB_140037cbc:
  cVar1 = thunk_FUN_14016f970(param_1 + 6);
  if (cVar1 == '\0') {
LAB_140037cc9:
    thunk_FUN_14016f460(param_1 + 6);
  }
  return;
}

// =============================================================================
// DISPLAY SI INPUT
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140039c50
// -----------------------------------------------------------------------------
void FUN_140039c50(longlong param_1)

{
  QMutexLocker local_res8 [32];
  
  QMutexLocker::QMutexLocker(local_res8,(QBasicMutex *)(param_1 + 0x10));
  if (*(char *)(param_1 + 0x38) == '\0') {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-displayEnumerator\\displayEnumerator.cpp"
              ,0x66);
  }
  *(undefined1 *)(param_1 + 0x38) = 0;
  thunk_FUN_140039a30(param_1 + 0x40);
  QTimer::stop((QTimer *)(param_1 + 0x18));
  *(undefined1 *)(param_1 + 0x48) = 0;
  QMutexLocker::~QMutexLocker(local_res8);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003a790
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14003a790(longlong param_1,QListData *param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  QString *pQVar5;
  QByteArray *this;
  QByteArray *this_00;
  char *pcVar6;
  char *pcVar7;
  basic_ostream<char,std::char_traits<char>_> *pbVar8;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar9;
  undefined8 uVar10;
  ulonglong uVar11;
  undefined1 auStack_1c8 [32];
  undefined4 local_1a8;
  undefined4 local_198 [2];
  QString local_190 [16];
  QString local_180 [8];
  QByteArray local_178 [8];
  QMutexLocker local_170 [8];
  undefined8 local_168;
  int iStack_15c;
  undefined *local_158;
  basic_streambuf<char,std::char_traits<char>_> local_150 [128];
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_168 = 0xfffffffffffffffe;
  local_48 = DAT_14036a720 ^ (ulonglong)auStack_1c8;
  local_198[0] = 0;
  QMutexLocker::QMutexLocker(local_170,(QBasicMutex *)(param_1 + 0x10));
  if ((*(char *)(param_1 + 0x58) != '\0') &&
     (cVar1 = thunk_FUN_1400396a0(param_2,param_1 + 0x40), cVar1 == '\0')) {
    uVar4 = thunk_FUN_1400c9380(local_190);
    cVar1 = thunk_FUN_1400c8f40(uVar4,20000);
    thunk_FUN_1400c7da0(local_190);
    if (cVar1 != '\0') {
      local_158 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      local_198[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,local_150,false);
      *(undefined ***)(local_150 + (longlong)*(int *)(local_158 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      thunk_FUN_14002d040(local_150,2);
      pQVar5 = (QString *)FUN_140039f30(local_190,param_2);
      this = (QByteArray *)QString::toLocal8Bit(pQVar5);
      pQVar5 = (QString *)FUN_140039f30(local_180,param_1 + 0x40);
      this_00 = (QByteArray *)QString::toLocal8Bit(pQVar5);
      iVar2 = QListData::size(param_2);
      iVar3 = QListData::size((QListData *)(param_1 + 0x40));
      pcVar6 = QByteArray::constData(this);
      pcVar7 = QByteArray::constData(this_00);
      uVar4 = thunk_FUN_14002cab0(&local_158,"display list changed");
      pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(uVar4,", oldCount: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar3);
      pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar9,", newCount: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar2);
      uVar4 = thunk_FUN_14002cab0(pbVar9,", old: ");
      uVar4 = thunk_FUN_14002cab0(uVar4,pcVar7);
      uVar4 = thunk_FUN_14002cab0(uVar4,", new: ");
      thunk_FUN_14002cab0(uVar4,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_198);
      QString::~QString(local_180);
      QByteArray::~QByteArray(local_178);
      QString::~QString(local_190);
      uVar4 = thunk_FUN_140030920(&local_158,local_68);
      uVar10 = thunk_FUN_1400c9380(local_190);
      local_1a8 = 0xc2;
      thunk_FUN_1400c8570(uVar10,20000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-displayEnumerator\\displayEnumerator.cpp"
                         );
      thunk_FUN_1400c7da0(local_190);
      if (0xf < local_50) {
        uVar11 = local_68[0];
        if (0xfff < local_50 + 1) {
          if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar11 = *(ulonglong *)(local_68[0] - 8);
          if (local_68[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_68[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_68[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar11);
      }
      thunk_FUN_14002d2f0(local_d0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
    thunk_FUN_140039580(param_1 + 0x40,param_2);
    if (*(char *)(param_1 + 0x48) == '\0') {
      *(undefined1 *)(param_1 + 0x48) = 1;
    }
    thunk_FUN_14003d720(*(undefined8 *)(param_1 + 0x50),param_1 + 0x40);
  }
  QMutexLocker::~QMutexLocker(local_170);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003b590
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14003b590(longlong param_1)

{
  char cVar1;
  DWORD DVar2;
  BOOL BVar3;
  HDESK hDesktop;
  HDESK hDesktop_00;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  HDC hdc;
  ulonglong uVar7;
  uint uVar8;
  undefined1 auStack_1d8 [32];
  undefined4 local_1b8;
  uint local_1a8;
  QMutexLocker local_1a0 [8];
  undefined ***local_198;
  int iStack_18c;
  undefined *local_188;
  undefined **local_180;
  basic_ostream<char,std::char_traits<char>_> local_178 [96];
  HDESK local_118;
  undefined4 local_110;
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  undefined8 local_98;
  undefined1 local_90 [16];
  undefined1 local_80 [16];
  undefined1 local_70 [16];
  undefined1 local_60 [16];
  ulonglong local_50 [3];
  ulonglong local_38;
  ulonglong local_30;
  
  local_98 = 0xfffffffffffffffe;
  local_30 = DAT_14036a720 ^ (ulonglong)auStack_1d8;
  uVar8 = 0;
  local_1a8 = 0;
  QMutexLocker::QMutexLocker(local_1a0,(QBasicMutex *)(param_1 + 0x68));
  do {
    QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 0x70));
    do {
      QWaitCondition::wait((QWaitCondition *)(param_1 + 0x70),(QMutex *)(param_1 + 0x68),0xffffffff)
      ;
      if (*(int *)(param_1 + 0x78) == 2) {
        QMutexLocker::~QMutexLocker(local_1a0);
        return 0;
      }
    } while (*(int *)(param_1 + 0x78) != 1);
    DVar2 = GetCurrentThreadId();
    hDesktop = GetThreadDesktop(DVar2);
    hDesktop_00 = OpenInputDesktop(0,0,0x10000000);
    if (hDesktop_00 == (HDESK)0x0) {
      uVar4 = thunk_FUN_1400c9380(local_90);
      cVar1 = thunk_FUN_1400c8f40(uVar4);
      thunk_FUN_1400c7da0();
      if (cVar1 != '\0') {
        local_188 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_100);
        uVar8 = uVar8 | 1;
        local_1a8 = uVar8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
        *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
             *(int *)(local_188 + 4) + -0x88;
        local_198 = &local_180;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
        local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_110 = 4;
        local_118 = hDesktop_00;
        DVar2 = GetLastError();
        pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_188,"(di) OpenInputDesktop failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar2);
        uVar4 = thunk_FUN_140030920(&local_188,local_50);
        uVar6 = thunk_FUN_1400c9380(local_80);
        local_1b8 = 0xa0;
        thunk_FUN_1400c8570(uVar6,30000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-displayEnumerator\\displayEnumeratorImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_80);
        if (0xf < local_38) {
          uVar7 = local_50[0];
          if (0xfff < local_38 + 1) {
            if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_50[0] - 8);
            if (local_50[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_50[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_50[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar7);
        }
        *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
             *(int *)(local_188 + 4) + -0x88;
LAB_14003b99d:
        local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_180);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_178);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_100);
      }
    }
    else {
      BVar3 = SetThreadDesktop(hDesktop_00);
      if (BVar3 == 0) {
        uVar4 = thunk_FUN_1400c9380(local_70);
        cVar1 = thunk_FUN_1400c8f40(uVar4);
        thunk_FUN_1400c7da0();
        if (cVar1 != '\0') {
          local_188 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_100);
          uVar8 = uVar8 | 2;
          local_1a8 = uVar8;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
          *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
               *(int *)(local_188 + 4) + -0x88;
          local_198 = &local_180;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
          local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_118 = (HDESK)0x0;
          local_110 = 4;
          DVar2 = GetLastError();
          pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(&local_188,"(di) SetThreadDesktop failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar2);
          uVar4 = thunk_FUN_140030920(&local_188,local_50);
          uVar6 = thunk_FUN_1400c9380(local_60);
          local_1b8 = 0xa4;
          thunk_FUN_1400c8570(uVar6,30000,uVar4,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-displayEnumerator\\displayEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_60);
          if (0xf < local_38) {
            uVar7 = local_50[0];
            if (0xfff < local_38 + 1) {
              if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar7 = *(ulonglong *)(local_50[0] - 8);
              if (local_50[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_50[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_50[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d06(uVar7);
          }
          *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
               *(int *)(local_188 + 4) + -0x88;
          goto LAB_14003b99d;
        }
      }
    }
    hdc = GetDC((HWND)0x0);
    EnumDisplayMonitors(hdc,(LPCRECT)0x0,thunk_FUN_14003c210,param_1);
    ReleaseDC((HWND)0x0,hdc);
    if (hDesktop != (HDESK)0x0) {
      SetThreadDesktop(hDesktop);
    }
    if (hDesktop_00 != (HDESK)0x0) {
      CloseDesktop(hDesktop_00);
    }
    *(undefined4 *)(param_1 + 0x78) = 0;
  } while( true );
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003bcb0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_14003bcb0(longlong param_1,HMONITOR param_2,undefined8 param_3,int *param_4)

{
  char cVar1;
  BOOL BVar2;
  DWORD DVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar5;
  QString *pQVar6;
  ulonglong uVar7;
  undefined1 auStack_218 [32];
  undefined4 local_1f8;
  undefined4 local_1e8 [2];
  QString local_1e0 [8];
  QRect local_1d8 [16];
  undefined ***local_1c8 [2];
  undefined8 local_1b8;
  int iStack_1ac;
  undefined *local_1a8;
  undefined **local_1a0;
  basic_ostream<char,std::char_traits<char>_> local_198 [96];
  undefined8 local_138;
  undefined4 local_130;
  basic_ios<char,std::char_traits<char>_> local_120 [104];
  ulonglong local_b8 [3];
  ulonglong local_a0;
  tagMONITORINFO local_98;
  undefined1 local_70 [72];
  ulonglong local_28;
  
  local_1b8 = 0xfffffffffffffffe;
  local_28 = DAT_14036a720 ^ (ulonglong)auStack_218;
  local_1e8[0] = 0;
  memset(&local_98,0,0x68);
  local_98.cbSize = 0x68;
  BVar2 = GetMonitorInfoW(param_2,&local_98);
  if (BVar2 == 0) {
    uVar4 = thunk_FUN_1400c9380(local_1c8);
    cVar1 = thunk_FUN_1400c8f40(uVar4,40000);
    thunk_FUN_1400c7da0(local_1c8);
    if (cVar1 != '\0') {
      local_1a8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_120);
      local_1e8[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1a0,false);
      *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
           *(int *)(local_1a8 + 4) + -0x88;
      local_1c8[0] = &local_1a0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1a0);
      local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_138 = 0;
      local_130 = 4;
      DVar3 = GetLastError();
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(&local_1a8,"GetMonitorInfo: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,DVar3);
      uVar4 = thunk_FUN_140030920(&local_1a8,local_b8);
      uVar5 = thunk_FUN_1400c9380(local_1c8);
      local_1f8 = 0x6f;
      thunk_FUN_1400c8570(uVar5,40000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-displayEnumerator\\displayEnumeratorImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_1c8);
      if (0xf < local_a0) {
        uVar7 = local_b8[0];
        if (0xfff < local_a0 + 1) {
          if ((local_b8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_b8[0] - 8);
          if (local_b8[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_b8[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_b8[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar7);
      }
      *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
           *(int *)(local_1a8 + 4) + -0x88;
      local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_1a0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1a0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_198);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_120);
    }
  }
  else {
    QString::QString(local_1e0);
    QRect::QRect(local_1d8);
    pQVar6 = (QString *)QString::fromWCharArray((wchar_t *)local_1e8,(int)local_70);
    QString::operator=(local_1e0,pQVar6);
    QString::~QString((QString *)local_1e8);
    QRect::setLeft(local_1d8,*param_4);
    QRect::setTop(local_1d8,param_4[1]);
    QRect::setRight(local_1d8,param_4[2]);
    QRect::setBottom(local_1d8,param_4[3]);
    if (((byte)local_98.dwFlags & 1) == 0) {
      thunk_FUN_14003b1e0(param_1 + 0x80,local_1e0);
    }
    else {
      thunk_FUN_14003c160();
    }
    QString::~QString(local_1e0);
  }
  return 1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003cbc0
// -----------------------------------------------------------------------------
void FUN_14003cbc0(longlong param_1)

{
  QMutexLocker local_res8 [32];
  
  QMutexLocker::QMutexLocker(local_res8,(QBasicMutex *)(param_1 + 0x10));
  if (*(char *)(param_1 + 0x18) == '\0') {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTracker.cpp"
              ,0x57);
  }
  *(undefined1 *)(param_1 + 0x18) = 0;
  QMutexLocker::~QMutexLocker(local_res8);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003d420
// -----------------------------------------------------------------------------
void FUN_14003d420(longlong param_1)

{
  if (*(char *)(param_1 + 0x18) != '\0') {
    qt_assert("!_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTracker.cpp"
              ,0x44);
  }
  *(undefined1 *)(param_1 + 0x18) = 1;
  *(undefined4 *)(param_1 + 0x1c) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0xffffffffffffffff;
  *(undefined4 *)(param_1 + 0x28) = 0;
  QElapsedTimer::invalidate((QElapsedTimer *)(param_1 + 0x30));
  *(undefined4 *)(param_1 + 0x40) = 0;
  QElapsedTimer::invalidate((QElapsedTimer *)(param_1 + 0x48));
                    /* WARNING: Could not recover jumptable at 0x00014003d487. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  QString::clear((QString *)(param_1 + 0x58));
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003df50
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 * FUN_14003df50(undefined8 *param_1)

{
  char cVar1;
  undefined8 uVar2;
  QString *this;
  QByteArray *this_00;
  undefined8 uVar3;
  ulonglong uVar4;
  char *pcVar5;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  QTypedArrayData<unsigned_short> *local_168;
  undefined ***local_160 [2];
  QByteArray local_150 [8];
  undefined8 local_148;
  undefined8 local_140;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_148 = 0xfffffffffffffffe;
  local_28 = DAT_14036a720 ^ (ulonglong)auStack_198;
  local_168 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_168 & 0xffffffff00000000);
  local_140 = param_1;
  thunk_FUN_14003c430();
  *param_1 = grabberAgent::InputTrackerImpl::vftable;
  param_1[0xf] = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  thunk_FUN_14003f4f0(param_1);
  uVar2 = thunk_FUN_1400c9380(local_160);
  cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
  thunk_FUN_1400c7da0(local_160);
  if (cVar1 != '\0') {
    local_138 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_b0);
    local_168 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_168._4_4_,1);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
         *(int *)(local_138 + 4) + -0x88;
    local_160[0] = &local_130;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_c8 = 0;
    local_c0 = 4;
    local_168 = QString::fromAscii_helper("New super WinAPI ",0x11);
    pcVar5 = "will be used :)";
    if (*(char *)(param_1 + 0x15) != '\0') {
      pcVar5 = "won\'t be used :(";
    }
    this = (QString *)thunk_FUN_14002d820(local_160,&local_168,pcVar5);
    this_00 = (QByteArray *)QString::toLocal8Bit(this);
    pcVar5 = QByteArray::data(this_00);
    thunk_FUN_14002cab0(&local_138,pcVar5);
    QByteArray::~QByteArray(local_150);
    QString::~QString((QString *)local_160);
    QString::~QString((QString *)&local_168);
    uVar2 = thunk_FUN_140030920(&local_138,local_48);
    uVar3 = thunk_FUN_1400c9380(local_160);
    local_178 = 0x37;
    thunk_FUN_1400c8570(uVar3,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTrackerImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_160);
    if (0xf < local_30) {
      uVar4 = local_48[0];
      if (0xfff < local_30 + 1) {
        if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar4 = *(ulonglong *)(local_48[0] - 8);
        if (local_48[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_48[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_48[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar4);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_130);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_128);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
  }
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14003fbe0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14003fbe0(void)

{
  QString *pQVar1;
  int iVar2;
  longlong lVar3;
  Data *pDVar4;
  char cVar5;
  bool bVar6;
  DWORD DVar7;
  BOOL BVar8;
  HDESK hDesktop;
  undefined8 uVar9;
  basic_ostream<char,std::char_traits<char>_> *pbVar10;
  undefined8 uVar11;
  ulonglong uVar12;
  basic_ios<char,std::char_traits<char>_> *this;
  longlong lVar13;
  QTypedArrayData<unsigned_short> **ppQVar15;
  QString *this_00;
  undefined1 auStackY_668 [32];
  Data *local_630;
  QTypedArrayData<unsigned_short> *local_628;
  QTypedArrayData<unsigned_short> *local_620;
  QTypedArrayData<unsigned_short> *local_618;
  QTypedArrayData<unsigned_short> *local_610;
  undefined8 local_608;
  undefined1 local_600 [16];
  undefined1 local_5f0 [16];
  undefined1 local_5e0 [16];
  undefined1 local_5d0 [16];
  undefined1 local_5c0 [16];
  undefined1 local_5b0 [16];
  undefined1 local_5a0 [16];
  undefined1 local_590 [20];
  int iStack_57c;
  undefined *local_578;
  undefined **local_570;
  basic_ostream<char,std::char_traits<char>_> local_568 [96];
  undefined8 local_508;
  undefined4 local_500;
  basic_ios<char,std::char_traits<char>_> local_4f0 [100];
  int iStack_48c;
  undefined *local_488;
  basic_streambuf<char,std::char_traits<char>_> local_480 [128];
  basic_ios<char,std::char_traits<char>_> local_400 [100];
  int iStack_39c;
  undefined *local_398;
  basic_streambuf<char,std::char_traits<char>_> local_390 [128];
  basic_ios<char,std::char_traits<char>_> local_310 [100];
  int iStack_2ac;
  undefined *local_2a8;
  basic_streambuf<char,std::char_traits<char>_> local_2a0 [128];
  basic_ios<char,std::char_traits<char>_> local_220 [104];
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
  longlong lVar14;
  
  local_608 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStackY_668;
  hDesktop = OpenInputDesktop(0,0,8);
  if (hDesktop == (HDESK)0x0) {
    uVar9 = thunk_FUN_1400c9380(local_600);
    cVar5 = thunk_FUN_1400c8f40(uVar9,30000);
    thunk_FUN_1400c7da0(local_600);
    if (cVar5 == '\0') {
      return;
    }
    local_578 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_4f0)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_578,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_570,false);
    *(undefined ***)((longlong)&local_578 + (longlong)*(int *)(local_578 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_57c + (longlong)*(int *)(local_578 + 4)) =
         *(int *)(local_578 + 4) + -0x88;
    local_628 = (QTypedArrayData<unsigned_short> *)&local_570;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_570);
    local_570 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_508 = 0;
    local_500 = 4;
    DVar7 = GetLastError();
    pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(&local_578,"(it) OpenInputDesktop failed: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,DVar7);
    uVar9 = thunk_FUN_140030920(&local_578,local_1b8);
    uVar11 = thunk_FUN_1400c9380(local_5f0);
    thunk_FUN_1400c8570(uVar11,30000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTrackerImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_5f0);
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
      FUN_140002d06(uVar12);
    }
    *(undefined ***)((longlong)&local_578 + (longlong)*(int *)(local_578 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_57c + (longlong)*(int *)(local_578 + 4)) =
         *(int *)(local_578 + 4) + -0x88;
    local_570 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_570);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_570);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_568);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_4f0);
    return;
  }
  BVar8 = GetUserObjectInformationA(hDesktop,2,local_138,0x100,(LPDWORD)&local_610);
  if (BVar8 == 0) {
    uVar9 = thunk_FUN_1400c9380(local_5a0);
    cVar5 = thunk_FUN_1400c8f40(uVar9,30000);
    thunk_FUN_1400c7da0(local_5a0);
    if (cVar5 == '\0') goto LAB_14004044e;
    local_2a8 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_220)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_2a8,local_2a0,false);
    *(undefined ***)(local_2a0 + (longlong)*(int *)(local_2a8 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
         *(int *)(local_2a8 + 4) + -0x88;
    thunk_FUN_14002d040(local_2a0,2);
    DVar7 = GetLastError();
    pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(&local_2a8,"(it) GetUserObjectInformation failed: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,DVar7);
    uVar9 = thunk_FUN_140030920(&local_2a8,local_158);
    uVar11 = thunk_FUN_1400c9380(local_590);
    thunk_FUN_1400c8570(uVar11,30000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTrackerImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_590);
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
      FUN_140002d06(uVar12);
    }
    thunk_FUN_14002d2f0(local_220);
    this = local_220;
  }
  else {
    uVar9 = thunk_FUN_1400c9380(local_5e0);
    cVar5 = thunk_FUN_1400c8f40(uVar9,20000);
    thunk_FUN_1400c7da0(local_5e0);
    if (cVar5 != '\0') {
      local_488 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_400);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_488,local_480,false);
      *(undefined ***)(local_480 + (longlong)*(int *)(local_488 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_48c + (longlong)*(int *)(local_488 + 4)) =
           *(int *)(local_488 + 4) + -0x88;
      thunk_FUN_14002d040(local_480,2);
      uVar9 = thunk_FUN_14002cab0(&local_488,"(it) Desktop switched to ");
      thunk_FUN_14002cab0(uVar9,local_138);
      uVar9 = thunk_FUN_140030920(&local_488,local_198);
      uVar11 = thunk_FUN_1400c9380(local_5d0);
      thunk_FUN_1400c8570(uVar11,20000,uVar9,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTrackerImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_5d0);
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
        FUN_140002d06(uVar12);
      }
      thunk_FUN_14002d2f0(local_400);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_400);
    }
    lVar14 = -1;
    do {
      lVar13 = lVar14 + 1;
      lVar3 = lVar14 + 1;
      lVar14 = lVar13;
    } while (local_138[lVar3] != '\0');
    local_628 = QString::fromAscii_helper(local_138,(int)lVar13);
    local_620 = QString::fromAscii_helper("Winlogon",8);
    local_618 = QString::fromAscii_helper("Screen-saver",0xc);
    local_630 = (Data *)shared_null_exref;
    if (*(int *)(shared_null_exref + 4) < 2) {
      if (*(uint *)shared_null_exref < 2) {
        QListData::realloc((QListData *)&local_630,2);
      }
      else {
        thunk_FUN_14003e930();
      }
    }
    ppQVar15 = &local_620;
    do {
      thunk_FUN_140039960(&local_630,ppQVar15);
      ppQVar15 = ppQVar15 + 1;
    } while (ppQVar15 != &local_610);
    bVar6 = QtPrivate::QStringList_contains((QStringList *)&local_630,(QString *)&local_628,1);
    pDVar4 = local_630;
    if (*(int *)local_630 == 0) {
LAB_1400400b5:
      this_00 = (QString *)(local_630 + ((longlong)*(int *)(local_630 + 0xc) + 2) * 8);
      pQVar1 = (QString *)(local_630 + ((longlong)*(int *)(local_630 + 8) + 2) * 8);
      while (pQVar1 != this_00) {
        this_00 = this_00 + -8;
        QString::~QString(this_00);
      }
      QListData::dispose(pDVar4);
    }
    else if (*(int *)local_630 != -1) {
      LOCK();
      iVar2 = *(int *)local_630;
      *(int *)local_630 = *(int *)local_630 + -1;
      UNLOCK();
      if (iVar2 == 1) goto LAB_1400400b5;
    }
    _eh_vector_destructor_iterator_(&local_620,8,2,~QString_exref);
    QString::~QString((QString *)&local_628);
    if ((bVar6) || (BVar8 = SetThreadDesktop(hDesktop), BVar8 != 0)) goto LAB_14004044e;
    uVar9 = thunk_FUN_1400c9380(local_5c0);
    cVar5 = thunk_FUN_1400c8f40(uVar9,30000);
    thunk_FUN_1400c7da0(local_5c0);
    if (cVar5 == '\0') goto LAB_14004044e;
    local_398 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_310)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_398,local_390,false);
    *(undefined ***)(local_390 + (longlong)*(int *)(local_398 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_39c + (longlong)*(int *)(local_398 + 4)) =
         *(int *)(local_398 + 4) + -0x88;
    thunk_FUN_14002d040(local_390,2);
    DVar7 = GetLastError();
    pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(&local_398,"(it) SetThreadDesktop failed: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,DVar7);
    uVar9 = thunk_FUN_140030920(&local_398,local_178);
    uVar11 = thunk_FUN_1400c9380(local_5b0);
    thunk_FUN_1400c8570(uVar11,30000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-inputTracker\\inputTrackerImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_5b0);
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
      FUN_140002d06(uVar12);
    }
    thunk_FUN_14002d2f0(local_310);
    this = local_310;
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this);
LAB_14004044e:
  CloseDesktop(hDesktop);
  return;
}

// =============================================================================
// PROGRAME SI WEB
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140042c30
// -----------------------------------------------------------------------------
void FUN_140042c30(longlong param_1)

{
  QMutexLocker local_res8 [32];
  
  QMutexLocker::QMutexLocker(local_res8,(QBasicMutex *)(param_1 + 0x10));
  if (*(char *)(param_1 + 0x7a) == '\0') {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumerator.cpp"
              ,0x5f);
  }
  *(undefined1 *)(param_1 + 0x7a) = 0;
  (**(code **)(**(longlong **)(param_1 + 0x50) + 0x60))();
  QTimer::stop((QTimer *)(param_1 + 0x18));
  thunk_FUN_140042820(param_1 + 0x38);
  thunk_FUN_140042760(param_1 + 0x40);
  thunk_FUN_140042760(param_1 + 0x48);
  thunk_FUN_140042760(param_1 + 0x58);
  *(undefined8 *)(param_1 + 0x60) = 0;
  *(undefined2 *)(param_1 + 0x78) = 0;
  QMutexLocker::~QMutexLocker(local_res8);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400439a0
// -----------------------------------------------------------------------------
undefined8 FUN_1400439a0(longlong param_1,int *param_2,uint *param_3)

{
  QHashData *pQVar1;
  int iVar2;
  longlong lVar3;
  int *piVar4;
  QMapDataBase *pQVar5;
  uint *puVar6;
  uint uVar7;
  int iVar8;
  ulonglong uVar9;
  undefined8 uVar10;
  Node *pNVar11;
  QMapDataBase *pQVar12;
  ulonglong *puVar13;
  uint *puVar14;
  uint *puVar15;
  longlong *plVar16;
  Node *pNVar17;
  uint *puVar18;
  QString local_res20 [8];
  Node *local_a8;
  Node *local_a0;
  Node *local_98;
  uint local_90;
  Node *local_88;
  Node *local_80;
  Node *local_78;
  uint local_70;
  QString local_68 [8];
  QString local_60 [8];
  QString local_58 [8];
  QMutexLocker local_50 [8];
  QMutexLocker local_48 [8];
  undefined8 local_40;
  
  local_40 = 0xfffffffffffffffe;
  QMutexLocker::QMutexLocker(local_48,(QBasicMutex *)(param_1 + 0x10));
  if ((*(char *)(param_1 + 0x7a) == '\0') || (*(char *)(param_1 + 0x78) == '\0')) {
    uVar10 = 0;
    goto LAB_1400440ba;
  }
  uVar9 = thunk_FUN_140044bb0(param_1);
  if (*param_3 <= uVar9) {
    *param_3 = 0;
    uVar10 = 1;
    goto LAB_1400440ba;
  }
  *param_2 = *(int *)(*(longlong *)(param_1 + 0x40) + 0x14) +
             *(int *)(*(longlong *)(param_1 + 0x48) + 0x14);
  param_2 = param_2 + 1;
  local_88 = *(Node **)(param_1 + 0x48);
  pNVar17 = local_88 + 0x10;
  if (1 < *(uint *)pNVar17 + 1) {
    LOCK();
    *(uint *)pNVar17 = *(uint *)pNVar17 + 1;
    UNLOCK();
  }
  if ((((byte)local_88[0x28] & 1) == 0) && (1 < *(uint *)pNVar17)) {
    thunk_FUN_140042ee0(&local_88);
  }
  pNVar17 = local_88;
  plVar16 = *(longlong **)(local_88 + 8);
  iVar8 = *(int *)(local_88 + 0x20);
  while (pNVar11 = local_88, iVar8 != 0) {
    iVar8 = iVar8 + -1;
    if ((Node *)*plVar16 != local_88) {
      pNVar11 = (Node *)*plVar16;
      break;
    }
    plVar16 = plVar16 + 1;
  }
  local_70 = 1;
  local_78 = local_88;
  do {
    local_80 = pNVar11;
    if (pNVar11 == pNVar17) break;
    if (local_70 != 0) {
      if (*(longlong *)(pNVar11 + 0x10) == 0) {
        qt_assert("programId",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumerator.cpp"
                  ,0xa4);
      }
      uVar10 = QString::number((__uint64)local_res20,(int)*(undefined8 *)(pNVar11 + 0x10));
      thunk_FUN_1400413c0(param_2,uVar10);
      QString::~QString(local_res20);
      *(undefined1 *)(param_2 + 8) = 0;
      *(undefined1 *)(param_2 + 0x10) = 0;
      *(undefined1 *)(param_2 + 0x410) = 0;
      *(undefined1 *)(param_2 + 0x418) = 0;
      *(undefined1 *)(param_2 + 0x818) = 0;
      param_2 = param_2 + 0xc18;
      local_70 = 0;
    }
    uVar7 = local_70;
    pNVar11 = QHashData::nextNode(pNVar11);
    local_70 = uVar7 ^ 1;
    local_80 = pNVar11;
  } while (local_70 != 0);
  if (*(int *)(pNVar17 + 0x10) == 0) {
LAB_140043b69:
    QHashData::free_helper((QHashData *)pNVar17,thunk_FUN_140042d00);
  }
  else if (*(int *)(pNVar17 + 0x10) != -1) {
    LOCK();
    pNVar17 = pNVar17 + 0x10;
    iVar8 = *(int *)pNVar17;
    *(int *)pNVar17 = *(int *)pNVar17 + -1;
    UNLOCK();
    pNVar17 = local_88;
    if (iVar8 == 1) goto LAB_140043b69;
  }
  local_a8 = *(Node **)(param_1 + 0x40);
  if (1 < *(int *)(local_a8 + 0x10) + 1U) {
    LOCK();
    *(int *)(local_a8 + 0x10) = *(int *)(local_a8 + 0x10) + 1;
    UNLOCK();
  }
  if ((((byte)local_a8[0x28] & 1) == 0) && (1 < *(uint *)(local_a8 + 0x10))) {
    thunk_FUN_140042ee0(&local_a8);
  }
  plVar16 = *(longlong **)(local_a8 + 8);
  iVar8 = *(int *)(local_a8 + 0x20);
  while (local_a0 = local_a8, iVar8 != 0) {
    iVar8 = iVar8 + -1;
    if ((Node *)*plVar16 != local_a8) {
      local_a0 = (Node *)*plVar16;
      break;
    }
    plVar16 = plVar16 + 1;
  }
  local_98 = local_a8;
  local_90 = 1;
  param_2 = param_2 + 0x10;
  do {
    pNVar17 = local_a0;
    if (local_a0 == local_a8) break;
    if (local_90 != 0) {
      if (*(longlong *)(local_a0 + 0x10) == 0) {
        qt_assert("programId",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumerator.cpp"
                  ,0xb4);
      }
      if (1 < **(uint **)(param_1 + 0x38)) {
        pQVar12 = QMapDataBase::createData();
        lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x10);
        if (lVar3 != 0) {
          puVar13 = (ulonglong *)thunk_FUN_140042a10(lVar3,pQVar12);
          *(ulonglong **)(pQVar12 + 0x10) = puVar13;
          *puVar13 = *puVar13 & 3;
          *puVar13 = *puVar13 | (ulonglong)(pQVar12 + 8);
        }
        piVar4 = *(int **)(param_1 + 0x38);
        if (*piVar4 == 0) {
LAB_140043c87:
          pQVar5 = *(QMapDataBase **)(param_1 + 0x38);
          lVar3 = *(longlong *)(pQVar5 + 0x10);
          if (lVar3 != 0) {
            QString::~QString((QString *)(lVar3 + 0x40));
            QString::~QString((QString *)(lVar3 + 0x38));
            QString::~QString((QString *)(lVar3 + 0x30));
            QString::~QString((QString *)(lVar3 + 0x28));
            if (*(longlong *)(lVar3 + 8) != 0) {
              thunk_FUN_140042e40();
            }
            if (*(longlong *)(lVar3 + 0x10) != 0) {
              thunk_FUN_140042e40();
            }
            QMapDataBase::freeTree(pQVar5,*(QMapNodeBase **)(pQVar5 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar5);
        }
        else if (*piVar4 != -1) {
          LOCK();
          iVar8 = *piVar4;
          *piVar4 = *piVar4 + -1;
          UNLOCK();
          if (iVar8 == 1) goto LAB_140043c87;
        }
        *(QMapDataBase **)(param_1 + 0x38) = pQVar12;
        QMapDataBase::recalcMostLeftNode(pQVar12);
      }
      puVar6 = *(uint **)(param_1 + 0x38);
      if (*(uint **)(puVar6 + 4) == (uint *)0x0) {
LAB_140043d43:
        puVar18 = puVar6 + 2;
      }
      else {
        puVar18 = (uint *)0x0;
        uVar9 = *(ulonglong *)(pNVar17 + 0x10);
        puVar14 = *(uint **)(puVar6 + 4);
        do {
          if (*(ulonglong *)(puVar14 + 6) < uVar9) {
            puVar15 = *(uint **)(puVar14 + 4);
          }
          else {
            puVar15 = *(uint **)(puVar14 + 2);
            puVar18 = puVar14;
          }
          puVar14 = puVar15;
        } while (puVar15 != (uint *)0x0);
        if ((puVar18 == (uint *)0x0) || (uVar9 < *(ulonglong *)(puVar18 + 6))) goto LAB_140043d43;
      }
      if (1 < *puVar6) {
        pQVar12 = QMapDataBase::createData();
        lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x10);
        if (lVar3 != 0) {
          puVar13 = (ulonglong *)thunk_FUN_140042a10(lVar3,pQVar12);
          *(ulonglong **)(pQVar12 + 0x10) = puVar13;
          *puVar13 = *puVar13 & 3;
          *puVar13 = *puVar13 | (ulonglong)(pQVar12 + 8);
        }
        piVar4 = *(int **)(param_1 + 0x38);
        if (*piVar4 == 0) {
LAB_140043d98:
          pQVar5 = *(QMapDataBase **)(param_1 + 0x38);
          lVar3 = *(longlong *)(pQVar5 + 0x10);
          if (lVar3 != 0) {
            QString::~QString((QString *)(lVar3 + 0x40));
            QString::~QString((QString *)(lVar3 + 0x38));
            QString::~QString((QString *)(lVar3 + 0x30));
            QString::~QString((QString *)(lVar3 + 0x28));
            if (*(longlong *)(lVar3 + 8) != 0) {
              thunk_FUN_140042e40();
            }
            if (*(longlong *)(lVar3 + 0x10) != 0) {
              thunk_FUN_140042e40();
            }
            QMapDataBase::freeTree(pQVar5,*(QMapNodeBase **)(pQVar5 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar5);
        }
        else if (*piVar4 != -1) {
          LOCK();
          iVar8 = *piVar4;
          *piVar4 = *piVar4 + -1;
          UNLOCK();
          if (iVar8 == 1) goto LAB_140043d98;
        }
        *(QMapDataBase **)(param_1 + 0x38) = pQVar12;
        QMapDataBase::recalcMostLeftNode(pQVar12);
      }
      if ((uint *)(*(longlong *)(param_1 + 0x38) + 8) == puVar18) {
        qt_assert("_programs.end() != iter",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumerator.cpp"
                  ,0xb7);
      }
      if (1 < **(uint **)(param_1 + 0x38)) {
        pQVar12 = QMapDataBase::createData();
        lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x10);
        if (lVar3 != 0) {
          puVar13 = (ulonglong *)thunk_FUN_140042a10(lVar3,pQVar12);
          *(ulonglong **)(pQVar12 + 0x10) = puVar13;
          *puVar13 = *puVar13 & 3;
          *puVar13 = *puVar13 | (ulonglong)(pQVar12 + 8);
        }
        piVar4 = *(int **)(param_1 + 0x38);
        if (*piVar4 == 0) {
LAB_140043e8e:
          pQVar5 = *(QMapDataBase **)(param_1 + 0x38);
          lVar3 = *(longlong *)(pQVar5 + 0x10);
          if (lVar3 != 0) {
            QString::~QString((QString *)(lVar3 + 0x40));
            QString::~QString((QString *)(lVar3 + 0x38));
            QString::~QString((QString *)(lVar3 + 0x30));
            QString::~QString((QString *)(lVar3 + 0x28));
            if (*(longlong *)(lVar3 + 8) != 0) {
              thunk_FUN_140042e40();
            }
            if (*(longlong *)(lVar3 + 0x10) != 0) {
              thunk_FUN_140042e40();
            }
            QMapDataBase::freeTree(pQVar5,*(QMapNodeBase **)(pQVar5 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar5);
        }
        else if (*piVar4 != -1) {
          LOCK();
          iVar8 = *piVar4;
          *piVar4 = *piVar4 + -1;
          UNLOCK();
          if (iVar8 == 1) goto LAB_140043e8e;
        }
        *(QMapDataBase **)(param_1 + 0x38) = pQVar12;
        QMapDataBase::recalcMostLeftNode(pQVar12);
      }
      if ((uint *)(*(longlong *)(param_1 + 0x38) + 8) == puVar18) {
        uVar10 = QString::number((__uint64)local_58,(int)*(undefined8 *)(pNVar17 + 0x10));
        thunk_FUN_1400413c0(param_2 + -0x10,uVar10);
        QString::~QString(local_58);
        *(undefined1 *)(param_2 + -8) = 0;
        *(undefined1 *)param_2 = 0;
        *(undefined1 *)(param_2 + 0x400) = 0;
        *(undefined1 *)(param_2 + 0x408) = 0;
        *(undefined1 *)(param_2 + 0x808) = 0;
      }
      else {
        uVar10 = QString::number((__uint64)local_68,(int)*(undefined8 *)(puVar18 + 8));
        thunk_FUN_1400413c0(param_2 + -0x10,uVar10);
        QString::~QString(local_68);
        uVar10 = QString::number((__uint64)local_60,(int)*(undefined8 *)(puVar18 + 8));
        thunk_FUN_1400413c0(param_2 + -8,uVar10);
        QString::~QString(local_60);
        thunk_FUN_140041280(param_2,puVar18 + 10);
        thunk_FUN_1400413c0(param_2 + 0x400,puVar18 + 0xc);
        thunk_FUN_140041280(param_2 + 0x408,puVar18 + 0xe);
        thunk_FUN_140041280(param_2 + 0x808,puVar18 + 0x10);
      }
      param_2 = param_2 + 0xc18;
      local_90 = 0;
    }
    uVar7 = local_90;
    local_a0 = QHashData::nextNode(local_a0);
    local_90 = uVar7 ^ 1;
  } while (local_90 != 0);
  if (*(int *)(local_a8 + 0x10) == 0) {
LAB_140044045:
    QHashData::free_helper((QHashData *)local_a8,thunk_FUN_140042d00);
  }
  else if (*(int *)(local_a8 + 0x10) != -1) {
    LOCK();
    pQVar1 = (QHashData *)(local_a8 + 0x10);
    iVar8 = *(int *)pQVar1;
    *(int *)pQVar1 = *(int *)pQVar1 + -1;
    UNLOCK();
    if (iVar8 == 1) goto LAB_140044045;
  }
  QMutexLocker::QMutexLocker(local_50,(QBasicMutex *)(param_1 + 0x10));
  iVar8 = *(int *)(*(longlong *)(param_1 + 0x48) + 0x14);
  iVar2 = *(int *)(*(longlong *)(param_1 + 0x40) + 0x14);
  QMutexLocker::~QMutexLocker(local_50);
  *param_3 = *param_3 - ((iVar2 + iVar8) * 0x3060 + 4);
  thunk_FUN_140042760(param_1 + 0x48);
  thunk_FUN_140042760(param_1 + 0x40);
  thunk_FUN_140098f80(*(undefined8 *)(param_1 + 0x50));
  *(undefined1 *)(param_1 + 0x78) = 0;
  uVar10 = 1;
LAB_1400440ba:
  QMutexLocker::~QMutexLocker(local_48);
  return uVar10;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140044570
// -----------------------------------------------------------------------------
void FUN_140044570(longlong param_1)

{
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  
  if (*(char *)(param_1 + 0x7a) != '\0') {
    qt_assert("!_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumerator.cpp"
              ,0x52);
  }
  *(undefined1 *)(param_1 + 0x7a) = 1;
  (**(code **)(**(longlong **)(param_1 + 0x50) + 0x58))();
  local_f8 = 0;
  uStack_f0 = 0;
  local_58 = 0;
  uStack_50 = 0;
  local_e8 = 0;
  uStack_e0 = 0;
  local_d8 = 0;
  uStack_d0 = 0;
  local_c8 = 0;
  uStack_c0 = 0;
  local_b8 = 0;
  uStack_b0 = 0;
  local_a8 = 0;
  uStack_a0 = 0;
  local_98 = 0;
  uStack_90 = 0;
  local_88 = 0;
  uStack_80 = 0;
  local_78 = 0;
  uStack_70 = 0;
  local_68 = 0;
  uStack_60 = 0;
  QMetaObject::invokeMethod
            (param_1,"onUpdateForcer",2,&local_58,&local_68,&local_78,&local_88,&local_98,&local_a8,
             &local_b8,&local_c8,&local_d8,&local_e8,&local_f8);
                    /* WARNING: Could not recover jumptable at 0x000140044701. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  QTimer::start((QTimer *)(param_1 + 0x18));
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140046a20
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140046a20(longlong param_1)

{
  char cVar1;
  DWORD DVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined4 local_168 [4];
  undefined8 local_158;
  int iStack_14c;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_158 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_198;
  local_168[0] = 0;
  QMutexLocker::QMutexLocker((QMutexLocker *)local_168,(QBasicMutex *)(param_1 + 0x18));
  *(undefined1 *)(param_1 + 0x10) = 1;
  QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 0x20));
  QMutexLocker::~QMutexLocker((QMutexLocker *)local_168);
  DVar2 = WaitForSingleObject(*(HANDLE *)(param_1 + 8),0xfa);
  if (DVar2 != 0) {
    uVar3 = thunk_FUN_1400c9380(local_168);
    cVar1 = thunk_FUN_1400c8f40(uVar3);
    thunk_FUN_1400c7da0(local_168);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      thunk_FUN_14002cab0(&local_148,"(pe) foreground window thread wait timed out");
      uVar3 = thunk_FUN_140030920(&local_148,local_58);
      uVar4 = thunk_FUN_1400c9380(local_168);
      local_178 = 0x60;
      thunk_FUN_1400c8570(uVar4,30000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_168);
      if (0xf < local_40) {
        uVar5 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar5);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 0x20));
    WaitForSingleObject(*(HANDLE *)(param_1 + 8),0xffffffff);
  }
  CloseHandle(*(HANDLE *)(param_1 + 8));
  QWaitCondition::~QWaitCondition((QWaitCondition *)(param_1 + 0x28));
  QWaitCondition::~QWaitCondition((QWaitCondition *)(param_1 + 0x20));
  QMutex::~QMutex((QMutex *)(param_1 + 0x18));
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140047f70
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140047f70(undefined8 *param_1)

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
  local_38 = DAT_14036a720 ^ (ulonglong)auStackY_7a8;
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
      uVar8 = thunk_FUN_1400c9380(local_6a8);
      cVar5 = thunk_FUN_1400c8f40(uVar8,30000);
      thunk_FUN_1400c7da0(local_6a8);
      if (cVar5 != '\0') {
        local_2c8 = &DAT_1402738f0;
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
                 thunk_FUN_14002cab0(&local_2c8,"(pe) OpenInputDesktop failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
        uVar8 = thunk_FUN_140030920(&local_2c8,local_158);
        uVar10 = thunk_FUN_1400c9380(local_698);
        thunk_FUN_1400c8570(uVar10,30000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_698);
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
          FUN_140002d06(uVar12);
        }
        *(undefined ***)((longlong)&local_2c8 + (longlong)*(int *)(local_2c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_2cc + (longlong)*(int *)(local_2c8 + 4)) =
             *(int *)(local_2c8 + 4) + -0x88;
        local_2c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_2c0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2c0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_2b8);
        this = local_240;
LAB_140048c1a:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this)
        ;
      }
    }
    else {
      BVar7 = GetUserObjectInformationA(hDesktop_00,2,local_138,0x100,(LPDWORD)&local_740);
      if (BVar7 == 0) {
        uVar8 = thunk_FUN_1400c9380(local_6e8);
        cVar5 = thunk_FUN_1400c8f40(uVar8,30000);
        thunk_FUN_1400c7da0(local_6e8);
        if (cVar5 != '\0') {
          local_4a8 = &DAT_1402738f0;
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
                   thunk_FUN_14002cab0(&local_4a8,"(pe) GetUserObjectInformation failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
          uVar8 = thunk_FUN_140030920(&local_4a8,local_198);
          uVar10 = thunk_FUN_1400c9380(local_6d8);
          thunk_FUN_1400c8570(uVar10,30000,uVar8,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_6d8);
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
            FUN_140002d06(uVar12);
          }
          *(undefined ***)((longlong)&local_4a8 + (longlong)*(int *)(local_4a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_4ac + (longlong)*(int *)(local_4a8 + 4)) =
               *(int *)(local_4a8 + 4) + -0x88;
          local_4a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14002e520(&local_4a0);
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
            thunk_FUN_14003e930();
          }
        }
        ppQVar15 = &local_750;
        do {
          thunk_FUN_140039960(&local_770,ppQVar15);
          ppQVar15 = ppQVar15 + 1;
        } while (ppQVar15 != &local_740);
        bVar4 = QtPrivate::QStringList_contains((QStringList *)&local_770,(QString *)&local_758,1);
        pDVar3 = local_770;
        if (*(int *)local_770 == 0) {
LAB_14004814c:
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
          if (*(int *)local_770 == 0) goto LAB_14004814c;
        }
        _eh_vector_destructor_iterator_(&local_750,8,2,~QString_exref);
        QString::~QString((QString *)&local_758);
        if (!bVar4) {
          BVar7 = SetThreadDesktop(hDesktop_00);
          if (BVar7 == 0) {
            uVar8 = thunk_FUN_1400c9380(local_728);
            cVar5 = thunk_FUN_1400c8f40(uVar8,30000);
            thunk_FUN_1400c7da0(local_728);
            if (cVar5 != '\0') {
              local_688 = &DAT_1402738f0;
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
              uVar8 = thunk_FUN_14002cab0(&local_688,"(pe) SetThreadDesktop to ");
              uVar8 = thunk_FUN_14002cab0(uVar8,local_138);
              pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_14002cab0(uVar8," failed: ");
              std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
              uVar8 = thunk_FUN_140030920(&local_688,local_1d8);
              uVar10 = thunk_FUN_1400c9380(local_718);
              thunk_FUN_1400c8570(uVar10,30000,uVar8,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                                 );
              thunk_FUN_1400c7da0(local_718);
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
                FUN_140002d06(uVar12);
              }
              *(undefined ***)((longlong)&local_688 + (longlong)*(int *)(local_688 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_68c + (longlong)*(int *)(local_688 + 4)) =
                   *(int *)(local_688 + 4) + -0x88;
              local_680 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                          vftable;
              thunk_FUN_14002e520(&local_680);
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
            goto LAB_14004880f;
          }
          uVar8 = thunk_FUN_1400c9380(local_708);
          cVar5 = thunk_FUN_1400c8f40(uVar8,30000);
          thunk_FUN_1400c7da0(local_708);
          if (cVar5 != '\0') {
            local_598 = &DAT_1402738f0;
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
                     thunk_FUN_14002cab0(&local_598,"(pe) SetThreadDesktop(prevDesk) failed: ");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
            uVar8 = thunk_FUN_140030920(&local_598,local_1b8);
            uVar10 = thunk_FUN_1400c9380(local_6f8);
            thunk_FUN_1400c8570(uVar10,30000,uVar8,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                               );
            thunk_FUN_1400c7da0(local_6f8);
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
              FUN_140002d06(uVar12);
            }
            *(undefined ***)((longlong)&local_598 + (longlong)*(int *)(local_598 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_59c + (longlong)*(int *)(local_598 + 4)) =
                 *(int *)(local_598 + 4) + -0x88;
            local_590 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14002e520(&local_590);
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
LAB_14004880f:
      BVar7 = CloseDesktop(hDesktop_00);
      if (BVar7 == 0) {
        uVar8 = thunk_FUN_1400c9380(local_6c8);
        cVar5 = thunk_FUN_1400c8f40(uVar8,30000);
        thunk_FUN_1400c7da0(local_6c8);
        if (cVar5 != '\0') {
          local_3b8 = &DAT_1402738f0;
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
                   thunk_FUN_14002cab0(&local_3b8,"(pe) CloseDesktop failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
          uVar8 = thunk_FUN_140030920(&local_3b8,local_178);
          uVar10 = thunk_FUN_1400c9380(local_6b8);
          thunk_FUN_1400c8570(uVar10,30000,uVar8,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_6b8);
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
            FUN_140002d06(uVar12);
          }
          *(undefined ***)((longlong)&local_3b8 + (longlong)*(int *)(local_3b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_3bc + (longlong)*(int *)(local_3b8 + 4)) =
               *(int *)(local_3b8 + 4) + -0x88;
          local_3b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14002e520(&local_3b0);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_3b0);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_3a8);
          this = local_330;
          goto LAB_140048c1a;
        }
      }
    }
    QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 5));
    QMutexLocker::~QMutexLocker(local_760);
    QMutexLocker::QMutexLocker(local_760,(QBasicMutex *)pQVar16);
  } while( true );
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140049080
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140049080(longlong param_1)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158;
  undefined ***local_150 [2];
  QMutexLocker local_140 [8];
  undefined8 local_138;
  int iStack_12c;
  undefined *local_128;
  undefined **local_120;
  basic_ostream<char,std::char_traits<char>_> local_118 [96];
  undefined8 local_b8;
  undefined4 local_b0;
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_138 = 0xfffffffffffffffe;
  local_18 = DAT_14036a720 ^ (ulonglong)auStack_188;
  local_158 = 0;
  QMutexLocker::QMutexLocker(local_140,(QBasicMutex *)(param_1 + 0x18));
  QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 0x20));
  bVar1 = QWaitCondition::wait((QWaitCondition *)(param_1 + 0x28),(QMutex *)(param_1 + 0x18),0xfa);
  if (!bVar1) {
    uVar3 = thunk_FUN_1400c9380(local_150);
    cVar2 = thunk_FUN_1400c8f40(uVar3,30000);
    thunk_FUN_1400c7da0(local_150);
    if (cVar2 != '\0') {
      local_128 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_158 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_120,false);
      *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      local_150[0] = &local_120;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_120);
      local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_b8 = 0;
      local_b0 = 4;
      thunk_FUN_14002cab0(&local_128,"(pe) foreground window update timed out");
      uVar3 = thunk_FUN_140030920(&local_128,local_38);
      uVar4 = thunk_FUN_1400c9380(local_150);
      local_168 = 0x6e;
      thunk_FUN_1400c8570(uVar4,30000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-programEnumerator\\programEnumeratorImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_150);
      if (0xf < local_20) {
        uVar5 = local_38[0];
        if (0xfff < local_20 + 1) {
          if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_38[0] - 8);
          if (local_38[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_38[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_38[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar5);
      }
      *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_120);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_120);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_118);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a0);
    }
  }
  QMutexLocker::~QMutexLocker(local_140);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140098a30
// -----------------------------------------------------------------------------
void FUN_140098a30(QObject *param_1)

{
  QObject *pQVar1;
  code *local_res8;
  code *local_res10;
  QMutexLocker local_res18 [16];
  
  QMutexLocker::QMutexLocker(local_res18,(QBasicMutex *)(param_1 + 0x10));
  if (param_1[0x18] == (QObject)0x0) {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-web\\manager.cpp",0x57
             );
  }
  param_1[0x18] = (QObject)0x0;
  thunk_FUN_140098750(param_1 + 0x20);
  thunk_FUN_1400986a0(param_1 + 0x28);
  local_res8 = thunk_FUN_140099db0;
  local_res10 = thunk_FUN_140095b50;
  pQVar1 = (QObject *)thunk_FUN_1400911c0();
  QObject::disconnectImpl(pQVar1,&local_res10,param_1,&local_res8,(QMetaObject *)&DAT_14036a240);
  QMutexLocker::~QMutexLocker(local_res18);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14009a160
// -----------------------------------------------------------------------------
QString * FUN_14009a160(longlong param_1,QString *param_2,ulonglong param_3,QString *param_4)

{
  uint *puVar1;
  longlong *plVar2;
  uint *puVar3;
  uint *puVar4;
  undefined8 uVar5;
  QString *this;
  uint *puVar6;
  QMutexLocker local_res8 [8];
  QString local_30 [8];
  undefined8 local_28;
  
  local_28 = 0xfffffffffffffffe;
  puVar6 = (uint *)0x0;
  QMutexLocker::QMutexLocker(local_res8,(QBasicMutex *)(param_1 + 0x10));
  if (*(char *)(param_1 + 0x18) == '\0') {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-web\\manager.cpp",0xc0
             );
  }
  if (1 < **(uint **)(param_1 + 0x20)) {
    thunk_FUN_140098e50(param_1 + 0x20);
  }
  puVar1 = *(uint **)(param_1 + 0x20);
  puVar3 = *(uint **)(puVar1 + 4);
  if (*(uint **)(puVar1 + 4) != (uint *)0x0) {
    do {
      if (*(ulonglong *)(puVar3 + 6) < param_3) {
        puVar4 = *(uint **)(puVar3 + 4);
      }
      else {
        puVar4 = *(uint **)(puVar3 + 2);
        puVar6 = puVar3;
      }
      puVar3 = puVar4;
    } while (puVar4 != (uint *)0x0);
    if ((puVar6 != (uint *)0x0) && (*(ulonglong *)(puVar6 + 6) <= param_3)) goto LAB_14009a208;
  }
  puVar6 = puVar1 + 2;
LAB_14009a208:
  if (1 < *puVar1) {
    thunk_FUN_140098e50(param_1 + 0x20);
  }
  if ((uint *)(*(longlong *)(param_1 + 0x20) + 8) == puVar6) {
    QString::QString(param_2);
  }
  else {
    plVar2 = *(longlong **)(puVar6 + 8);
    uVar5 = QString::QString(local_30,param_4);
    (**(code **)(*plVar2 + 0x60))(plVar2,uVar5);
    (**(code **)(**(longlong **)(puVar6 + 8) + 0x58))();
    this = (QString *)thunk_FUN_14009e060(*(undefined8 *)(puVar6 + 8),local_30);
    QString::trimmed(this);
    QString::~QString(local_30);
  }
  QMutexLocker::~QMutexLocker(local_res8);
  QString::~QString(param_4);
  return param_2;
}

// =============================================================================
// CAPTURA ECRAN SI STARE SESIUNE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14004d970
// -----------------------------------------------------------------------------
void FUN_14004d970(longlong param_1)

{
  if (*(char *)(param_1 + 0x30) == '\0') {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
              ,0x8b);
    *(undefined1 *)(param_1 + 0x30) = 0;
    return;
  }
  *(undefined1 *)(param_1 + 0x30) = 0;
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14004df20
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14004df20(longlong param_1,QImage *param_2,undefined4 param_3)

{
  undefined1 uVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  Format FVar5;
  int iVar6;
  int iVar7;
  QMapNodeBase *this;
  undefined8 uVar8;
  longlong lVar9;
  QDateTime *pQVar10;
  uint *puVar11;
  QImage *pQVar12;
  uchar *puVar13;
  basic_ostream<char,std::char_traits<char>_> *pbVar14;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar15;
  undefined8 uVar16;
  uint uVar17;
  ulonglong uVar18;
  uint uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  double dVar22;
  undefined1 auStack_218 [32];
  undefined8 local_1f8;
  uint local_1e8;
  uint local_1e0 [2];
  undefined ***local_1d8 [4];
  undefined4 local_1b8;
  QImage local_1b0 [36];
  int iStack_18c;
  undefined *local_188;
  undefined **local_180;
  basic_ostream<char,std::char_traits<char>_> local_178 [96];
  undefined8 local_118;
  undefined4 local_110;
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  undefined8 local_98;
  ulonglong local_90 [3];
  ulonglong local_78;
  QImage local_70 [32];
  ulonglong local_50;
  
  local_98 = 0xfffffffffffffffe;
  local_50 = DAT_14036a720 ^ (ulonglong)auStack_218;
  local_1e8 = 0;
  local_1b8 = param_3;
  if (*(longlong *)(param_1 + 0x10) == 0) {
    qt_assert("_screenshootData",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
              ,0x12e);
  }
  bVar2 = QImage::isNull(param_2);
  if (((bVar2) || (iVar4 = QImage::width(param_2), iVar4 < 1)) ||
     (iVar4 = QImage::height(param_2), iVar4 < 1)) {
    uVar8 = thunk_FUN_1400c9380(local_1d8);
    cVar3 = thunk_FUN_1400c8f40(uVar8,30000);
    thunk_FUN_1400c7da0(local_1d8);
    if (cVar3 == '\0') {
      return;
    }
    local_188 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    local_1e8 = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_1d8[0] = &local_180;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_118 = 0;
    local_110 = 4;
    thunk_FUN_14002cab0(&local_188,"invalid source QImage in screen shooter");
    uVar8 = thunk_FUN_140030920(&local_188,local_90);
    uVar16 = thunk_FUN_1400c9380(local_1d8);
    local_1f8 = CONCAT44(local_1f8._4_4_,0x132);
    thunk_FUN_1400c8570(uVar16,30000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                       );
    thunk_FUN_1400c7da0(local_1d8);
    if (0xf < local_78) {
      uVar18 = local_90[0];
      if (0xfff < local_78 + 1) {
        if ((local_90[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar18 = *(ulonglong *)(local_90[0] - 8);
        if (local_90[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_90[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_90[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar18);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_180);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_178);
LAB_14004eb3c:
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_100);
    return;
  }
  FVar5 = QImage::format(param_2);
  if ((0xd < FVar5) || ((0x2810U >> (FVar5 & 0x1f) & 1) == 0)) {
    uVar8 = thunk_FUN_1400c9380(local_1d8);
    cVar3 = thunk_FUN_1400c8f40(uVar8,30000);
    thunk_FUN_1400c7da0(local_1d8);
    if (cVar3 == '\0') {
      return;
    }
    local_188 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    local_1e8 = 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    thunk_FUN_14002d040(&local_180,2);
    iVar4 = QImage::height(param_2);
    iVar6 = QImage::width(param_2);
    FVar5 = QImage::format(param_2);
    uVar8 = thunk_FUN_14002cab0(&local_188,"unsupported QImage format in screen shooter");
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(uVar8,", format: ")
    ;
    pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,FVar5);
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(pbVar15,", width: ");
    pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,iVar6);
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(pbVar15,", height: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,iVar4);
    uVar8 = thunk_FUN_140030920(&local_188,local_70);
    uVar16 = thunk_FUN_1400c9380(local_1d8);
    local_1f8 = CONCAT44(local_1f8._4_4_,0x140);
    thunk_FUN_1400c8570(uVar16,30000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                       );
    thunk_FUN_1400c7da0(local_1d8);
    thunk_FUN_14002d390(local_70);
    thunk_FUN_14002d2f0(local_100);
    goto LAB_14004eb3c;
  }
  iVar4 = QImage::height(param_2);
  iVar6 = QImage::width(param_2);
  uVar18 = (longlong)iVar6 * (longlong)iVar4;
  iVar4 = *(int *)(*(longlong *)(param_1 + 0x28) + 4);
  while (0x32 < iVar4) {
    if (1 < **(uint **)(param_1 + 0x28)) {
      thunk_FUN_14004db70(param_1 + 0x28);
    }
    lVar9 = *(longlong *)(param_1 + 0x28);
    if (*(longlong *)(lVar9 + 0x10) == 0) {
      this = (QMapNodeBase *)(lVar9 + 8);
    }
    else {
      this = *(QMapNodeBase **)(lVar9 + 0x20);
    }
    if (1 < **(uint **)(param_1 + 0x28)) {
      thunk_FUN_14004db70(param_1 + 0x28);
    }
    lVar9 = *(longlong *)(param_1 + 0x28);
    QString::QString((QString *)local_1d8);
    QDateTime::currentDateTimeUtc();
    for (; this != (QMapNodeBase *)(lVar9 + 8); this = QMapNodeBase::nextNode(this)) {
      bVar2 = QDateTime::operator<((QDateTime *)(this + 0x50),(QDateTime *)local_1e0);
      if (bVar2) {
        QDateTime::operator=((QDateTime *)local_1e0,(QDateTime *)(this + 0x50));
        QString::operator=((QString *)local_1d8,(QString *)(this + 0x18));
      }
    }
    thunk_FUN_14004fe70(param_1 + 0x28,local_1d8);
    QDateTime::~QDateTime((QDateTime *)local_1e0);
    QString::~QString((QString *)local_1d8);
    iVar4 = *(int *)(*(longlong *)(param_1 + 0x28) + 4);
  }
  uVar8 = thunk_FUN_14004fd60(local_1d8,*(undefined8 *)(param_1 + 0x10));
  lVar9 = thunk_FUN_14004d390(param_1 + 0x28,uVar8);
  QString::~QString((QString *)local_1d8);
  pQVar10 = (QDateTime *)QDateTime::currentDateTimeUtc();
  QDateTime::operator=((QDateTime *)(lVar9 + 0x30),pQVar10);
  QDateTime::~QDateTime((QDateTime *)local_1e0);
  if (*(longlong *)(param_1 + 8) == 0) {
    qt_assert("_screenshootId",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
              ,0xd2);
  }
  local_1e0[0] = *(uint *)(*(longlong *)(param_1 + 8) + 0x404);
  uVar19 = 0x1fa400;
  if (local_1e0[0] != 0) {
    puVar11 = local_1e0;
    if (0x1fa3ff < local_1e0[0]) {
      puVar11 = &DAT_14029ebd0;
    }
    uVar19 = *puVar11;
  }
  if (uVar18 < uVar19 || uVar18 - uVar19 == 0) {
    uVar20 = SUB84(DAT_14029f2c0,0);
    uVar21 = (undefined4)((ulonglong)DAT_14029f2c0 >> 0x20);
  }
  else {
    dVar22 = (double)(longlong)uVar18;
    if ((longlong)uVar18 < 0) {
      dVar22 = dVar22 + DAT_14029f2d0;
    }
    dVar22 = sqrt((double)uVar19 / dVar22);
    uVar20 = SUB84(dVar22,0);
    uVar21 = (undefined4)((ulonglong)dVar22 >> 0x20);
  }
  FVar5 = QImage::format(param_2);
  if ((FVar5 == 0xb) && ((double)CONCAT44(uVar21,uVar20) == DAT_14029f2c0)) {
    pQVar12 = (QImage *)QImage::QImage(local_70,param_2);
    uVar17 = 1;
  }
  else {
    local_1f8 = CONCAT44(uVar21,uVar20);
    pQVar12 = (QImage *)thunk_FUN_140139510(lVar9,local_90,param_2,0xb);
    uVar17 = 2;
  }
  QImage::QImage(local_1b0,pQVar12);
  if ((uVar17 & 2) != 0) {
    uVar17 = uVar17 & 0xfffffffd;
    QImage::~QImage((QImage *)local_90);
  }
  if ((uVar17 & 1) != 0) {
    uVar17 = uVar17 & 0xfffffffe;
    QImage::~QImage(local_70);
  }
  bVar2 = QImage::isNull(local_1b0);
  if (!bVar2) {
    pQVar12 = (QImage *)FUN_14004ff90(local_1d8,local_1b0);
    QImage::operator=(local_1b0,pQVar12);
    QImage::~QImage((QImage *)local_1d8);
  }
  bVar2 = QImage::isNull(local_1b0);
  if ((bVar2) || (FVar5 = QImage::format(local_1b0), FVar5 != 0xb)) {
    uVar8 = thunk_FUN_1400c9380(local_1d8);
    cVar3 = thunk_FUN_1400c8f40(uVar8,30000);
    thunk_FUN_1400c7da0(local_1d8);
    if (cVar3 == '\0') goto LAB_14004e80a;
    local_188 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    local_1e8 = uVar17 | 0x10;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_1d8[0] = &local_180;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_118 = 0;
    local_110 = 4;
    iVar4 = QImage::height(param_2);
    iVar6 = QImage::width(param_2);
    iVar7 = QImage::depth(param_2);
    uVar8 = thunk_FUN_14002cab0(&local_188,"unable to normalize image in screen shooter");
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(uVar8,", sourceBpp: ");
    pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,iVar7);
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(pbVar15,", sourceWidth: ");
    pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,iVar6);
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(pbVar15,", sourceHeight: ");
    pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,iVar4);
    pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(pbVar15,", maxArea: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,uVar19);
    uVar8 = thunk_FUN_140030920(&local_188,local_90);
    uVar16 = thunk_FUN_1400c9380(local_1d8);
    local_1f8 = CONCAT44(local_1f8._4_4_,0x16c);
    thunk_FUN_1400c8570(uVar16,30000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                       );
    thunk_FUN_1400c7da0(local_1d8);
    if (0xf < local_78) {
      uVar18 = local_90[0];
      if (0xfff < local_78 + 1) {
        if ((local_90[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar18 = *(ulonglong *)(local_90[0] - 8);
        if (local_90[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_90[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_90[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar18);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_180);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_178);
LAB_14004e7ff:
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_100);
    goto LAB_14004e80a;
  }
  iVar4 = QImage::byteCount(local_1b0);
  uVar18 = (longlong)iVar4 + 0x41c;
  if (uVar18 < **(uint **)(param_1 + 0x18)) {
    if (uVar18 < 0x100000000) {
      iVar4 = QImage::width(local_1b0);
      *(int *)(*(longlong *)(param_1 + 0x10) + 0x408) = iVar4;
      iVar4 = QImage::height(local_1b0);
      *(int *)(*(longlong *)(param_1 + 0x10) + 0x40c) = iVar4;
      iVar4 = QImage::depth(local_1b0);
      *(int *)(*(longlong *)(param_1 + 0x10) + 0x410) = iVar4;
      iVar4 = QImage::byteCount(local_1b0);
      *(int *)(*(longlong *)(param_1 + 0x10) + 0x414) = iVar4;
      lVar9 = *(longlong *)(param_1 + 0x10);
      puVar13 = QImage::constBits(local_1b0);
      if (puVar13 != (uchar *)(lVar9 + 0x418)) {
        uVar19 = *(uint *)(lVar9 + 0x414);
        puVar13 = QImage::constBits(local_1b0);
        memcpy((void *)(*(longlong *)(param_1 + 0x10) + 0x418),puVar13,(ulonglong)uVar19);
      }
      **(int **)(param_1 + 0x18) = **(int **)(param_1 + 0x18) - (int)uVar18;
      iVar4 = *(int *)(*(longlong *)(param_1 + 0x10) + 0x410);
      if (((iVar4 - 0x10U & 0xffffffe7) == 0) && (iVar4 != 0x28)) goto LAB_14004e80a;
      uVar8 = thunk_FUN_1400c9380(local_1d8);
      cVar3 = thunk_FUN_1400c8f40(uVar8,30000);
      thunk_FUN_1400c7da0(local_1d8);
      if (cVar3 == '\0') goto LAB_14004e80a;
      local_188 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_100);
      local_1e8 = uVar17 | 0x20;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
      *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
           *(int *)(local_188 + 4) + -0x88;
      thunk_FUN_14002d040(&local_180);
      lVar9 = *(longlong *)(param_1 + 0x10);
      uVar19 = *(uint *)(lVar9 + 0x40c);
      if (uVar19 == 0) {
        uVar17 = 0;
      }
      else {
        uVar17 = *(uint *)(lVar9 + 0x414) / uVar19;
      }
      uVar1 = *(undefined1 *)(lVar9 + 0x400);
      uVar8 = thunk_FUN_14002cab0(&local_188,"downscaler produced unexpected bpp in screen shooter")
      ;
      pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(uVar8,", bpp: ");
      pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar14,*(uint *)(lVar9 + 0x410));
      pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(pbVar15,", width: ");
      pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar14,*(uint *)(lVar9 + 0x408));
      pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(pbVar15,", height: ");
      pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,uVar19);
      pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(pbVar15,", dataSize: ");
      pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar14,*(uint *)(lVar9 + 0x414));
      pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(pbVar15,", bytesPerLine: ");
      pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar14,uVar17);
      uVar8 = thunk_FUN_14002cab0(pbVar15,", isMonitorId: ");
      uVar8 = thunk_FUN_14004c7c0(uVar8,uVar1);
      lVar9 = *(longlong *)(param_1 + 0x10);
      pbVar14 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(uVar8,", maxArea: ");
      pbVar15 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar14,*(uint *)(lVar9 + 0x404));
      uVar8 = thunk_FUN_14002cab0(pbVar15,", monitorOrWindowId: ");
      thunk_FUN_14002cab0(uVar8,lVar9);
      uVar8 = thunk_FUN_140030920(&local_188,local_70);
      uVar16 = thunk_FUN_1400c9380(local_1d8);
      local_1f8 = CONCAT44(local_1f8._4_4_,399);
      thunk_FUN_1400c8570(uVar16,30000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                         );
      thunk_FUN_1400c7da0(local_1d8);
      thunk_FUN_14002d390(local_70);
      thunk_FUN_14002d2f0(local_100);
      goto LAB_14004e7ff;
    }
LAB_14004e5bb:
    uVar18 = 0xffffffff;
  }
  else if (0xffffffff < uVar18) goto LAB_14004e5bb;
  *(int *)(param_1 + 0x20) = (int)uVar18;
  **(uint **)(param_1 + 0x18) = 0;
LAB_14004e80a:
  QImage::~QImage(local_1b0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14004ee90
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14004ee90(undefined ***param_1,uint param_2,uint param_3,uint param_4,uchar *param_5,
                  uint param_6)

{
  undefined1 uVar1;
  undefined **ppuVar2;
  undefined ***pppuVar3;
  char cVar4;
  bool bVar5;
  uint uVar6;
  undefined8 uVar7;
  basic_ostream<char,std::char_traits<char>_> *pbVar8;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar9;
  undefined8 uVar10;
  ulonglong uVar11;
  Format FVar12;
  undefined1 auStackY_1e8 [32];
  undefined1 local_1a0 [16];
  undefined8 local_190;
  undefined *local_188;
  undefined **local_180;
  basic_ostream<char,std::char_traits<char>_> local_178 [96];
  undefined8 local_118;
  undefined4 local_110;
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  undefined8 local_98;
  QImage local_90 [32];
  ulonglong local_70 [3];
  ulonglong local_58;
  ulonglong local_50;
  
  local_98 = 0xfffffffffffffffe;
  local_50 = DAT_14036a720 ^ (ulonglong)auStackY_1e8;
  local_190 = param_1;
  if (param_1[2] == (undefined **)0x0) {
    qt_assert("_screenshootData",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
              ,0xdc);
  }
  if ((((param_5 == (uchar *)0x0) || (param_2 == 0)) || (param_3 == 0)) || (param_6 < param_3)) {
    uVar7 = thunk_FUN_1400c9380(local_1a0);
    cVar4 = thunk_FUN_1400c8f40(uVar7,30000);
    thunk_FUN_1400c7da0(local_1a0);
    if (cVar4 == '\0') {
      return;
    }
    local_188 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_190 + (longlong)*(int *)(local_188 + 4) + 4) =
         *(int *)(local_188 + 4) + -0x88;
    local_190 = &local_180;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_118 = 0;
    local_110 = 4;
    uVar7 = thunk_FUN_14002cab0(&local_188,"invalid source image in screen shooter");
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(uVar7,", bpp: ");
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_4);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(pbVar9,", width: ");
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_2);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(pbVar9,", height: ")
    ;
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_3);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(pbVar9,", dataSize: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_6);
    uVar7 = thunk_FUN_140030920(&local_188,local_70);
    uVar10 = thunk_FUN_1400c9380(local_1a0);
    thunk_FUN_1400c8570(uVar10,30000,uVar7,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                       );
    thunk_FUN_1400c7da0(local_1a0);
    if (0xf < local_58) {
      uVar11 = local_70[0];
      if (0xfff < local_58 + 1) {
        if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar11 = *(ulonglong *)(local_70[0] - 8);
        if (local_70[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_70[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_70[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar11);
    }
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_190 + (longlong)*(int *)(local_188 + 4) + 4) =
         *(int *)(local_188 + 4) + -0x88;
    local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_180);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_178);
  }
  else {
    if (param_4 == 0x10) {
      FVar12 = 0xb;
    }
    else if (param_4 == 0x18) {
      FVar12 = 0xd;
    }
    else {
      if (param_4 != 0x20) {
        uVar7 = thunk_FUN_1400c9380(local_1a0);
        cVar4 = thunk_FUN_1400c8f40(uVar7,30000);
        thunk_FUN_1400c7da0(local_1a0);
        if (cVar4 == '\0') {
          return;
        }
        local_188 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_100);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
        *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_190 + (longlong)*(int *)(local_188 + 4) + 4) =
             *(int *)(local_188 + 4) + -0x88;
        thunk_FUN_14002d040(&local_180,2);
        pppuVar3 = local_190;
        uVar1 = *(undefined1 *)(local_190[2] + 0x80);
        uVar6 = FUN_14004d6c0(param_3,param_6);
        uVar7 = thunk_FUN_14002cab0(&local_188,"unknown bpp in screen shooter");
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(uVar7,", bpp: ")
        ;
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_4);
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(pbVar9,", width: ");
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_2);
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(pbVar9,", height: ");
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_3);
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(pbVar9,", dataSize: ");
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_6);
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(pbVar9,", bytesPerLine: ");
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,uVar6);
        uVar7 = thunk_FUN_14002cab0(pbVar9,", isMonitorId: ");
        uVar7 = thunk_FUN_14004c7c0(uVar7,uVar1);
        ppuVar2 = pppuVar3[2];
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(uVar7,", maxArea: ");
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                           (pbVar8,*(uint *)((longlong)ppuVar2 + 0x404));
        uVar7 = thunk_FUN_14002cab0(pbVar9,", monitorOrWindowId: ");
        thunk_FUN_14002cab0(uVar7,ppuVar2);
        uVar7 = thunk_FUN_140030920(&local_188,local_70);
        uVar10 = thunk_FUN_1400c9380(local_1a0);
        thunk_FUN_1400c8570(uVar10,30000,uVar7,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                           );
        thunk_FUN_1400c7da0(local_1a0);
        thunk_FUN_14002d390(local_70);
        thunk_FUN_14002d2f0(local_100);
        goto LAB_14004f75f;
      }
      FVar12 = 4;
    }
    if (0x7fffffff < param_2) {
      return;
    }
    if (0x7fffffff < param_3) {
      return;
    }
    uVar11 = (ulonglong)param_6 / (ulonglong)param_3;
    if (((ulonglong)param_4 * (ulonglong)param_2 + 7 >> 3 <= uVar11) && (uVar11 < 0x80000000)) {
      QImage::QImage(local_90,param_5,param_2,param_3,(int)uVar11,FVar12,(_func_void_void_ptr *)0x0,
                     (void *)0x0);
      bVar5 = QImage::isNull(local_90);
      if (bVar5) {
        uVar7 = thunk_FUN_1400c9380(local_1a0);
        cVar4 = thunk_FUN_1400c8f40(uVar7,30000);
        thunk_FUN_1400c7da0(local_1a0);
        if (cVar4 != '\0') {
          local_188 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_100);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
          *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_190 + (longlong)*(int *)(local_188 + 4) + 4) =
               *(int *)(local_188 + 4) + -0x88;
          thunk_FUN_14002d040(&local_180,2);
          uVar6 = FUN_14004d6c0(param_3,param_6);
          uVar7 = thunk_FUN_14002cab0(&local_188,
                                      "unable to construct source image in screen shooter");
          pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(uVar7,", bpp: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_4);
          pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(pbVar9,", width: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_2);
          pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(pbVar9,", height: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_3);
          pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(pbVar9,", dataSize: ");
          pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_6);
          pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(pbVar9,", bytesPerLine: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,uVar6);
          uVar7 = thunk_FUN_140030920(&local_188,local_70);
          uVar10 = thunk_FUN_1400c9380(local_1a0);
          thunk_FUN_1400c8570(uVar10,30000,uVar7,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                             );
          thunk_FUN_1400c7da0(local_1a0);
          thunk_FUN_14002d390(local_70);
          thunk_FUN_14002d2f0(local_100);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_100);
        }
      }
      else {
        thunk_FUN_14004df20(param_1,local_90,0);
      }
      QImage::~QImage(local_90);
      return;
    }
    uVar7 = thunk_FUN_1400c9380(local_1a0,(ulonglong)param_6 % (ulonglong)param_3);
    cVar4 = thunk_FUN_1400c8f40(uVar7,30000);
    thunk_FUN_1400c7da0(local_1a0);
    if (cVar4 == '\0') {
      return;
    }
    local_188 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
    *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_190 + (longlong)*(int *)(local_188 + 4) + 4) =
         *(int *)(local_188 + 4) + -0x88;
    thunk_FUN_14002d040(&local_180,2);
    uVar7 = thunk_FUN_14002cab0(&local_188,"invalid source stride in screen shooter");
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(uVar7,", bpp: ");
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_4);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(pbVar9,", width: ");
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_2);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(pbVar9,", height: ")
    ;
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_3);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(pbVar9,", dataSize: ");
    pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_6);
    pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(pbVar9,", bytesPerLine: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,uVar11);
    uVar7 = thunk_FUN_140030920(&local_188,local_70);
    uVar10 = thunk_FUN_1400c9380(local_1a0);
    thunk_FUN_1400c8570(uVar10,30000,uVar7,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooter.cpp"
                       );
    thunk_FUN_1400c7da0(local_1a0);
    thunk_FUN_14002d390(local_70);
    thunk_FUN_14002d2f0(local_100);
  }
LAB_14004f75f:
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_100);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140051330
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140051330(longlong param_1)

{
  char cVar1;
  DWORD DVar2;
  BOOL BVar3;
  HDESK hDesktop;
  HDESK hDesktop_00;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  HDC hdc;
  ulonglong uVar7;
  uint uVar8;
  undefined1 auStack_1f8 [32];
  undefined4 local_1d8;
  uint local_1c8;
  undefined8 local_1c0;
  undefined *local_1b8;
  undefined **local_1b0;
  basic_ostream<char,std::char_traits<char>_> local_1a8 [96];
  HDESK local_148;
  undefined4 local_140;
  basic_ios<char,std::char_traits<char>_> local_130 [104];
  QMutexLocker local_c8 [8];
  undefined8 local_c0;
  undefined1 local_b8 [16];
  undefined1 local_a8 [16];
  undefined1 local_98 [16];
  undefined1 local_88 [16];
  undefined1 local_78 [16];
  undefined1 local_68 [16];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_c0 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_1f8;
  uVar8 = 0;
  local_1c8 = 0;
  QMutexLocker::QMutexLocker(local_c8,(QBasicMutex *)(param_1 + 0x90));
  do {
    QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 0x98));
    do {
      QWaitCondition::wait((QWaitCondition *)(param_1 + 0x98),(QMutex *)(param_1 + 0x90),0xffffffff)
      ;
      if (*(int *)(param_1 + 0xa0) == 2) {
        QMutexLocker::~QMutexLocker(local_c8);
        return 0;
      }
    } while (*(int *)(param_1 + 0xa0) != 1);
    DVar2 = GetCurrentThreadId();
    hDesktop = GetThreadDesktop(DVar2);
    hDesktop_00 = OpenInputDesktop(0,0,0x10000000);
    if (hDesktop_00 == (HDESK)0x0) {
      uVar4 = thunk_FUN_1400c9380(local_b8);
      cVar1 = thunk_FUN_1400c8f40(uVar4);
      thunk_FUN_1400c7da0();
      if (cVar1 != '\0') {
        local_1b8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_130);
        uVar8 = uVar8 | 1;
        local_1c8 = uVar8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
        *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
             *(int *)(local_1b8 + 4) + -0x88;
        local_1c0 = &local_1b0;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
        local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_140 = 4;
        local_148 = hDesktop_00;
        DVar2 = GetLastError();
        pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_1b8,"(ss) OpenInputDesktop failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar2);
        uVar4 = thunk_FUN_140030920(&local_1b8,local_58);
        uVar6 = thunk_FUN_1400c9380(local_a8);
        local_1d8 = 0x188;
        thunk_FUN_1400c8570(uVar6,30000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_a8);
        if (0xf < local_40) {
          uVar7 = local_58[0];
          if (0xfff < local_40 + 1) {
            if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_58[0] - 8);
            if (local_58[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_58[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_58[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar7);
        }
        *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
             *(int *)(local_1b8 + 4) + -0x88;
LAB_14005174e:
        local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_1b0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_1a8);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_130);
      }
    }
    else {
      BVar3 = SetThreadDesktop(hDesktop_00);
      if (BVar3 == 0) {
        uVar4 = thunk_FUN_1400c9380(local_98);
        cVar1 = thunk_FUN_1400c8f40(uVar4);
        thunk_FUN_1400c7da0();
        if (cVar1 != '\0') {
          local_1b8 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_130);
          uVar8 = uVar8 | 2;
          local_1c8 = uVar8;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
          *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
               *(int *)(local_1b8 + 4) + -0x88;
          local_1c0 = &local_1b0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
          local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_148 = (HDESK)0x0;
          local_140 = 4;
          DVar2 = GetLastError();
          pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(&local_1b8,"(ss) SetThreadDesktop failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar2);
          uVar4 = thunk_FUN_140030920(&local_1b8,local_58);
          uVar6 = thunk_FUN_1400c9380(local_88);
          local_1d8 = 0x18c;
          thunk_FUN_1400c8570(uVar6,30000,uVar4,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_88);
          if (0xf < local_40) {
            uVar7 = local_58[0];
            if (0xfff < local_40 + 1) {
              if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar7 = *(ulonglong *)(local_58[0] - 8);
              if (local_58[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_58[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_58[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d06(uVar7);
          }
          *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
               *(int *)(local_1b8 + 4) + -0x88;
          goto LAB_14005174e;
        }
      }
    }
    hdc = GetDC((HWND)0x0);
    EnumDisplayMonitors(hdc,(LPCRECT)0x0,thunk_FUN_1400524c0,param_1);
    ReleaseDC((HWND)0x0,hdc);
    if (*(char *)(param_1 + 0xa8) == '\0') {
      uVar4 = thunk_FUN_1400c9380(local_78);
      cVar1 = thunk_FUN_1400c8f40(uVar4,30000);
      thunk_FUN_1400c7da0(local_78);
      if (cVar1 != '\0') {
        local_1b8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_130);
        uVar8 = uVar8 | 4;
        local_1c8 = uVar8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
        *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
             *(int *)(local_1b8 + 4) + -0x88;
        local_1c0 = &local_1b0;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
        local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_148 = (HDESK)0x0;
        local_140 = 4;
        uVar4 = thunk_FUN_14002cab0(&local_1b8,"gdi monitor not found for display capture");
        uVar4 = thunk_FUN_14002cab0(uVar4,", requested: ");
        thunk_FUN_14002cab0(uVar4,*(undefined8 *)(param_1 + 8));
        uVar4 = thunk_FUN_140030920(&local_1b8,local_58);
        uVar6 = thunk_FUN_1400c9380(local_68);
        local_1d8 = 0x196;
        thunk_FUN_1400c8570(uVar6,30000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_68);
        if (0xf < local_40) {
          uVar7 = local_58[0];
          if (0xfff < local_40 + 1) {
            if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_58[0] - 8);
            if (local_58[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_58[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_58[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar7);
        }
        *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
             *(int *)(local_1b8 + 4) + -0x88;
        local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_1b0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_1a8);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_130);
      }
    }
    if (hDesktop != (HDESK)0x0) {
      SetThreadDesktop(hDesktop);
    }
    if (hDesktop_00 != (HDESK)0x0) {
      CloseDesktop(hDesktop_00);
    }
    *(undefined4 *)(param_1 + 0xa0) = 0;
  } while( true );
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140052550
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140052550(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  QHashData *this;
  bool bVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  __int64 _Var7;
  longlong *plVar8;
  undefined8 *puVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  QString *pQVar12;
  QByteArray *pQVar13;
  char *pcVar14;
  basic_ostream<char,std::char_traits<char>_> *pbVar15;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar16;
  undefined8 uVar17;
  longlong lVar18;
  __uint64 _Var19;
  undefined1 auStack_1f8 [32];
  undefined4 local_1d8;
  QTypedArrayData<unsigned_short> *local_1c8;
  undefined8 *local_1c0 [2];
  int local_1b0 [2];
  undefined4 local_1a8 [2];
  QMutexLocker local_1a0 [8];
  undefined8 local_198;
  int iStack_18c;
  undefined *local_188;
  basic_streambuf<char,std::char_traits<char>_> local_180 [128];
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  undefined1 local_98 [32];
  undefined **local_78;
  longlong local_70;
  undefined ***local_40;
  ulonglong local_38;
  
  local_198 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_1f8;
  local_1a8[0] = 0;
  pcVar14 = *(char **)(param_1 + 8);
  if (pcVar14[0x400] == '\0') {
    lVar18 = -1;
    if (pcVar14 != (char *)0x0) {
      do {
        lVar18 = lVar18 + 1;
      } while (pcVar14[lVar18] != '\0');
    }
    local_1c8 = QString::fromAscii_helper(pcVar14,(int)lVar18);
    _Var19 = QString::toULongLong((QString *)&local_1c8,(bool *)0x0,10);
    thunk_FUN_140054340(param_1,_Var19);
    goto LAB_140052bb9;
  }
  QString::fromUtf8((char *)&local_1c8,(int)pcVar14);
  _Var7 = QElapsedTimer::elapsed((QElapsedTimer *)(param_1 + 0x70));
  plVar1 = (longlong *)(param_1 + 0x80);
  if (*(int *)(*plVar1 + 0x20) != 0) {
    qHash((QString *)&local_1c8,*(uint *)(*plVar1 + 0x24));
  }
  plVar8 = (longlong *)thunk_FUN_140051c40(plVar1,&local_1c8);
  lVar18 = *plVar8;
  lVar2 = *plVar1;
  if ((lVar18 == lVar2) || (*(longlong *)(lVar18 + 0x18) <= _Var7)) {
    bVar3 = true;
  }
  else {
    bVar3 = false;
  }
  cVar4 = thunk_FUN_1400582b0(param_1 + 0x38);
  if ((cVar4 != '\0') && (bVar3)) {
    local_1b0[0] = 0;
    local_78 = std::
               _Func_impl<<lambda_1b8defbf6051d7ede5785380a21caf5e>,std::allocator<int>,void,grabberAgent::screenShooterImpl_win::Dx112::Frame_const&___ptr64>
               ::vftable;
    local_40 = &local_78;
    local_70 = param_1;
    iVar5 = thunk_FUN_140059cb0(param_1 + 0x38,&local_1c8,&local_78,local_1b0);
    if (local_40 != (undefined ***)0x0) {
      (*(code *)(*local_40)[4])(local_40,local_40 != &local_78);
      local_40 = (undefined ***)0x0;
    }
    if (iVar5 == 0) {
      if (lVar18 != lVar2) {
        thunk_FUN_140052380(plVar1,&local_1c8);
        uVar11 = thunk_FUN_1400c9380(local_1c0);
        cVar4 = thunk_FUN_1400c8f40(uVar11,20000);
        thunk_FUN_1400c7da0(local_1c0);
        if (cVar4 != '\0') {
          local_188 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_100);
          local_1a8[0] = 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_188,local_180,false);
          *(undefined ***)(local_180 + (longlong)*(int *)(local_188 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
               *(int *)(local_188 + 4) + -0x88;
          thunk_FUN_14002d040(local_180,2);
          pQVar12 = (QString *)QString::QString((QString *)local_1a8,(QString *)&local_1c8);
          pQVar13 = (QByteArray *)QString::toLocal8Bit(pQVar12);
          pcVar14 = QByteArray::constData(pQVar13);
          uVar11 = thunk_FUN_14002cab0(&local_188,"dxgi capture restored after gdi fallback");
          uVar11 = thunk_FUN_14002cab0(uVar11,", display: ");
          uVar11 = thunk_FUN_14002cab0(uVar11,pcVar14);
          pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_14002cab0(uVar11,", format: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,local_1b0[0]);
          QByteArray::~QByteArray((QByteArray *)local_1c0);
          QString::~QString((QString *)local_1a8);
          uVar11 = thunk_FUN_140030920(&local_188,local_98);
          uVar17 = thunk_FUN_1400c9380(local_1c0);
          local_1d8 = 0x87;
          thunk_FUN_1400c8570(uVar17,20000,uVar11,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_1c0);
          thunk_FUN_14002d390(local_98);
          thunk_FUN_14002d2f0(local_100);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_100);
        }
      }
      goto LAB_140052bb9;
    }
    if (iVar5 == 2) {
LAB_1400526dc:
      if (lVar18 == lVar2) goto LAB_140052b25;
    }
    else {
      if (iVar5 != 3) {
        if (iVar5 != 4) {
          qt_assert("0",
                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                    ,0xa1);
          goto LAB_140052b25;
        }
        goto LAB_1400526dc;
      }
      if (lVar18 == lVar2) {
        uVar11 = thunk_FUN_1400c9380(local_1c0);
        cVar4 = thunk_FUN_1400c8f40(uVar11,30000);
        thunk_FUN_1400c7da0(local_1c0);
        if (cVar4 != '\0') {
          local_188 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_100);
          local_1a8[0] = 2;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_188,local_180,false);
          *(undefined ***)(local_180 + (longlong)*(int *)(local_188 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
               *(int *)(local_188 + 4) + -0x88;
          thunk_FUN_14002d040(local_180,2);
          pQVar12 = (QString *)QString::QString((QString *)local_1c0,(QString *)&local_1c8);
          pQVar13 = (QByteArray *)QString::toLocal8Bit(pQVar12);
          pcVar14 = QByteArray::constData(pQVar13);
          uVar11 = thunk_FUN_14002cab0(&local_188,
                                       "unsupported dxgi display format, switching to gdi fallback")
          ;
          uVar11 = thunk_FUN_14002cab0(uVar11,", display: ");
          uVar11 = thunk_FUN_14002cab0(uVar11,pcVar14);
          pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_14002cab0(uVar11,", format: ");
          pbVar16 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                              (pbVar15,local_1b0[0]);
          pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_14002cab0(pbVar16,", probeIntervalMs: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,60000);
          QByteArray::~QByteArray((QByteArray *)local_1a8);
          QString::~QString((QString *)local_1c0);
          uVar11 = thunk_FUN_140030920(&local_188,local_98);
          uVar17 = thunk_FUN_1400c9380(local_1c0);
          local_1d8 = 0x90;
          thunk_FUN_1400c8570(uVar17,30000,uVar11,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_1c0);
          thunk_FUN_14002d390(local_98);
          thunk_FUN_14002d2f0(local_100);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_100);
        }
      }
    }
    if (1 < *(uint *)(*plVar1 + 0x10)) {
      thunk_FUN_140051240(plVar1);
    }
    uVar6 = qHash((QString *)&local_1c8,*(uint *)(*plVar1 + 0x24));
    puVar9 = (undefined8 *)thunk_FUN_140051c40(plVar1,&local_1c8,uVar6);
    this = (QHashData *)*plVar1;
    if ((QHashData *)*puVar9 == this) {
      if (*(int *)(this + 0x20) <= *(int *)(this + 0x14)) {
        QHashData::rehash(this,*(short *)(this + 0x1e) + 1);
        uVar6 = qHash((QString *)&local_1c8,*(uint *)(*plVar1 + 0x24));
        puVar9 = (undefined8 *)thunk_FUN_140051c40(plVar1,&local_1c8);
      }
      puVar10 = QHashData::allocateNode((QHashData *)*plVar1,8);
      local_1c0[0] = puVar10;
      if (puVar10 == (undefined8 *)0x0) {
        puVar10 = (undefined8 *)0x0;
      }
      else {
        *puVar10 = *puVar9;
        *(uint *)(puVar10 + 1) = uVar6;
        QString::QString((QString *)(puVar10 + 2),(QString *)&local_1c8);
        puVar10[3] = _Var7 + 60000;
      }
      *puVar9 = puVar10;
      *(int *)(*plVar1 + 0x14) = *(int *)(*plVar1 + 0x14) + 1;
    }
    else {
      *(__int64 *)((QHashData *)*puVar9 + 0x18) = _Var7 + 60000;
    }
  }
LAB_140052b25:
  QMutexLocker::QMutexLocker(local_1a0,(QBasicMutex *)(param_1 + 0x90));
  *(undefined1 *)(param_1 + 0xa8) = 0;
  *(undefined4 *)(param_1 + 0xa0) = 1;
  QWaitCondition::wakeAll((QWaitCondition *)(param_1 + 0x98));
  QWaitCondition::wait((QWaitCondition *)(param_1 + 0x98),(QMutex *)(param_1 + 0x90),0xffffffff);
  QMutexLocker::~QMutexLocker(local_1a0);
LAB_140052bb9:
  QString::~QString((QString *)&local_1c8);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140052da0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140052da0(longlong param_1,HDC param_2,HWND param_3,int *param_4)

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
  char *lpvBits;
  ulonglong uVar10;
  UINT cLines;
  int iVar11;
  undefined1 auStackY_628 [32];
  HWND local_5d0 [2];
  HGDIOBJ local_5c0;
  int local_5b8;
  undefined8 local_5b0;
  undefined *local_5a8;
  undefined **local_5a0;
  basic_ostream<char,std::char_traits<char>_> local_598 [96];
  HBITMAP local_538;
  undefined4 local_530;
  basic_ios<char,std::char_traits<char>_> local_520 [104];
  longlong local_4b8;
  undefined8 local_4b0;
  ulonglong local_4a8 [3];
  ulonglong local_490;
  tagBITMAPINFO local_488 [24];
  ulonglong local_58;
  
  local_4b0 = 0xfffffffffffffffe;
  local_58 = DAT_14036a720 ^ (ulonglong)auStackY_628;
  iVar11 = param_4[2] - *param_4;
  cLines = param_4[3] - param_4[1];
  local_5d0[0] = param_3;
  local_5b8 = iVar11;
  local_4b8 = param_1;
  hbm = CreateCompatibleBitmap(param_2,iVar11,cLines);
  if (hbm == (HBITMAP)0x0) {
    uVar6 = thunk_FUN_1400c9380(local_5d0);
    cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
    thunk_FUN_1400c7da0(local_5d0);
    if (cVar3 == '\0') {
      return;
    }
    local_5a8 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_520)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
    *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
         *(int *)(local_5a8 + 4) + -0x88;
    local_5d0[0] = (HWND)&local_5a0;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_5a0);
    local_5a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_530 = 4;
    local_538 = hbm;
    DVar4 = GetLastError();
    pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(&local_5a8,"CreateCompatibleBitmap: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
    uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
    uVar8 = thunk_FUN_1400c9380(local_5d0);
    thunk_FUN_1400c8570(uVar8,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_5d0);
    if (0xf < local_490) {
      uVar10 = local_4a8[0];
      if (0xfff < local_490 + 1) {
        if ((local_4a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_4a8[0] - 8);
        if (local_4a8[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_4a8[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_4a8[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar10);
    }
    *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
         *(int *)(local_5a8 + 4) + -0x88;
    local_5a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_5a0);
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
    uVar6 = thunk_FUN_1400c9380(local_5d0);
    cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
    thunk_FUN_1400c7da0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002d040(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_5a8,"CreateCompatibleDC: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
      uVar8 = thunk_FUN_1400c9380(local_5d0);
      thunk_FUN_1400c8570(uVar8,40000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_5d0);
      if (0xf < local_490) {
        uVar10 = local_4a8[0];
        if (0xfff < local_490 + 1) {
          if ((local_4a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_4a8[0] - 8);
          if (local_4a8[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_4a8[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_4a8[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar10);
      }
      thunk_FUN_14002d2f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
    }
    DeleteObject(hbm);
    return;
  }
  local_5c0 = SelectObject(hdc,hbm);
  if (0xfffffffffffffffd < (longlong)local_5c0 - 1U) {
    uVar6 = thunk_FUN_1400c9380(local_5d0);
    cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
    thunk_FUN_1400c7da0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002d040(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_5a8,"SelectObject: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
      uVar8 = thunk_FUN_1400c9380(local_5d0);
      thunk_FUN_1400c8570(uVar8,40000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_5d0);
      if (0xf < local_490) {
        uVar10 = local_4a8[0];
        if (0xfff < local_490 + 1) {
          if ((local_4a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_4a8[0] - 8);
          if (local_4a8[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_4a8[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_4a8[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar10);
      }
      thunk_FUN_14002d2f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
    }
    goto LAB_140053eb1;
  }
  if (local_5d0[0] == (HWND)0x0) {
    BVar5 = BitBlt(hdc,0,0,iVar11,cLines,param_2,*param_4,param_4[1],0xcc0020);
    if (BVar5 != 0) goto LAB_140053448;
    uVar6 = thunk_FUN_1400c9380(local_5d0);
    cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
    thunk_FUN_1400c7da0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002d040(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_5a8,"BitBlt: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
      uVar8 = thunk_FUN_1400c9380(local_5d0);
      thunk_FUN_1400c8570(uVar8,40000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_5d0);
      thunk_FUN_14002d390(local_4a8);
      goto LAB_1400532da;
    }
    goto LAB_1400532ed;
  }
  BVar5 = PrintWindow(local_5d0[0],hdc,*(UINT *)(local_4b8 + 0xa4));
  if (BVar5 == 0) {
    uVar6 = thunk_FUN_1400c9380(local_5d0);
    cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
    thunk_FUN_1400c7da0(local_5d0);
    if (cVar3 == '\0') goto LAB_1400532ed;
    local_5a8 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_520)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
    *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
         *(int *)(local_5a8 + 4) + -0x88;
    thunk_FUN_14002d040(&local_5a0,2);
    DVar4 = GetLastError();
    pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(&local_5a8,"PrintWindow: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
    uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
    uVar8 = thunk_FUN_1400c9380(local_5d0);
    thunk_FUN_1400c8570(uVar8,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_5d0);
    thunk_FUN_14002d390(local_4a8);
LAB_1400532da:
    thunk_FUN_14002d2f0(local_520);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_520);
  }
  else {
LAB_140053448:
    memset((void *)((longlong)&local_488[0].bmiHeader.biSize + 1),0,0x427);
    local_488[0].bmiHeader.biSize = 0x28;
    local_488[0].bmiHeader.biPlanes = 1;
    local_488[0].bmiHeader.biBitCount = 0;
    local_488[0].bmiHeader.biCompression = 0;
    local_488[0].bmiHeader.biWidth = iVar11;
    local_488[0].bmiHeader.biHeight = cLines;
    iVar11 = GetDIBits(hdc,hbm,0,cLines,(LPVOID)0x0,local_488,0);
    if (iVar11 == 0) {
      uVar6 = thunk_FUN_1400c9380(local_5d0);
      cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
      thunk_FUN_1400c7da0(local_5d0);
      if (cVar3 == '\0') goto LAB_1400532ed;
      local_5a8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002d040(&local_5a0,2);
      DVar4 = GetLastError();
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_5a8,"GetDIBits: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
      uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
      uVar8 = thunk_FUN_1400c9380(local_5d0);
      thunk_FUN_1400c8570(uVar8,40000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_5d0);
      thunk_FUN_14002d390(local_4a8);
      goto LAB_1400532da;
    }
    if ((local_488[0].bmiHeader.biBitCount == 0x10) ||
       ((local_488[0].bmiHeader.biBitCount - 0x18 & 0xfffffff7) == 0)) {
LAB_140053a38:
      local_488[0].bmiHeader.biHeight = -cLines;
      local_488[0].bmiHeader._12_8_ = local_488[0].bmiHeader._12_8_ & 0xffffffff;
      QByteArray::QByteArray((QByteArray *)&local_5b0);
      QByteArray::resize((QByteArray *)&local_5b0,local_488[0].bmiHeader.biSizeImage);
      if (*(int *)(local_5b0 + 4) == 0) {
        uVar6 = thunk_FUN_1400c9380(local_5d0);
        cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
        thunk_FUN_1400c7da0(local_5d0);
        if (cVar3 == '\0') goto LAB_140053d0b;
        local_5a8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_520);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
        *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
             *(int *)(local_5a8 + 4) + -0x88;
        thunk_FUN_14002d040(&local_5a0,2);
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_5a8,"unable to allocate ");
        pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                           (pbVar7,local_488[0].bmiHeader.biSizeImage);
        thunk_FUN_14002cab0(pbVar9," bytes for GDI screen capture");
        uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
        uVar8 = thunk_FUN_1400c9380(local_5d0);
        thunk_FUN_1400c8570(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_5d0);
        thunk_FUN_14002d390(local_4a8);
      }
      else {
        lpvBits = QByteArray::data((QByteArray *)&local_5b0);
        iVar11 = GetDIBits(hdc,hbm,0,cLines,lpvBits,local_488,0);
        if (iVar11 != 0) {
          QByteArray::constData((QByteArray *)&local_5b0);
          thunk_FUN_14004ee90(local_4b8,local_5b8,cLines,local_488[0].bmiHeader.biBitCount);
          goto LAB_140053d0b;
        }
        uVar6 = thunk_FUN_1400c9380(local_5d0);
        cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
        thunk_FUN_1400c7da0(local_5d0);
        if (cVar3 == '\0') goto LAB_140053d0b;
        local_5a8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_520);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
        *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
             *(int *)(local_5a8 + 4) + -0x88;
        thunk_FUN_14002d040(&local_5a0,2);
        DVar4 = GetLastError();
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_5a8,"GetDIBits: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
        uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
        uVar8 = thunk_FUN_1400c9380(local_5d0);
        thunk_FUN_1400c8570(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_5d0);
        thunk_FUN_14002d390(local_4a8);
      }
      thunk_FUN_14002d2f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
LAB_140053d0b:
      SelectObject(hdc,local_5c0);
      DeleteDC(hdc);
      DeleteObject(hbm);
      QByteArray::~QByteArray((QByteArray *)&local_5b0);
      return;
    }
    uVar6 = thunk_FUN_1400c9380(local_5d0);
    cVar3 = thunk_FUN_1400c8f40(uVar6,30000);
    thunk_FUN_1400c7da0(local_5d0);
    if (cVar3 != '\0') {
      local_5a8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_520);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
      *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
           *(int *)(local_5a8 + 4) + -0x88;
      thunk_FUN_14002d040(&local_5a0,2);
      uVar1 = *(undefined1 *)(*(longlong *)(local_4b8 + 0x10) + 0x400);
      uVar6 = thunk_FUN_14002cab0(&local_5a8,"unexpected gdi biBitCount before fallback");
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(uVar6,", biBitCount: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar7,local_488[0].bmiHeader.biBitCount);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar9,", biCompression: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar7,local_488[0].bmiHeader.biCompression);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar9,", biSizeImage: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar7,local_488[0].bmiHeader.biSizeImage);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar9,", width: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,local_5b8);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar9,", height: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,cLines);
      uVar6 = thunk_FUN_14002cab0(pbVar9,", isMonitorId: ");
      uVar6 = thunk_FUN_14004c7c0(uVar6,uVar1);
      lVar2 = *(longlong *)(local_4b8 + 0x10);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(uVar6,", maxArea: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar7,*(uint *)(lVar2 + 0x404));
      uVar6 = thunk_FUN_14002cab0(pbVar9,", monitorOrWindowId: ");
      thunk_FUN_14002cab0(uVar6,lVar2);
      uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
      uVar8 = thunk_FUN_1400c9380(local_5d0);
      thunk_FUN_1400c8570(uVar8,30000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_5d0);
      thunk_FUN_14002d390(local_4a8);
      thunk_FUN_14002d2f0(local_520);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_520);
    }
    local_488[0].bmiHeader.biBitCount = 0x10;
    local_488[0].bmiHeader.biCompression = 0;
    iVar11 = GetDIBits(hdc,hbm,0,cLines,(LPVOID)0x0,local_488,0);
    if (iVar11 == 0) {
      uVar6 = thunk_FUN_1400c9380(local_5d0);
      cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
      thunk_FUN_1400c7da0(local_5d0);
      if (cVar3 != '\0') {
        local_5a8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_520);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
        *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
             *(int *)(local_5a8 + 4) + -0x88;
        thunk_FUN_14002d040(&local_5a0,2);
        DVar4 = GetLastError();
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_5a8,"GetDIBits: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,DVar4);
        uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
        uVar8 = thunk_FUN_1400c9380(local_5d0);
        thunk_FUN_1400c8570(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_5d0);
        thunk_FUN_14002d390(local_4a8);
        goto LAB_1400532da;
      }
    }
    else {
      if (local_488[0].bmiHeader.biBitCount == 0x10) goto LAB_140053a38;
      uVar6 = thunk_FUN_1400c9380(local_5d0);
      cVar3 = thunk_FUN_1400c8f40(uVar6,40000);
      thunk_FUN_1400c7da0(local_5d0);
      if (cVar3 != '\0') {
        local_5a8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_520);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_5a8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_5a0,false);
        *(undefined ***)((longlong)&local_5a8 + (longlong)*(int *)(local_5a8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_5b0 + (longlong)*(int *)(local_5a8 + 4) + 4) =
             *(int *)(local_5a8 + 4) + -0x88;
        thunk_FUN_14002d040(&local_5a0,2);
        thunk_FUN_14002cab0(&local_5a8,"incompatible device");
        uVar6 = thunk_FUN_140030920(&local_5a8,local_4a8);
        uVar8 = thunk_FUN_1400c9380(local_5d0);
        thunk_FUN_1400c8570(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_5d0);
        thunk_FUN_14002d390(local_4a8);
        goto LAB_1400532da;
      }
    }
  }
LAB_1400532ed:
  SelectObject(hdc,local_5c0);
LAB_140053eb1:
  DeleteObject(hbm);
  DeleteDC(hdc);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140054340
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140054340(undefined8 param_1,HWND param_2)

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
  local_20 = DAT_14036a720 ^ (ulonglong)auStack_198;
  local_168 = 0;
  BVar2 = GetWindowRect(param_2,&local_50);
  if (BVar2 == 0) {
    uVar4 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar4,30000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
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
               thunk_FUN_14002cab0(&local_148,"GetWindowRect: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar3);
      uVar4 = thunk_FUN_140030920(&local_148,local_40);
      uVar6 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x1b0;
      thunk_FUN_1400c8570(uVar6,30000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
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
        FUN_140002d06(uVar8);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_140);
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
      uVar4 = thunk_FUN_1400c9380(local_160);
      cVar1 = thunk_FUN_1400c8f40(uVar4,30000);
      thunk_FUN_1400c7da0(local_160);
      if (cVar1 != '\0') {
        local_148 = &DAT_1402738f0;
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
        thunk_FUN_14002d040(&local_140,2);
        DVar3 = GetLastError();
        pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_148,"GetWindowDC: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,DVar3);
        uVar4 = thunk_FUN_140030920(&local_148,local_40);
        uVar6 = thunk_FUN_1400c9380(local_160);
        local_178 = 0x1bc;
        thunk_FUN_1400c8570(uVar6,30000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\screenShooterImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_160);
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
          FUN_140002d06(uVar8);
        }
        thunk_FUN_14002d2f0(local_c0);
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
      thunk_FUN_140052da0(param_1,hDC,pHVar9,&local_50);
      ReleaseDC(param_2,hDC);
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140056dc0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140056dc0(longlong param_1,undefined ***param_2)

{
  undefined **ppuVar1;
  int *piVar2;
  QMapDataBase *this;
  char cVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  undefined8 uVar7;
  basic_ostream<char,std::char_traits<char>_> *pbVar8;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar9;
  undefined8 uVar10;
  void *pvVar11;
  longlong *plVar12;
  longlong *plVar13;
  QString *pQVar14;
  QMapDataBase *this_00;
  ulonglong *puVar15;
  ulonglong uVar16;
  longlong lVar17;
  int iVar18;
  uint uVar19;
  uint uVar20;
  longlong lVar21;
  longlong lVar22;
  undefined1 auStack_678 [32];
  undefined ***local_658;
  int local_650;
  undefined4 local_648;
  longlong **local_640;
  undefined1 *local_638;
  longlong **local_630;
  uint local_628;
  longlong *local_620;
  longlong *local_618;
  longlong *local_610;
  longlong *local_608;
  uint local_600;
  longlong *local_5f8;
  longlong *local_5f0;
  longlong **local_5e8;
  undefined **local_5e0;
  longlong *local_5d8;
  longlong *local_5d0;
  longlong *local_5c8;
  longlong local_5c0;
  undefined ***local_5b8;
  undefined ***local_5b0;
  undefined ***local_5a8;
  undefined1 local_5a0 [8];
  undefined8 local_598;
  undefined ***local_590;
  void *local_588;
  undefined8 local_580;
  longlong **local_578;
  undefined1 local_570 [16];
  undefined1 local_560 [16];
  undefined1 local_550 [16];
  undefined1 local_540 [16];
  undefined1 local_530 [16];
  undefined1 local_520 [16];
  undefined1 local_510 [16];
  undefined1 local_500 [20];
  int iStack_4ec;
  undefined *local_4e8;
  undefined **local_4e0;
  basic_ostream<char,std::char_traits<char>_> local_4d8 [96];
  undefined8 local_478;
  undefined4 local_470;
  basic_ios<char,std::char_traits<char>_> local_460 [100];
  int iStack_3fc;
  undefined *local_3f8;
  undefined **local_3f0;
  basic_ostream<char,std::char_traits<char>_> local_3e8 [96];
  undefined8 local_388;
  undefined4 local_380;
  basic_ios<char,std::char_traits<char>_> local_370 [100];
  int iStack_30c;
  undefined *local_308;
  undefined **local_300;
  basic_ostream<char,std::char_traits<char>_> local_2f8 [96];
  undefined8 local_298;
  undefined4 local_290;
  basic_ios<char,std::char_traits<char>_> local_280 [100];
  int iStack_21c;
  undefined *local_218;
  undefined **local_210;
  basic_ostream<char,std::char_traits<char>_> local_208 [96];
  undefined8 local_1a8;
  undefined4 local_1a0;
  basic_ios<char,std::char_traits<char>_> local_190 [104];
  ulonglong local_128 [3];
  ulonglong local_110;
  ulonglong local_108 [3];
  ulonglong local_f0;
  ulonglong local_e8 [3];
  ulonglong local_d0;
  ulonglong local_c8 [3];
  ulonglong local_b0;
  undefined1 local_a8 [96];
  ulonglong local_48;
  
  local_598 = 0xfffffffffffffffe;
  local_48 = DAT_14036a720 ^ (ulonglong)auStack_678;
  uVar19 = 0;
  local_628 = 0;
  local_608 = (longlong *)0x0;
  local_610 = (longlong *)0x0;
  iVar18 = 7;
  local_5c0 = param_1;
  local_5b0 = param_2;
  local_590 = param_2;
  uVar20 = uVar19;
  do {
    if (local_610 != (longlong *)0x0) {
      qt_assert("!_raw","R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-utils/win/comPtr.hpp",
                0x94);
    }
    if (local_608 != (longlong *)0x0) {
      qt_assert("!_raw","R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-utils/win/comPtr.hpp",
                0x94);
    }
    local_658 = (undefined ***)(&DAT_1402a010c + (longlong)iVar18 * -4);
    local_630 = &local_610;
    local_638 = local_5a0;
    local_640 = &local_608;
    local_648 = 7;
    local_650 = iVar18;
    iVar6 = (**(code **)(param_1 + 0x18))(*param_2,0,0,0x21);
    if (-1 < iVar6) break;
    if (iVar18 < 2) {
      uVar7 = thunk_FUN_1400c9380(local_550);
      cVar3 = thunk_FUN_1400c8f40(uVar7,30000);
      thunk_FUN_1400c7da0(local_550);
      if (cVar3 != '\0') {
        local_3f8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_370);
        local_628 = uVar20 | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_3f8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_3f0,false);
        *(undefined ***)((longlong)&local_3f8 + (longlong)*(int *)(local_3f8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_3fc + (longlong)*(int *)(local_3f8 + 4)) =
             *(int *)(local_3f8 + 4) + -0x88;
        local_5b0 = &local_3f0;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_3f0);
        local_3f0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_388 = 0;
        local_380 = 4;
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_3f8,"D3D11CreateDevice failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar6);
        uVar7 = thunk_FUN_140030920(&local_3f8,local_108);
        uVar10 = thunk_FUN_1400c9380(local_540);
        local_658 = (undefined ***)CONCAT44(local_658._4_4_,0x14e);
        thunk_FUN_1400c8570(uVar10,30000,uVar7,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\dx112.cpp"
                           );
        thunk_FUN_1400c7da0(local_540);
        if (0xf < local_f0) {
          uVar16 = local_108[0];
          if (0xfff < local_f0 + 1) {
            if ((local_108[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar16 = *(ulonglong *)(local_108[0] - 8);
            if (local_108[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_108[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_108[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar16);
        }
        *(undefined ***)((longlong)&local_3f8 + (longlong)*(int *)(local_3f8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_3fc + (longlong)*(int *)(local_3f8 + 4)) =
             *(int *)(local_3f8 + 4) + -0x88;
        local_3f0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_3f0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_3f0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_3e8);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_370);
      }
      if (local_610 != (longlong *)0x0) {
        (**(code **)(*local_610 + 0x10))();
        local_610 = (longlong *)0x0;
      }
      if (local_608 != (longlong *)0x0) {
        (**(code **)(*local_608 + 0x10))();
        local_608 = (longlong *)0x0;
      }
      if (*param_2 != (undefined **)0x0) {
        (**(code **)(**param_2 + 0x10))();
        *param_2 = (undefined **)0x0;
      }
      return 0;
    }
    uVar7 = thunk_FUN_1400c9380(local_570);
    cVar3 = thunk_FUN_1400c8f40(uVar7,30000);
    thunk_FUN_1400c7da0(local_570);
    if (cVar3 != '\0') {
      local_4e8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_460);
      uVar20 = 1;
      local_628 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_4e8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_4e0,false);
      *(undefined ***)((longlong)&local_4e8 + (longlong)*(int *)(local_4e8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_4ec + (longlong)*(int *)(local_4e8 + 4)) =
           *(int *)(local_4e8 + 4) + -0x88;
      local_5b8 = &local_4e0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_4e0);
      local_4e0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_478 = 0;
      local_470 = 4;
      pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_4e8,"D3D11CreateDevice failed: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar6);
      pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar9," for flevels: ");
      pbVar9 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar18);
      thunk_FUN_14002cab0(pbVar9,", try smaller flevels");
      uVar7 = thunk_FUN_140030920(&local_4e8,local_128);
      uVar10 = thunk_FUN_1400c9380(local_560);
      local_658 = (undefined ***)CONCAT44(local_658._4_4_,0x14a);
      thunk_FUN_1400c8570(uVar10,30000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\dx112.cpp"
                         );
      thunk_FUN_1400c7da0(local_560);
      if (0xf < local_110) {
        uVar16 = local_128[0];
        if (0xfff < local_110 + 1) {
          if ((local_128[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar16 = *(ulonglong *)(local_128[0] - 8);
          if (local_128[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_128[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_128[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar16);
      }
      *(undefined ***)((longlong)&local_4e8 + (longlong)*(int *)(local_4e8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_4ec + (longlong)*(int *)(local_4e8 + 4)) =
           *(int *)(local_4e8 + 4) + -0x88;
      local_4e0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_4e0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_4e0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_4d8);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_460);
    }
    iVar18 = iVar18 + -1;
  } while (iVar18 != 0);
  do {
    plVar13 = (longlong *)0x0;
    local_618 = (longlong *)0x0;
    local_600 = uVar19;
    if (*param_2 == (undefined **)0x0) {
      qt_assert("_raw","R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-utils/win/comPtr.hpp",0x89
               );
    }
    ppuVar1 = *param_2;
    if (local_618 != (longlong *)0x0) {
      qt_assert("!_raw","R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-utils/win/comPtr.hpp",
                0x94);
    }
    iVar18 = (**(code **)(*ppuVar1 + 0x38))(ppuVar1,uVar19);
    if (iVar18 < 0) {
      if (local_618 != (longlong *)0x0) {
        (**(code **)(*local_618 + 0x10))();
      }
      if (local_610 != (longlong *)0x0) {
        (**(code **)(*local_610 + 0x10))();
        local_610 = (longlong *)0x0;
      }
      if (local_608 != (longlong *)0x0) {
        (**(code **)(*local_608 + 0x10))();
        local_608 = (longlong *)0x0;
      }
      if (*param_2 != (undefined **)0x0) {
        (**(code **)(**param_2 + 0x10))();
        *param_2 = (undefined **)0x0;
      }
      return 1;
    }
    local_620 = (longlong *)0x0;
    if (local_618 == (longlong *)0x0) {
      iVar18 = -0x7fffbffb;
LAB_140057423:
      uVar7 = thunk_FUN_1400c9380(local_530);
      cVar3 = thunk_FUN_1400c8f40(uVar7,30000);
      thunk_FUN_1400c7da0(local_530);
      if (cVar3 != '\0') {
        local_308 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_280);
        uVar20 = uVar20 | 4;
        local_628 = uVar20;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_308,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_300,false);
        *(undefined ***)((longlong)&local_308 + (longlong)*(int *)(local_308 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_30c + (longlong)*(int *)(local_308 + 4)) =
             *(int *)(local_308 + 4) + -0x88;
        local_5a8 = &local_300;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_300);
        local_300 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_298 = 0;
        local_290 = 4;
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_308,"IDXGIOutput.QueryInterface(IDXGIOutput1) failed: ")
        ;
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar18);
        uVar7 = thunk_FUN_140030920(&local_308,local_e8);
        uVar10 = thunk_FUN_1400c9380(local_520);
        local_658 = (undefined ***)CONCAT44(local_658._4_4_,0x163);
        thunk_FUN_1400c8570(uVar10,30000,uVar7,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\dx112.cpp"
                           );
        thunk_FUN_1400c7da0(local_520);
        if (0xf < local_d0) {
          uVar16 = local_e8[0];
          if (0xfff < local_d0 + 1) {
            if ((local_e8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar16 = *(ulonglong *)(local_e8[0] - 8);
            if (local_e8[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_e8[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_e8[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar16);
        }
        *(undefined ***)((longlong)&local_308 + (longlong)*(int *)(local_308 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_30c + (longlong)*(int *)(local_308 + 4)) =
             *(int *)(local_308 + 4) + -0x88;
        local_300 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_300);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_300);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_2f8);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_280);
      }
      if (local_620 != (longlong *)0x0) {
        (**(code **)(*local_620 + 0x10))();
        local_620 = (longlong *)0x0;
      }
      if (local_618 != (longlong *)0x0) {
        (**(code **)(*local_618 + 0x10))();
      }
    }
    else {
      iVar18 = (**(code **)*local_618)(local_618,&DAT_1402a0280);
      if (iVar18 < 0) goto LAB_140057423;
      if (local_620 == (longlong *)0x0) {
        qt_assert("_raw","R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-utils/win/comPtr.hpp",
                  0x89);
      }
      iVar18 = (**(code **)(*local_620 + 0x38))(local_620,local_a8);
      if (iVar18 < 0) {
        uVar7 = thunk_FUN_1400c9380(local_510);
        cVar3 = thunk_FUN_1400c8f40(uVar7,30000);
        thunk_FUN_1400c7da0(local_510);
        if (cVar3 != '\0') {
          local_218 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_190);
          uVar20 = uVar20 | 8;
          local_628 = uVar20;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_218,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_210,false);
          *(undefined ***)((longlong)&local_218 + (longlong)*(int *)(local_218 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_21c + (longlong)*(int *)(local_218 + 4)) =
               *(int *)(local_218 + 4) + -0x88;
          local_5a8 = &local_210;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_210);
          local_210 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_1a8 = 0;
          local_1a0 = 4;
          pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(&local_218,"IDXGIOutput1.GetDesc failed: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar18);
          uVar7 = thunk_FUN_140030920(&local_218,local_c8);
          uVar10 = thunk_FUN_1400c9380(local_500);
          local_658 = (undefined ***)CONCAT44(local_658._4_4_,0x16b);
          thunk_FUN_1400c8570(uVar10,30000,uVar7,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-screenShooter\\dx112.cpp"
                             );
          thunk_FUN_1400c7da0(local_500);
          if (0xf < local_b0) {
            uVar16 = local_c8[0];
            if (0xfff < local_b0 + 1) {
              if ((local_c8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar16 = *(ulonglong *)(local_c8[0] - 8);
              if (local_c8[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_c8[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_c8[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d06(uVar16);
          }
          *(undefined ***)((longlong)&local_218 + (longlong)*(int *)(local_218 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_21c + (longlong)*(int *)(local_218 + 4)) =
               *(int *)(local_218 + 4) + -0x88;
          local_210 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14002e520(&local_210);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_210);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_208);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_190);
        }
        if (local_620 != (longlong *)0x0) {
          (**(code **)(*local_620 + 0x10))();
          local_620 = (longlong *)0x0;
        }
        if (local_618 != (longlong *)0x0) {
          (**(code **)(*local_618 + 0x10))();
        }
      }
      else {
        pvVar11 = operator_new(0x78);
        plVar12 = plVar13;
        local_588 = pvVar11;
        if (pvVar11 != (void *)0x0) {
          local_5a8 = &local_5e0;
          local_578 = &local_5d8;
          local_5e8 = &local_5d0;
          local_5e0 = *param_2;
          if (local_5e0 != (undefined **)0x0) {
            (**(code **)(*local_5e0 + 8))();
          }
          local_5d8 = local_610;
          if (local_610 != (longlong *)0x0) {
            (**(code **)(*local_610 + 8))();
          }
          local_5d0 = local_608;
          if (local_608 != (longlong *)0x0) {
            (**(code **)(*local_608 + 8))();
          }
          local_5c8 = local_620;
          if (local_620 != (longlong *)0x0) {
            (**(code **)(*local_620 + 8))();
          }
          local_658 = &local_5e0;
          plVar12 = (longlong *)thunk_FUN_14005a4c0(pvVar11,&local_5c8,&local_5d0,&local_5d8);
        }
        local_5f8 = plVar12;
        if (plVar12 == (longlong *)0x0) {
          local_5f0 = (longlong *)0x0;
        }
        else {
          plVar13 = operator_new(0x18);
          local_5e8 = (longlong **)(plVar13 + 2);
          if (local_5e8 != (longlong **)0x0) {
            *local_5e8 = plVar12;
          }
          if (plVar13 != (longlong *)0x0) {
            plVar13[1] = (longlong)&LAB_140004ddb;
            *(int *)((longlong)plVar13 + 4) = 1;
            *(int *)plVar13 = 1;
          }
          local_5f0 = plVar13;
          thunk_FUN_140036f80(plVar13,plVar12,1);
          FUN_14000130c(&local_5f8,plVar12);
        }
        pQVar14 = (QString *)QString::fromWCharArray((wchar_t *)&local_5b8,(int)local_a8);
        if (1 < **(uint **)(param_1 + 0x28)) {
          this_00 = QMapDataBase::createData();
          lVar22 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x10);
          if (lVar22 != 0) {
            puVar15 = (ulonglong *)thunk_FUN_1400558f0(lVar22,this_00);
            *(ulonglong **)(this_00 + 0x10) = puVar15;
            *puVar15 = *puVar15 & 3;
            *puVar15 = *puVar15 | (ulonglong)(this_00 + 8);
          }
          piVar2 = *(int **)(param_1 + 0x28);
          if (*piVar2 == 0) {
LAB_140057a38:
            this = *(QMapDataBase **)(param_1 + 0x28);
            lVar22 = *(longlong *)(this + 0x10);
            if (lVar22 != 0) {
              QString::~QString((QString *)(lVar22 + 0x18));
              thunk_FUN_140055d50(*(undefined8 *)(lVar22 + 0x28));
              if (*(longlong *)(lVar22 + 8) != 0) {
                thunk_FUN_140055f30();
              }
              if (*(longlong *)(lVar22 + 0x10) != 0) {
                thunk_FUN_140055f30();
              }
              QMapDataBase::freeTree(this,*(QMapNodeBase **)(this + 0x10),8);
            }
            QMapDataBase::freeData(this);
          }
          else if (*piVar2 != -1) {
            LOCK();
            *piVar2 = *piVar2 + -1;
            UNLOCK();
            plVar13 = local_5f0;
            if (*piVar2 == 0) goto LAB_140057a38;
          }
          *(QMapDataBase **)(param_1 + 0x28) = this_00;
          QMapDataBase::recalcMostLeftNode(this_00);
        }
        lVar17 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x10);
        lVar22 = *(longlong *)(param_1 + 0x28) + 8;
        lVar21 = 0;
        bVar4 = true;
        if (lVar17 == 0) {
LAB_140057b31:
          param_1 = local_5c0;
          local_658 = (undefined ***)CONCAT71(local_658._1_7_,bVar4);
          thunk_FUN_140055b30(*(undefined8 *)(local_5c0 + 0x28),pQVar14,&local_5f8,lVar22);
        }
        else {
          do {
            lVar22 = lVar17;
            bVar4 = operator<((QString *)(lVar22 + 0x18),pQVar14);
            if (bVar4) {
              lVar17 = *(longlong *)(lVar22 + 0x10);
            }
            else {
              lVar17 = *(longlong *)(lVar22 + 8);
              lVar21 = lVar22;
            }
            bVar4 = !bVar4;
          } while (lVar17 != 0);
          if ((lVar21 == 0) || (bVar5 = operator<(pQVar14,(QString *)(lVar21 + 0x18)), bVar5))
          goto LAB_140057b31;
          if (plVar13 != (longlong *)0x0) {
            LOCK();
            *(int *)plVar13 = (int)*plVar13 + 1;
            UNLOCK();
            LOCK();
            *(int *)((longlong)local_5f0 + 4) = *(int *)((longlong)local_5f0 + 4) + 1;
            UNLOCK();
            plVar13 = local_5f0;
          }
          local_580 = *(undefined8 *)(lVar21 + 0x28);
          *(longlong **)(lVar21 + 0x28) = plVar13;
          *(longlong **)(lVar21 + 0x20) = plVar12;
          thunk_FUN_140055d50();
          param_1 = local_5c0;
        }
        QString::~QString((QString *)&local_5b8);
        thunk_FUN_140055d50(plVar13);
        if (local_620 != (longlong *)0x0) {
          (**(code **)(*local_620 + 0x10))();
          local_620 = (longlong *)0x0;
        }
        param_2 = local_5b0;
        uVar20 = local_628;
        if (local_618 != (longlong *)0x0) {
          (**(code **)(*local_618 + 0x10))();
          param_2 = local_5b0;
          uVar20 = local_628;
        }
      }
    }
    uVar19 = local_600 + 1;
  } while( true );
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14005cbb0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14005cbb0(longlong param_1)

{
  char cVar1;
  int iVar2;
  DWORD DVar3;
  HWND pHVar4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar6;
  ulonglong uVar7;
  QAbstractNativeEventFilter *pQVar8;
  undefined1 auStackY_1b8 [32];
  undefined ***local_150 [2];
  undefined8 local_140;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_140 = 0xfffffffffffffffe;
  local_28 = DAT_14036a720 ^ (ulonglong)auStackY_1b8;
  pHVar4 = CreateWindowExW(0,L"STATIC",(LPCWSTR)((longlong)&PTR_1402a0c28 + 4),0,0,0,0,0,(HWND)0x0,
                           (HMENU)0x0,(HINSTANCE)0x0,(LPVOID)0x0);
  *(HWND *)(param_1 + 0x58) = pHVar4;
  iVar2 = WTSRegisterSessionNotification(pHVar4,0);
  if (iVar2 == 0) {
    uVar5 = thunk_FUN_1400c9380(local_150);
    cVar1 = thunk_FUN_1400c8f40(uVar5,40000);
    thunk_FUN_1400c7da0(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_150[0] = &local_130;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_c8 = 0;
      local_c0 = 4;
      DVar3 = GetLastError();
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(&local_138,"WTSRegisterSessionNotification: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,DVar3);
      uVar5 = thunk_FUN_140030920(&local_138,local_48);
      uVar6 = thunk_FUN_1400c9380(local_150);
      thunk_FUN_1400c8570(uVar6,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_150);
      if (0xf < local_30) {
        uVar7 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar7);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  pQVar8 = (QAbstractNativeEventFilter *)(param_1 + 0x48);
  if (param_1 == 0) {
    pQVar8 = (QAbstractNativeEventFilter *)0x0;
  }
  QCoreApplication::installNativeEventFilter(*(QCoreApplication **)self_exref,pQVar8);
  *(undefined2 *)(param_1 + 0x60) = 1;
  thunk_FUN_14005c6d0(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14005cf10
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_14005cf10(longlong param_1,undefined8 param_2,ulonglong *param_3)

{
  char cVar1;
  undefined8 uVar2;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar3;
  ulonglong extraout_RAX;
  ulonglong uVar4;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined4 local_168;
  undefined1 local_160 [20];
  int iStack_14c;
  undefined *local_148;
  basic_streambuf<char,std::char_traits<char>_> local_140 [128];
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  QMutexLocker local_58 [8];
  undefined8 local_50;
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_50 = 0xfffffffffffffffe;
  local_28 = DAT_14036a720 ^ (ulonglong)auStack_198;
  local_168 = 0;
  uVar4 = *(ulonglong *)(param_1 + 0x10);
  if (*param_3 != uVar4) goto LAB_14005d993;
  QMutexLocker::QMutexLocker(local_58,(QBasicMutex *)(param_1 + -0x38));
  if ((int)param_3[1] != 0x2b1) goto LAB_14005d989;
  switch(param_3[2]) {
  case 1:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"console connect");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x80;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
LAB_14005d09a:
      thunk_FUN_14002d2f0(local_c0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    goto LAB_14005d0ad;
  case 2:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"console disconnect");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x88;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
LAB_14005d2b1:
      thunk_FUN_14002d2f0(local_c0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    goto LAB_14005d2c4;
  case 3:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"remote connect");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x7c;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
      goto LAB_14005d09a;
    }
LAB_14005d0ad:
    *(undefined1 *)(param_1 + 0x18) = 1;
    break;
  case 4:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"remote disconnect");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x84;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
      goto LAB_14005d2b1;
    }
LAB_14005d2c4:
    *(undefined1 *)(param_1 + 0x18) = 0;
    break;
  case 5:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 0x40;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"session logon");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x95;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
LAB_14005d95c:
      thunk_FUN_14002d2f0(local_c0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    break;
  case 6:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 0x80;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"session logoff");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x98;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
      goto LAB_14005d95c;
    }
    break;
  case 7:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 0x10;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"session lock");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x8d;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
      thunk_FUN_14002d2f0(local_c0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    *(undefined1 *)(param_1 + 0x19) = 1;
    break;
  case 8:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,20000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 0x20;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      thunk_FUN_14002cab0(&local_148,"session unlock");
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x91;
      thunk_FUN_1400c8570(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      thunk_FUN_14002d390(local_48);
      thunk_FUN_14002d2f0(local_c0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    *(undefined1 *)(param_1 + 0x19) = 0;
    break;
  default:
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,30000);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 0x100;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
      *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_14002d040(local_140,2);
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(&local_148,"WTS_SESSION_???: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_3[2]);
      uVar2 = thunk_FUN_140030920(&local_148,local_48);
      uVar3 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x9b;
      thunk_FUN_1400c8570(uVar3,30000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-sessionStateShooter\\sessionStateShooterImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      if (0xf < local_30) {
        uVar4 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar4 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar4);
      }
      goto LAB_14005d95c;
    }
  }
  if (*(char *)(param_1 + 0x18) == '\0') {
    cVar1 = '\x01';
  }
  else {
    cVar1 = *(char *)(param_1 + 0x19);
  }
  if (cVar1 != *(char *)(param_1 + -0x2e)) {
    *(char *)(param_1 + -0x2e) = cVar1;
    *(undefined1 *)(param_1 + -0x30) = 1;
  }
LAB_14005d989:
  QMutexLocker::~QMutexLocker(local_58);
  uVar4 = extraout_RAX;
LAB_14005d993:
  return uVar4 & 0xffffffffffffff00;
}

// =============================================================================
// CONTROL SI SUBAGENT
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14006c500
// -----------------------------------------------------------------------------
void FUN_14006c500(longlong param_1)

{
  if (*(char *)(param_1 + 0x10) == '\0') {
    qt_assert("_initialized",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp",0x43
             );
  }
  *(undefined1 *)(param_1 + 0x10) = 0;
  thunk_FUN_140073070(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140071ee0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140071ee0(longlong param_1,bool param_2)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar4;
  ulonglong uVar5;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined4 local_168;
  undefined ***local_160 [2];
  undefined8 local_150;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_150 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_198;
  local_168 = 0;
  uVar3 = thunk_FUN_1400c9380(local_160);
  cVar1 = thunk_FUN_1400c8f40(uVar3,20000);
  thunk_FUN_1400c7da0(local_160);
  if (cVar1 != '\0') {
    local_148 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    local_168 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
         *(int *)(local_148 + 4) + -0x88;
    local_160[0] = &local_140;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_d8 = 0;
    local_d0 = 4;
    this = (basic_ostream<char,std::char_traits<char>_> *)
           thunk_FUN_14002cab0(&local_148,"slave connected as client: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
    uVar3 = thunk_FUN_140030920(&local_148,local_58);
    uVar4 = thunk_FUN_1400c9380(local_160);
    local_178 = 0x142;
    thunk_FUN_1400c8570(uVar4,20000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp"
                       );
    thunk_FUN_1400c7da0(local_160);
    if (0xf < local_40) {
      uVar5 = local_58[0];
      if (0xfff < local_40 + 1) {
        if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar5 = *(ulonglong *)(local_58[0] - 8);
        if (local_58[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_58[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_58[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar5);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14002e520(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  iVar2 = 2 - (uint)(param_2 != false);
  if (*(int *)(param_1 + 0x39474) != iVar2) {
    *(int *)(param_1 + 0x39474) = iVar2;
    *(undefined1 *)(param_1 + 0x12) = 1;
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400724a0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400724a0(longlong param_1,undefined8 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char cVar5;
  undefined8 uVar6;
  QString *pQVar7;
  QByteArray *pQVar8;
  char *pcVar9;
  undefined8 uVar10;
  longlong lVar11;
  undefined ***pppuVar12;
  undefined ***pppuVar13;
  char *pcVar14;
  char *pcVar15;
  ulonglong uVar16;
  undefined8 *puVar17;
  uint uVar18;
  uint uVar19;
  undefined1 auStack_338 [32];
  undefined ****local_318;
  undefined ***local_308;
  uint local_300;
  undefined8 *local_2f8;
  undefined1 local_2f0 [8];
  Data *local_2e8;
  QTypedArrayData<unsigned_short> *local_2e0;
  undefined ***local_2d8;
  undefined ***local_2d0;
  QByteArray local_2c8 [8];
  QString local_2c0 [8];
  QString local_2b8 [8];
  undefined8 local_2b0;
  undefined1 local_2a8 [16];
  undefined1 local_298 [16];
  undefined1 local_288 [16];
  undefined1 local_278 [12];
  int iStack_26c;
  undefined *local_268;
  undefined **local_260;
  basic_ostream<char,std::char_traits<char>_> local_258 [96];
  undefined8 local_1f8;
  undefined4 local_1f0;
  basic_ios<char,std::char_traits<char>_> local_1e0 [100];
  int iStack_17c;
  undefined *local_178;
  basic_streambuf<char,std::char_traits<char>_> local_170 [128];
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  ulonglong local_88 [3];
  ulonglong local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_2b0 = 0xfffffffffffffffe;
  local_48 = DAT_14036a720 ^ (ulonglong)auStack_338;
  pppuVar13 = (undefined ***)0x0;
  uVar19 = 0;
  local_300 = 0;
  if (*(char *)(param_1 + 0x10) == '\0') {
    return;
  }
  if (*(char *)(param_1 + 0x394d0) == '\0') {
    return;
  }
  local_2f8 = param_2;
  uVar6 = thunk_FUN_1400c9380(local_2a8);
  cVar5 = thunk_FUN_1400c8f40(uVar6,20000);
  thunk_FUN_1400c7da0(local_2a8);
  if (cVar5 != '\0') {
    local_178 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_f0);
    uVar19 = 1;
    local_300 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_178,local_170,false);
    *(undefined ***)(local_170 + (longlong)*(int *)(local_178 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    thunk_FUN_14002d040(local_170,2);
    pQVar7 = (QString *)FUN_14006e1a0(local_2c0,param_2);
    uVar6 = thunk_FUN_14002cab0(&local_178,"slave notified about master point(s): ");
    pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar7);
    pcVar9 = QByteArray::data(pQVar8);
    thunk_FUN_14002cab0(uVar6,pcVar9);
    QByteArray::~QByteArray(local_2c8);
    QString::~QString(local_2c0);
    uVar6 = thunk_FUN_140030920(&local_178,local_88);
    uVar10 = thunk_FUN_1400c9380(local_298);
    local_318 = (undefined ****)CONCAT44(local_318._4_4_,0xb0);
    thunk_FUN_1400c8570(uVar10,20000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp"
                       );
    thunk_FUN_1400c7da0(local_298);
    if (0xf < local_70) {
      uVar16 = local_88[0];
      if (0xfff < local_70 + 1) {
        if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar16 = *(ulonglong *)(local_88[0] - 8);
        if (local_88[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_88[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_88[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar16);
    }
    thunk_FUN_14002d2f0(local_f0);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_f0)
    ;
  }
  pcVar9 = (char *)(param_1 + 0x28040);
  lVar11 = 0x114;
  pcVar15 = pcVar9;
  do {
    puVar17 = param_2;
    pcVar14 = pcVar15;
    uVar6 = puVar17[1];
    *(undefined8 *)pcVar14 = *puVar17;
    *(undefined8 *)(pcVar14 + 8) = uVar6;
    uVar6 = puVar17[3];
    *(undefined8 *)(pcVar14 + 0x10) = puVar17[2];
    *(undefined8 *)(pcVar14 + 0x18) = uVar6;
    uVar6 = puVar17[5];
    *(undefined8 *)(pcVar14 + 0x20) = puVar17[4];
    *(undefined8 *)(pcVar14 + 0x28) = uVar6;
    uVar6 = puVar17[7];
    *(undefined8 *)(pcVar14 + 0x30) = puVar17[6];
    *(undefined8 *)(pcVar14 + 0x38) = uVar6;
    uVar6 = puVar17[9];
    *(undefined8 *)(pcVar14 + 0x40) = puVar17[8];
    *(undefined8 *)(pcVar14 + 0x48) = uVar6;
    uVar6 = puVar17[0xb];
    *(undefined8 *)(pcVar14 + 0x50) = puVar17[10];
    *(undefined8 *)(pcVar14 + 0x58) = uVar6;
    uVar6 = puVar17[0xd];
    *(undefined8 *)(pcVar14 + 0x60) = puVar17[0xc];
    *(undefined8 *)(pcVar14 + 0x68) = uVar6;
    uVar2 = *(undefined4 *)((longlong)puVar17 + 0x74);
    uVar3 = *(undefined4 *)(puVar17 + 0xf);
    uVar4 = *(undefined4 *)((longlong)puVar17 + 0x7c);
    *(undefined4 *)(pcVar14 + 0x70) = *(undefined4 *)(puVar17 + 0xe);
    *(undefined4 *)(pcVar14 + 0x74) = uVar2;
    *(undefined4 *)(pcVar14 + 0x78) = uVar3;
    *(undefined4 *)(pcVar14 + 0x7c) = uVar4;
    lVar11 = lVar11 + -1;
    pcVar15 = pcVar14 + 0x80;
    param_2 = puVar17 + 0x10;
  } while (lVar11 != 0);
  uVar2 = *(undefined4 *)((longlong)puVar17 + 0x84);
  uVar3 = *(undefined4 *)(puVar17 + 0x11);
  uVar4 = *(undefined4 *)((longlong)puVar17 + 0x8c);
  *(undefined4 *)(pcVar14 + 0x80) = *(undefined4 *)(puVar17 + 0x10);
  *(undefined4 *)(pcVar14 + 0x84) = uVar2;
  *(undefined4 *)(pcVar14 + 0x88) = uVar3;
  *(undefined4 *)(pcVar14 + 0x8c) = uVar4;
  *(undefined4 *)(pcVar14 + 0x90) = *(undefined4 *)(puVar17 + 0x12);
  *(undefined2 *)(pcVar14 + 0x94) = *(undefined2 *)((longlong)puVar17 + 0x94);
  thunk_FUN_140148f90(local_2f0);
  pppuVar12 = pppuVar13;
  do {
    if (*pcVar9 == '\0') break;
    pcVar15 = (char *)(param_1 + 0x28040 + (longlong)(int)pppuVar12 * 0x100);
    if (pcVar15 == (char *)0x0) {
      lVar11 = 0xffffffff;
    }
    else {
      lVar11 = -1;
      do {
        lVar11 = lVar11 + 1;
      } while (pcVar15[lVar11] != '\0');
    }
    local_2e0 = QString::fromAscii_helper(pcVar15,(int)lVar11);
    thunk_FUN_140149800(local_2f0,&local_2e0);
    QString::~QString((QString *)&local_2e0);
    uVar18 = (int)pppuVar12 + 1;
    pppuVar12 = (undefined ***)(ulonglong)uVar18;
    pcVar9 = pcVar9 + 0x100;
  } while ((int)uVar18 < 10);
  cVar5 = thunk_FUN_140154b00(*(undefined8 *)(param_1 + 0x394b8));
  if (cVar5 == '\0') {
    cVar5 = thunk_FUN_14014aa30(local_2f0);
    if (cVar5 == '\0') {
      thunk_FUN_140071ee0(param_1,0);
    }
    else {
      uVar6 = thunk_FUN_1400c9380(local_288);
      cVar5 = thunk_FUN_1400c8f40(uVar6,20000);
      thunk_FUN_1400c7da0(local_288);
      if (cVar5 != '\0') {
        local_268 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_1e0);
        local_300 = uVar19 | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
        *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
             *(int *)(local_268 + 4) + -0x88;
        local_308 = &local_260;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
        local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_1f8 = 0;
        local_1f0 = 4;
        pQVar7 = (QString *)FUN_14006e1a0(local_2b8,local_2f8);
        uVar6 = thunk_FUN_14002cab0(&local_268,"slave start client: ");
        pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar7);
        pcVar9 = QByteArray::data(pQVar8);
        thunk_FUN_14002cab0(uVar6,pcVar9);
        QByteArray::~QByteArray((QByteArray *)&local_2f8);
        QString::~QString(local_2b8);
        uVar6 = thunk_FUN_140030920(&local_268,local_68);
        uVar10 = thunk_FUN_1400c9380(local_278);
        local_318 = (undefined ****)CONCAT44(local_318._4_4_,199);
        thunk_FUN_1400c8570(uVar10,20000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp"
                           );
        thunk_FUN_1400c7da0(local_278);
        if (0xf < local_50) {
          uVar16 = local_68[0];
          if (0xfff < local_50 + 1) {
            if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar16 = *(ulonglong *)(local_68[0] - 8);
            if (local_68[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_68[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_68[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar16);
        }
        *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
             *(int *)(local_268 + 4) + -0x88;
        local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_260);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_258);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_1e0);
      }
      local_308 = operator_new(0x18);
      pppuVar12 = pppuVar13;
      if (local_308 != (undefined ***)0x0) {
        pppuVar12 = (undefined ***)
                    thunk_FUN_140195880(local_308,*(undefined8 *)(param_1 + 0x394a8),
                                        *(undefined8 *)(param_1 + 0x394b0));
      }
      local_2d8 = pppuVar12;
      if (pppuVar12 == (undefined ***)0x0) {
        local_2d0 = (undefined ***)0x0;
      }
      else {
        pppuVar13 = operator_new(0x18);
        if (pppuVar13 + 2 != (undefined ***)0x0) {
          pppuVar13[2] = (undefined **)pppuVar12;
        }
        if (pppuVar13 != (undefined ***)0x0) {
          pppuVar13[1] = (undefined **)&LAB_14000702c;
          *(undefined4 *)((longlong)pppuVar13 + 4) = 1;
          *(undefined4 *)pppuVar13 = 1;
        }
        local_308 = pppuVar13;
        local_2d0 = pppuVar13;
        thunk_FUN_140036f80(pppuVar13,pppuVar12,1);
        FUN_140012daf(&local_2d8,pppuVar12);
      }
      local_318 = &local_2d8;
      thunk_FUN_140072d60(*(undefined8 *)(param_1 + 0x394b8),param_1 + 0x39478,param_1 + 0x39480,
                          param_1 + 0x39488);
      thunk_FUN_14006c5c0(pppuVar13);
      thunk_FUN_140073670(*(undefined8 *)(param_1 + 0x394b8),local_2f0,
                          -(uint)(*(char *)(param_1 + 0x30a54) != '\0') & 3000);
      uVar6 = QString::fromUtf8((char *)&local_308,(int)param_1 + 0x28a54);
      thunk_FUN_140087cb0(*(undefined8 *)(param_1 + 0x394b8),uVar6);
      QString::~QString((QString *)&local_308);
    }
  }
  if (*(int *)local_2e8 != 0) {
    if (*(int *)local_2e8 == -1) goto LAB_140072b60;
    LOCK();
    iVar1 = *(int *)local_2e8;
    *(int *)local_2e8 = *(int *)local_2e8 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_140072b60;
  }
  QListData::dispose(local_2e8);
LAB_140072b60:
  thunk_FUN_140039310(local_2f0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140074080
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140074080(longlong param_1,undefined8 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  ushort uVar5;
  undefined8 *puVar6;
  char cVar7;
  Algorithm AVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 *puVar11;
  QByteArray *pQVar12;
  QSslCertificate *pQVar13;
  QSslKey *pQVar14;
  undefined8 uVar15;
  uint *puVar16;
  uint *puVar17;
  char *pcVar18;
  basic_ostream<char,std::char_traits<char>_> *this;
  longlong lVar19;
  undefined8 *puVar20;
  uint uVar21;
  undefined1 auStackY_1e8 [32];
  uint local_1b8 [2];
  QByteArray local_1b0 [8];
  QByteArray local_1a8 [8];
  QSslCertificate local_1a0 [8];
  QSslCertificate *local_198 [2];
  longlong local_188;
  QString local_180 [8];
  QSslCertificate local_178 [8];
  QSslCertificate local_170 [8];
  QSslCertificate local_168 [8];
  QSslCertificate local_160 [4];
  int iStack_15c;
  undefined *local_158;
  basic_streambuf<char,std::char_traits<char>_> local_150 [128];
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  undefined8 local_68;
  uint *local_60;
  uint *local_58;
  ulonglong local_48;
  ulonglong local_40;
  
  local_68 = 0xfffffffffffffffe;
  local_40 = DAT_14036a720 ^ (ulonglong)auStackY_1e8;
  puVar17 = (uint *)0x0;
  uVar21 = 0;
  local_1b8[0] = 0;
  if (*(char *)(param_1 + 0x10) == '\0') {
    return;
  }
  if (*(char *)(param_1 + 0x3946c) != '\0') {
    return;
  }
  if (*(char *)(param_1 + 0x394d0) == '\0') {
    return;
  }
  uVar9 = thunk_FUN_1400c9380(local_198);
  cVar7 = thunk_FUN_1400c8f40(uVar9,20000);
  thunk_FUN_1400c7da0(local_198);
  if (cVar7 != '\0') {
    local_158 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_d0);
    uVar21 = 1;
    local_1b8[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_158,local_150,false);
    *(undefined ***)(local_150 + (longlong)*(int *)(local_158 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    thunk_FUN_14002d040(local_150,2);
    thunk_FUN_14002cab0(&local_158,"slave startup");
    uVar9 = thunk_FUN_140030920(&local_158,&local_60);
    uVar10 = thunk_FUN_1400c9380(local_198);
    thunk_FUN_1400c8570(uVar10,20000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp"
                       );
    thunk_FUN_1400c7da0(local_198);
    if (0xf < local_48) {
      puVar16 = local_60;
      if (0xfff < local_48 + 1) {
        if (((ulonglong)local_60 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        puVar16 = *(uint **)(local_60 + -2);
        if (local_60 <= puVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_60 - (longlong)puVar16) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_60 - (longlong)puVar16)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(puVar16);
    }
    thunk_FUN_14002d2f0(local_d0);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_d0)
    ;
  }
  *(undefined1 *)(param_1 + 0x3946c) = 1;
  *(undefined1 *)(param_1 + 0x12) = 1;
  lVar19 = 0x500;
  puVar6 = (undefined8 *)(param_1 + 0x18);
  do {
    puVar20 = param_2;
    puVar11 = puVar6;
    uVar9 = puVar20[1];
    *puVar11 = *puVar20;
    puVar11[1] = uVar9;
    uVar9 = puVar20[3];
    puVar11[2] = puVar20[2];
    puVar11[3] = uVar9;
    uVar9 = puVar20[5];
    puVar11[4] = puVar20[4];
    puVar11[5] = uVar9;
    uVar9 = puVar20[7];
    puVar11[6] = puVar20[6];
    puVar11[7] = uVar9;
    uVar9 = puVar20[9];
    puVar11[8] = puVar20[8];
    puVar11[9] = uVar9;
    uVar9 = puVar20[0xb];
    puVar11[10] = puVar20[10];
    puVar11[0xb] = uVar9;
    uVar9 = puVar20[0xd];
    puVar11[0xc] = puVar20[0xc];
    puVar11[0xd] = uVar9;
    uVar9 = puVar20[0xf];
    puVar11[0xe] = puVar20[0xe];
    puVar11[0xf] = uVar9;
    lVar19 = lVar19 + -1;
    puVar6 = puVar11 + 0x10;
    param_2 = puVar20 + 0x10;
  } while (lVar19 != 0);
  uVar2 = *(undefined4 *)((longlong)puVar20 + 0x84);
  uVar3 = *(undefined4 *)(puVar20 + 0x11);
  uVar4 = *(undefined4 *)((longlong)puVar20 + 0x8c);
  *(undefined4 *)(puVar11 + 0x10) = *(undefined4 *)(puVar20 + 0x10);
  *(undefined4 *)((longlong)puVar11 + 0x84) = uVar2;
  *(undefined4 *)(puVar11 + 0x11) = uVar3;
  *(undefined4 *)((longlong)puVar11 + 0x8c) = uVar4;
  uVar2 = *(undefined4 *)((longlong)puVar20 + 0x94);
  uVar3 = *(undefined4 *)(puVar20 + 0x13);
  uVar4 = *(undefined4 *)((longlong)puVar20 + 0x9c);
  *(undefined4 *)(puVar11 + 0x12) = *(undefined4 *)(puVar20 + 0x12);
  *(undefined4 *)((longlong)puVar11 + 0x94) = uVar2;
  *(undefined4 *)(puVar11 + 0x13) = uVar3;
  *(undefined4 *)((longlong)puVar11 + 0x9c) = uVar4;
  puVar11[0x14] = puVar20[0x14];
  thunk_FUN_140087da0(*(undefined8 *)(param_1 + 0x394b8));
  QByteArray::QByteArray(local_1b0);
  iVar1 = (int)param_1;
  pQVar12 = (QByteArray *)QByteArray::fromRawData((char *)local_1b8,iVar1 + 0x1c);
  QByteArray::operator=(local_1b0,pQVar12);
  QByteArray::~QByteArray((QByteArray *)local_1b8);
  pQVar13 = (QSslCertificate *)
            QSslCertificate::QSslCertificate((QSslCertificate *)local_1b8,local_1b0,1);
  QSslCertificate::operator=((QSslCertificate *)(param_1 + 0x39478),pQVar13);
  QSslCertificate::~QSslCertificate((QSslCertificate *)local_1b8);
  pQVar12 = (QByteArray *)QByteArray::fromRawData((char *)local_1b8,iVar1 + 0x8020);
  QByteArray::operator=(local_1b0,pQVar12);
  QByteArray::~QByteArray((QByteArray *)local_1b8);
  pQVar12 = (QByteArray *)QByteArray::QByteArray(local_1a8);
  pQVar14 = (QSslKey *)QSslKey::QSslKey((QSslKey *)local_1b8,local_1b0,1,1,0,pQVar12);
  QSslKey::operator=((QSslKey *)(param_1 + 0x39480),pQVar14);
  QSslKey::~QSslKey((QSslKey *)local_1b8);
  QByteArray::~QByteArray(local_1a8);
  pQVar12 = (QByteArray *)QByteArray::QByteArray(local_1a8);
  QSslCertificate::QSslCertificate(local_168,pQVar12,0);
  QByteArray::~QByteArray(local_1a8);
  pQVar12 = (QByteArray *)QByteArray::QByteArray(local_1a8);
  QSslCertificate::QSslCertificate(local_170,pQVar12,0);
  QByteArray::~QByteArray(local_1a8);
  pQVar12 = (QByteArray *)QByteArray::QByteArray(local_1a8);
  QSslCertificate::QSslCertificate(local_178,pQVar12,0);
  QByteArray::~QByteArray(local_1a8);
  pQVar12 = (QByteArray *)QByteArray::fromRawData((char *)local_1b8,iVar1 + 0x10024);
  QByteArray::operator=(local_1b0,pQVar12);
  QByteArray::~QByteArray((QByteArray *)local_1b8);
  pQVar13 = (QSslCertificate *)
            QSslCertificate::QSslCertificate((QSslCertificate *)local_1b8,local_1b0,1);
  QSslCertificate::operator=(local_168,pQVar13);
  QSslCertificate::~QSslCertificate((QSslCertificate *)local_1b8);
  pQVar12 = (QByteArray *)QByteArray::fromRawData((char *)local_1b8,iVar1 + 0x18028);
  QByteArray::operator=(local_1b0,pQVar12);
  QByteArray::~QByteArray((QByteArray *)local_1b8);
  pQVar13 = (QSslCertificate *)
            QSslCertificate::QSslCertificate((QSslCertificate *)local_1b8,local_1b0,1);
  QSslCertificate::operator=(local_170,pQVar13);
  QSslCertificate::~QSslCertificate((QSslCertificate *)local_1b8);
  pQVar12 = (QByteArray *)QByteArray::fromRawData((char *)local_1b8,iVar1 + 0x2002c);
  QByteArray::operator=(local_1b0,pQVar12);
  QByteArray::~QByteArray((QByteArray *)local_1b8);
  pQVar13 = (QSslCertificate *)
            QSslCertificate::QSslCertificate((QSslCertificate *)local_1b8,local_1b0,1);
  QSslCertificate::operator=(local_178,pQVar13);
  QSslCertificate::~QSslCertificate((QSslCertificate *)local_1b8);
  local_60 = local_1b8;
  local_198[0] = local_160;
  uVar9 = QSslCertificate::QSslCertificate((QSslCertificate *)local_1b8,local_178);
  uVar10 = QSslCertificate::QSslCertificate(local_160,local_170);
  uVar15 = QSslCertificate::QSslCertificate(local_1a0,local_168);
  thunk_FUN_140144c90(param_1 + 0x39488,uVar15,uVar10,uVar9);
  *(undefined8 *)(param_1 + 0x394a8) = *(undefined8 *)(param_1 + 0x28030);
  *(undefined8 *)(param_1 + 0x394b0) = *(undefined8 *)(param_1 + 0x28038);
  QString::QString((QString *)&local_188);
  local_198[0] = operator_new(0x18);
  puVar16 = puVar17;
  if (local_198[0] != (QSslCertificate *)0x0) {
    puVar16 = (uint *)thunk_FUN_140195880(local_198[0],*(undefined8 *)(param_1 + 0x394a8),
                                          *(undefined8 *)(param_1 + 0x394b0));
  }
  local_60 = puVar16;
  if (puVar16 == (uint *)0x0) {
    local_58 = (uint *)0x0;
  }
  else {
    puVar17 = operator_new(0x18);
    local_198[0] = (QSslCertificate *)(puVar17 + 4);
    if (local_198[0] != (QSslCertificate *)0x0) {
      *(uint **)local_198[0] = puVar16;
    }
    if (puVar17 != (uint *)0x0) {
      *(undefined1 **)(puVar17 + 2) = &LAB_14000702c;
      puVar17[1] = 1;
      *puVar17 = 1;
    }
    local_58 = puVar17;
    thunk_FUN_140036f80(puVar17,puVar16,1);
    FUN_140012daf(&local_60,puVar16);
  }
  thunk_FUN_140072d60(*(undefined8 *)(param_1 + 0x394b8),param_1 + 0x39478,param_1 + 0x39480,
                      param_1 + 0x39488);
  thunk_FUN_14006c5c0(puVar17);
  thunk_FUN_140073910(*(undefined8 *)(param_1 + 0x394b8),&local_188);
  uVar5 = (ushort)local_1b8[0];
  if (*(int *)(local_188 + 4) == 0) {
    uVar9 = thunk_FUN_1400c9380(local_198);
    cVar7 = thunk_FUN_1400c8f40(uVar9,20000);
    thunk_FUN_1400c7da0(local_198);
    if (cVar7 == '\0') goto LAB_140074a09;
    local_158 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_d0);
    local_1b8[0] = uVar21 | 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_158,local_150,false);
    *(undefined ***)(local_150 + (longlong)*(int *)(local_158 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    thunk_FUN_14002d040(local_150,2);
    thunk_FUN_14002cab0(&local_158,"slave unable to start server");
    uVar9 = thunk_FUN_140030920(&local_158,&local_60);
    uVar10 = thunk_FUN_1400c9380(local_198);
    thunk_FUN_1400c8570(uVar10,20000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp"
                       );
    thunk_FUN_1400c7da0(local_198);
    if (0xf < local_48) {
      puVar17 = local_60;
      if (0xfff < local_48 + 1) {
        if (((ulonglong)local_60 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        puVar17 = *(uint **)(local_60 + -2);
        if (local_60 <= puVar17) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_60 - (longlong)puVar17) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_60 - (longlong)puVar17)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(puVar17);
    }
  }
  else {
    uVar9 = thunk_FUN_1400c9380(local_198);
    cVar7 = thunk_FUN_1400c8f40(uVar9,20000);
    thunk_FUN_1400c7da0(local_198);
    if (cVar7 == '\0') goto LAB_140074a09;
    local_158 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_d0);
    local_1b8[0] = uVar21 | 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_158,local_150,false);
    *(undefined ***)(local_150 + (longlong)*(int *)(local_158 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    thunk_FUN_14002d040(local_150,2);
    uVar9 = thunk_FUN_14002cab0(&local_158,"slave start server: ");
    pQVar12 = (QByteArray *)QString::toLocal8Bit((QString *)&local_188);
    pcVar18 = QByteArray::data(pQVar12);
    thunk_FUN_14002cab0(uVar9,pcVar18);
    QByteArray::~QByteArray((QByteArray *)local_1a0);
    this = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14002cab0(uVar9,&DAT_140273bd4);
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,uVar5);
    uVar9 = thunk_FUN_140030920(&local_158,&local_60);
    uVar10 = thunk_FUN_1400c9380(local_198);
    thunk_FUN_1400c8570(uVar10,20000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave.cpp"
                       );
    thunk_FUN_1400c7da0(local_198);
    if (0xf < local_48) {
      puVar17 = local_60;
      if (0xfff < local_48 + 1) {
        if (((ulonglong)local_60 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        puVar17 = *(uint **)(local_60 + -2);
        if (local_60 <= puVar17) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if ((ulonglong)((longlong)local_60 - (longlong)puVar17) < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < (ulonglong)((longlong)local_60 - (longlong)puVar17)) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(puVar17);
    }
  }
  thunk_FUN_14002d2f0(local_d0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_d0);
LAB_140074a09:
  pQVar12 = (QByteArray *)QString::toUtf8((QString *)&local_188);
  pcVar18 = QByteArray::operator_char_const____ptr64(pQVar12);
  strncpy((char *)(param_1 + 0x30a56),pcVar18,0xff);
  QByteArray::~QByteArray((QByteArray *)local_1a0);
  *(ushort *)(param_1 + 0x31456) = uVar5;
  AVar8 = QSslCertificate::toPem((QSslCertificate *)(param_1 + 0x39478));
  pQVar12 = (QByteArray *)QCryptographicHash::hash((QByteArray *)local_160,AVar8);
  pQVar12 = (QByteArray *)QByteArray::toHex(pQVar12);
  QString::QString(local_180,pQVar12);
  QByteArray::~QByteArray((QByteArray *)local_1a0);
  QByteArray::~QByteArray((QByteArray *)local_160);
  QByteArray::~QByteArray(local_1a8);
  thunk_FUN_140087cd0(*(undefined8 *)(param_1 + 0x394b8),local_180);
  pQVar12 = (QByteArray *)QString::toUtf8(local_180);
  pcVar18 = QByteArray::constData(pQVar12);
  strncpy((char *)(param_1 + 0x3146a),pcVar18,0x7fff);
  QByteArray::~QByteArray((QByteArray *)local_1b8);
  *(undefined1 *)(param_1 + 0x3946a) = 1;
  *(undefined1 *)(param_1 + 0x11) = 1;
  QString::~QString(local_180);
  QString::~QString((QString *)&local_188);
  QSslCertificate::~QSslCertificate(local_178);
  QSslCertificate::~QSslCertificate(local_170);
  QSslCertificate::~QSslCertificate(local_168);
  QByteArray::~QByteArray(local_1b0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14008f0e0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QObject * FUN_14008f0e0(QObject *param_1,QString *param_2,undefined8 *param_3,QString *param_4,
                       QObject *param_5)

{
  bool bVar1;
  char cVar2;
  QObject QVar3;
  int iVar4;
  QByteArray *pQVar5;
  char *pcVar6;
  char *pcVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  QLocalSocket *this;
  QObject *pQVar10;
  basic_ostream<char,std::char_traits<char>_> *this_00;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar11;
  ulonglong uVar12;
  QLocalSocket *pQVar13;
  undefined1 auStackY_208 [32];
  longlong local_1d0 [2];
  undefined8 local_1c0;
  undefined *local_1b8;
  undefined **local_1b0;
  basic_ostream<char,std::char_traits<char>_> local_1a8 [96];
  undefined8 local_148;
  undefined4 local_140;
  basic_ios<char,std::char_traits<char>_> local_130 [104];
  QString *local_c8;
  undefined8 local_c0;
  QObject *local_b8;
  QString *local_b0;
  undefined8 *local_a8;
  QString *local_a0;
  QLocalSocket *local_98 [3];
  ulonglong local_80;
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58;
  
  local_c0 = 0xfffffffffffffffe;
  local_58 = DAT_14036a720 ^ (ulonglong)auStackY_208;
  pQVar13 = (QLocalSocket *)0x0;
  local_1c0 = param_4;
  local_c8 = param_2;
  local_b8 = param_1;
  local_b0 = param_2;
  local_a8 = param_3;
  local_a0 = param_4;
  QObject::QObject(param_1,param_5);
  *(undefined ***)param_1 = grabberAgent::subAgentIo::Bridge::vftable;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x20) = 0;
  QString::QString((QString *)(param_1 + 0x28),param_2);
  param_1[0x30] = (QObject)0x0;
  uVar8 = param_3[1];
  *(undefined8 *)(param_1 + 0x38) = *param_3;
  *(undefined8 *)(param_1 + 0x40) = uVar8;
  *(code **)(param_1 + 0x48) = shared_null_exref;
  QString::QString((QString *)(param_1 + 0x50),param_4);
  QMetaObject::normalizedType((char *)local_1d0);
  QByteArray::constData((QByteArray *)local_1d0);
  pQVar5 = (QByteArray *)QMetaObject::normalizedType((char *)local_98);
  iVar4 = *(int *)(local_1d0[0] + 4);
  if (iVar4 == *(int *)(*(longlong *)pQVar5 + 4)) {
    pcVar6 = QByteArray::constData(pQVar5);
    pcVar7 = QByteArray::constData((QByteArray *)local_1d0);
    iVar4 = memcmp(pcVar7,pcVar6,(longlong)iVar4);
    if (iVar4 != 0) goto LAB_14008f202;
  }
  else {
LAB_14008f202:
    qt_assert_x("qRegisterNormalizedMetaType",
                "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
                ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
  }
  QByteArray::~QByteArray((QByteArray *)local_98);
  QMetaType::registerNormalizedType(local_1d0,thunk_FUN_140090440,thunk_FUN_140090410,4);
  QByteArray::~QByteArray((QByteArray *)local_1d0);
  QMetaObject::normalizedType((char *)local_1d0);
  QByteArray::constData((QByteArray *)local_1d0);
  pQVar5 = (QByteArray *)QMetaObject::normalizedType((char *)local_98);
  iVar4 = *(int *)(local_1d0[0] + 4);
  if (iVar4 == *(int *)(*(longlong *)pQVar5 + 4)) {
    pcVar6 = QByteArray::constData(pQVar5);
    pcVar7 = QByteArray::constData((QByteArray *)local_1d0);
    iVar4 = memcmp(pcVar7,pcVar6,(longlong)iVar4);
    if (iVar4 == 0) goto LAB_14008f2f0;
  }
  qt_assert_x("qRegisterNormalizedMetaType",
              "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
              ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
LAB_14008f2f0:
  QByteArray::~QByteArray((QByteArray *)local_98);
  QMetaType::registerNormalizedType(local_1d0,thunk_FUN_140090430,thunk_FUN_1400903f0,4);
  QByteArray::~QByteArray((QByteArray *)local_1d0);
  if (DAT_140375720 == (char *)0x0) {
    DAT_140375720 = (char *)param_1;
    this = operator_new(0x10);
    local_98[0] = this;
    if (this != (QLocalSocket *)0x0) {
      QLocalSocket::QLocalSocket(this,(QObject *)0x0);
      *(undefined ***)this = QLocalSocket::vftable;
      pQVar13 = this;
    }
    thunk_FUN_140092bf0(param_1 + 0x10,pQVar13);
    pcVar6 = *(char **)(param_1 + 0x10);
    if (pcVar6 == (char *)0x0) {
      uVar8 = thunk_FUN_1400c9380(local_98);
      cVar2 = thunk_FUN_1400c8f40(uVar8);
      thunk_FUN_1400c7da0(local_98);
      if (cVar2 != '\0') {
        local_1b8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_130);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
        *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
             *(int *)(local_1b8 + 4) + -0x88;
        thunk_FUN_14002d040(&local_1b0,2);
        thunk_FUN_14002cab0(&local_1b8,"Failed to allocate new memory");
        uVar8 = thunk_FUN_140030920(&local_1b8,local_78);
        uVar9 = thunk_FUN_1400c9380(local_98);
        thunk_FUN_1400c8570(uVar9,30000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                           );
        thunk_FUN_1400c7da0(local_98);
        if (0xf < local_60) {
          uVar12 = local_78[0];
          if (0xfff < local_60 + 1) {
            if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar12 = *(ulonglong *)(local_78[0] - 8);
            if (local_78[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_78[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_78[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar12);
        }
        thunk_FUN_14002d2f0(local_130);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_130);
        param_4 = local_1c0;
      }
    }
    else {
      pcVar7 = qFlagLocation("1ipcDisconnected()");
      pQVar10 = (QObject *)qFlagLocation("2disconnected()");
      QObject::connect((QObject *)local_98,pcVar6,pQVar10,(char *)param_1,(ConnectionType)pcVar7);
      QMetaObject::Connection::~Connection((Connection *)local_98);
      pcVar6 = *(char **)(param_1 + 0x10);
      pcVar7 = qFlagLocation("1onSocketError(QLocalSocket::LocalSocketError)");
      pQVar10 = (QObject *)qFlagLocation("2error(QLocalSocket::LocalSocketError)");
      QObject::connect((QObject *)local_98,pcVar6,pQVar10,(char *)param_1,(ConnectionType)pcVar7);
      QMetaObject::Connection::~Connection((Connection *)local_98);
      thunk_FUN_14008ea80(local_98,*(undefined8 *)(param_1 + 0x10),stateChanged_exref,0);
      QMetaObject::Connection::~Connection((Connection *)local_98);
      iVar4 = 5;
      do {
        QVar3 = (QObject)thunk_FUN_140092d70(param_1);
        param_1[0x30] = QVar3;
        if (QVar3 == (QObject)0x0) {
          uVar8 = thunk_FUN_1400c9380(local_1d0);
          cVar2 = thunk_FUN_1400c8f40(uVar8);
          thunk_FUN_1400c7da0(local_1d0);
          if (cVar2 != '\0') {
            local_1b8 = &DAT_1402738f0;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_130);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
            *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
                 *(int *)(local_1b8 + 4) + -0x88;
            local_98[0] = (QLocalSocket *)&local_1b0;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
            local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_148 = 0;
            local_140 = 4;
            this_00 = (basic_ostream<char,std::char_traits<char>_> *)
                      thunk_FUN_14002cab0(&local_1b8,"Failed start sub agent, retry (");
            pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(this_00,iVar4);
            thunk_FUN_14002cab0(pbVar11,&DAT_1402a9b34);
            uVar8 = thunk_FUN_140030920(&local_1b8,local_98);
            uVar9 = thunk_FUN_1400c9380(local_78);
            thunk_FUN_1400c8570(uVar9,40000,uVar8,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                               );
            thunk_FUN_1400c7da0(local_78);
            if (0xf < local_80) {
              pQVar13 = local_98[0];
              if (0xfff < local_80 + 1) {
                if (((ulonglong)local_98[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                pQVar13 = *(QLocalSocket **)(local_98[0] + -8);
                if (local_98[0] <= pQVar13) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if ((ulonglong)((longlong)local_98[0] - (longlong)pQVar13) < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < (ulonglong)((longlong)local_98[0] - (longlong)pQVar13)) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d06(pQVar13);
            }
            *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
                 *(int *)(local_1b8 + 4) + -0x88;
            local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14002e520(&local_1b0);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_1a8);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_130);
          }
        }
        param_4 = local_1c0;
        if (param_1[0x30] != (QObject)0x0) goto LAB_14008f9bb;
        bVar1 = 0 < iVar4;
        iVar4 = iVar4 + -1;
      } while (bVar1);
      if (param_1[0x30] == (QObject)0x0) {
        QLocalSocket::disconnectFromServer(*(QLocalSocket **)(param_1 + 0x10));
        param_4 = local_1c0;
        if (*(HANDLE *)(param_1 + 0x38) != (HANDLE)0xffffffffffffffff) {
          TerminateProcess(*(HANDLE *)(param_1 + 0x38),0);
          CloseHandle(*(HANDLE *)(param_1 + 0x38));
          *(undefined8 *)(param_1 + 0x38) = 0xffffffffffffffff;
          param_4 = local_1c0;
        }
      }
    }
  }
  else {
    uVar8 = thunk_FUN_1400c9380(local_98);
    cVar2 = thunk_FUN_1400c8f40(uVar8);
    thunk_FUN_1400c7da0(local_98);
    if (cVar2 != '\0') {
      local_1b8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_130);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
      *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
           *(int *)(local_1b8 + 4) + -0x88;
      local_98[0] = (QLocalSocket *)&local_1b0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
      local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_148 = 0;
      local_140 = 4;
      thunk_FUN_14002cab0(&local_1b8,"Attempt to init singleton twice");
      uVar8 = thunk_FUN_140030920(&local_1b8,local_78);
      uVar9 = thunk_FUN_1400c9380(local_98);
      thunk_FUN_1400c8570(uVar9,30000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                         );
      thunk_FUN_1400c7da0(local_98);
      if (0xf < local_60) {
        uVar12 = local_78[0];
        if (0xfff < local_60 + 1) {
          if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar12 = *(ulonglong *)(local_78[0] - 8);
          if (local_78[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_78[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_78[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar12);
      }
      *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_1c0 + (longlong)*(int *)(local_1b8 + 4) + 4) =
           *(int *)(local_1b8 + 4) + -0x88;
      local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_1b0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_1a8);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_130);
      param_4 = local_1c0;
    }
  }
LAB_14008f9bb:
  QString::~QString(local_c8);
  if ((HANDLE)*param_3 != (HANDLE)0xffffffffffffffff) {
    TerminateProcess((HANDLE)*param_3,0);
    CloseHandle((HANDLE)*param_3);
    *param_3 = 0xffffffffffffffff;
  }
  QString::~QString(param_4);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140092d70
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140092d70(longlong param_1)

{
  longlong *plVar1;
  undefined1 auVar2 [16];
  char cVar3;
  bool bVar4;
  int iVar5;
  DWORD DVar6;
  BOOL BVar7;
  undefined8 uVar8;
  basic_ostream<char,std::char_traits<char>_> *pbVar9;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar10;
  undefined8 uVar11;
  QString *pQVar12;
  QDir *this;
  void **ppvVar13;
  __uint64 _Var14;
  LPWSTR lpCommandLine;
  ushort *_Src;
  LPCWSTR lpCurrentDirectory;
  LPCWSTR lpApplicationName;
  QByteArray *pQVar15;
  char *pcVar16;
  undefined8 uVar17;
  ulonglong uVar18;
  uint uVar19;
  longlong lVar20;
  uint uVar21;
  uint uVar22;
  undefined1 auStackY_308 [32];
  undefined8 local_2a8;
  code *local_2a0;
  QTypedArrayData<unsigned_short> *local_298;
  undefined ***local_290 [2];
  undefined ***local_280 [2];
  HANDLE local_270;
  longlong local_268;
  int iStack_25c;
  undefined *local_258;
  undefined **local_250;
  basic_ostream<char,std::char_traits<char>_> local_248 [96];
  undefined8 local_1e8;
  undefined4 local_1e0;
  basic_ios<char,std::char_traits<char>_> local_1d0 [104];
  LPVOID local_168;
  QString local_160 [8];
  QString local_158 [8];
  QDir local_150 [8];
  QString local_148 [8];
  _PROCESS_INFORMATION local_140;
  undefined8 local_128;
  undefined1 local_120 [16];
  undefined1 local_110 [16];
  undefined1 local_100 [24];
  _STARTUPINFOW local_e8;
  ulonglong local_78 [3];
  ulonglong local_60;
  undefined8 local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_128 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStackY_308;
  uVar19 = 0;
  local_2a8 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_2a8 & 0xffffffff00000000);
  uVar22 = uVar19;
  if ((*(longlong *)(param_1 + 0x38) != -1) &&
     (cVar3 = thunk_FUN_140095250(param_1 + 0x38), uVar22 = 0, cVar3 != '\0')) {
    uVar8 = thunk_FUN_1400c9380(local_290);
    cVar3 = thunk_FUN_1400c8f40(uVar8,30000);
    thunk_FUN_1400c7da0(local_290);
    if (cVar3 != '\0') {
      local_258 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1d0);
      uVar22 = 1;
      local_2a8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_2a8._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      thunk_FUN_14002d040(&local_250,2);
      thunk_FUN_14002cab0(&local_258,"Process already started");
      thunk_FUN_140030920(&local_258,local_78);
      uVar8 = thunk_FUN_1400c9380(local_290);
      thunk_FUN_1400c8570(uVar8,30000);
      thunk_FUN_1400c7da0(local_290);
      if (0xf < local_60) {
        uVar18 = local_78[0];
        if (0xfff < local_60 + 1) {
          if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar18 = *(ulonglong *)(local_78[0] - 8);
          if (local_78[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_78[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_78[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar18);
      }
      thunk_FUN_14002d2f0(local_1d0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1d0);
    }
  }
  thunk_FUN_140090830(param_1,local_158);
  local_2a0 = shared_null_exref;
  local_298 = QString::fromAscii_helper("-logging-postfix",0x10);
  local_2a8 = QString::fromAscii_helper("-ipc-name",9);
  thunk_FUN_140039960(&local_2a0,&local_2a8);
  thunk_FUN_140039960(&local_2a0,local_158);
  thunk_FUN_140039960(&local_2a0,&local_298);
  thunk_FUN_140039960(&local_2a0,param_1 + 0x50);
  QString::~QString((QString *)&local_2a8);
  QString::~QString((QString *)&local_298);
  local_270 = (HANDLE)0x0;
  iVar5 = WTSQueryUserToken(*(undefined4 *)(param_1 + 0x40),&local_270);
  uVar21 = uVar22;
  if (iVar5 == 0) {
    uVar8 = thunk_FUN_1400c9380(local_290);
    cVar3 = thunk_FUN_1400c8f40(uVar8,30000);
    thunk_FUN_1400c7da0(local_290);
    if (cVar3 != '\0') {
      local_258 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1d0);
      uVar21 = uVar22 | 2;
      local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar22) | 2);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_290[0] = &local_250;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1e8 = 0;
      local_1e0 = 4;
      DVar6 = GetLastError();
      pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_258,"Failed to WTSQueryUserToken for sesssion ");
      pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar9,*(ulong *)(param_1 + 0x40));
      pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar10,&DAT_1402a3634);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
      uVar8 = thunk_FUN_140030920(&local_258,local_78);
      uVar11 = thunk_FUN_1400c9380(local_290);
      thunk_FUN_1400c8570(uVar11,30000,uVar8);
      thunk_FUN_1400c7da0(local_290);
      if (0xf < local_60) {
        uVar18 = local_78[0];
        if (0xfff < local_60 + 1) {
          if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar18 = *(ulonglong *)(local_78[0] - 8);
          if (local_78[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_78[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_78[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar18);
      }
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_250);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_248);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1d0);
    }
  }
  thunk_FUN_14002d820(&local_298,param_1 + 0x28,&DAT_1402771ec);
  QDir::toNativeSeparators(local_160);
  QString::~QString((QString *)&local_298);
  pQVar12 = (QString *)QCoreApplication::applicationDirPath();
  this = (QDir *)QDir::QDir(local_150,pQVar12);
  QDir::absolutePath(this);
  QDir::toNativeSeparators(local_148);
  QString::~QString((QString *)&local_2a8);
  QDir::~QDir(local_150);
  QString::~QString((QString *)local_280);
  uVar8 = thunk_FUN_14002d8b0(local_290,&DAT_1402a9bc4,local_160);
  thunk_FUN_14002d820(&local_268,uVar8,&DAT_1402a9bc4);
  QString::~QString((QString *)local_290);
  iVar5 = QListData::size((QListData *)&local_2a0);
  if (0 < iVar5) {
    do {
      if (((int)uVar19 < 0) ||
         (iVar5 = QListData::size((QListData *)&local_2a0), iVar5 <= (int)uVar19)) {
        qt_assert_x("QList<T>::operator[]","index out of range",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
      }
      if (1 < *(uint *)local_2a0) {
        thunk_FUN_14003e930(&local_2a0,*(uint *)(local_2a0 + 4));
      }
      ppvVar13 = QListData::at((QListData *)&local_2a0,uVar19);
      pQVar12 = (QString *)thunk_FUN_14002d8b0(local_280,&DAT_1402a96bc,ppvVar13);
      QString::append((QString *)&local_268,pQVar12);
      QString::~QString((QString *)local_280);
      uVar19 = uVar19 + 1;
      iVar5 = QListData::size((QListData *)&local_2a0);
    } while ((int)uVar19 < iVar5);
  }
  auVar2._8_8_ = 0;
  auVar2._0_8_ = (longlong)(*(int *)(local_268 + 4) + 1);
  _Var14 = SUB168(ZEXT816(2) * auVar2,0);
  if (SUB168(ZEXT816(2) * auVar2,8) != 0) {
    _Var14 = 0xffffffffffffffff;
  }
  lpCommandLine = operator_new(_Var14);
  _Src = QString::utf16((QString *)&local_268);
  memmove(lpCommandLine,_Src,(longlong)*(int *)(local_268 + 4) * 2);
  lpCommandLine[*(int *)(local_268 + 4)] = L'\0';
  local_168 = (LPVOID)0x0;
  iVar5 = CreateEnvironmentBlock(&local_168);
  if (iVar5 == 0) {
    uVar8 = thunk_FUN_1400c9380(local_280);
    cVar3 = thunk_FUN_1400c8f40(uVar8,40000);
    thunk_FUN_1400c7da0(local_280);
    if (cVar3 != '\0') {
      local_258 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1d0);
      local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar21) | 4);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_290[0] = &local_250;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1e8 = 0;
      local_1e0 = 4;
      DVar6 = GetLastError();
      pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_258,"Failed to call CreateEnvironmentBlock for sesssion ")
      ;
      pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                          (pbVar9,*(ulong *)(param_1 + 0x40));
      pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(pbVar10,&DAT_1402a3634);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
      uVar8 = thunk_FUN_140030920(&local_258,local_78);
      uVar11 = thunk_FUN_1400c9380(local_290);
      thunk_FUN_1400c8570(uVar11,40000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                         );
      thunk_FUN_1400c7da0(local_290);
      if (0xf < local_60) {
        uVar18 = local_78[0];
        if (0xfff < local_60 + 1) {
          if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar18 = *(ulonglong *)(local_78[0] - 8);
          if (local_78[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_78[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_78[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar18);
      }
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_250);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_248);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1d0);
    }
  }
  else {
    local_140.hProcess = (HANDLE)0x0;
    local_140.hThread = (HANDLE)0x0;
    local_140.dwProcessId = 0;
    local_140.dwThreadId = 0;
    memset(&local_e8,0,0x68);
    local_e8.cb = 0x68;
    local_e8.dwFlags = 1;
    local_e8.wShowWindow = 5;
    lpCurrentDirectory = (LPCWSTR)QString::utf16(local_148);
    lpApplicationName = (LPCWSTR)QString::utf16(local_160);
    BVar7 = CreateProcessAsUserW
                      (local_270,lpApplicationName,lpCommandLine,(LPSECURITY_ATTRIBUTES)0x0,
                       (LPSECURITY_ATTRIBUTES)0x0,0,0x400,local_168,lpCurrentDirectory,&local_e8,
                       &local_140);
    if (BVar7 != 0) {
      *(HANDLE *)(param_1 + 0x38) = local_140.hProcess;
      uVar8 = thunk_FUN_1400c9380(local_280);
      cVar3 = thunk_FUN_1400c8f40(uVar8,20000);
      thunk_FUN_1400c7da0(local_280);
      uVar22 = uVar21;
      if (cVar3 != '\0') {
        local_258 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_1d0);
        uVar22 = uVar21 | 0x10;
        local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar21) | 0x10);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
        *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
             *(int *)(local_258 + 4) + -0x88;
        local_280[0] = &local_250;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
        local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_1e8 = 0;
        local_1e0 = 4;
        DVar6 = GetProcessId(*(HANDLE *)(param_1 + 0x38));
        pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_258,"subAgent startted with pid ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
        uVar8 = thunk_FUN_140030920(&local_258,local_78);
        uVar11 = thunk_FUN_1400c9380(local_280);
        thunk_FUN_1400c8570(uVar11,20000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                           );
        thunk_FUN_1400c7da0(local_280);
        if (0xf < local_60) {
          uVar18 = local_78[0];
          if (0xfff < local_60 + 1) {
            if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar18 = *(ulonglong *)(local_78[0] - 8);
            if (local_78[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_78[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_78[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar18);
        }
        *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
             *(int *)(local_258 + 4) + -0x88;
        local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_250);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_248);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_1d0);
      }
      thunk_FUN_140002d06(lpCommandLine);
      iVar5 = 5;
      do {
        uVar8 = thunk_FUN_1400c9380(local_120);
        cVar3 = thunk_FUN_1400c8f40(uVar8,20000);
        thunk_FUN_1400c7da0(local_120);
        if (cVar3 != '\0') {
          local_258 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_1d0);
          local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar22) | 0x20);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
          *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
               *(int *)(local_258 + 4) + -0x88;
          thunk_FUN_14002d040(&local_250,2);
          DVar6 = GetProcessId(*(HANDLE *)(param_1 + 0x38));
          pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(&local_258,"Connecting to sub worker with PID ");
          pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
          uVar8 = thunk_FUN_14002cab0(pbVar10," via ");
          pQVar15 = (QByteArray *)QString::toLocal8Bit(local_158);
          pcVar16 = QByteArray::data(pQVar15);
          thunk_FUN_14002cab0(uVar8,pcVar16);
          QByteArray::~QByteArray((QByteArray *)local_280);
          pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14002cab0(uVar8,&DAT_1402a9c64);
          pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,iVar5);
          thunk_FUN_14002cab0(pbVar10,&DAT_1402a9b34);
          uVar8 = thunk_FUN_140030920(&local_258,local_58);
          uVar11 = thunk_FUN_1400c9380(local_110);
          thunk_FUN_1400c8570(uVar11,20000,uVar8,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                             );
          thunk_FUN_1400c7da0(local_110);
          if (0xf < local_40) {
            thunk_FUN_14002dff0(local_58[0],local_40 + 1,1);
          }
          thunk_FUN_14002d2f0(local_1d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_1d0);
          uVar22 = uVar22 | 0x20;
        }
        QThread::msleep(500);
        QLocalSocket::connectToServer(*(QLocalSocket **)(param_1 + 0x10),local_158);
        bVar4 = QLocalSocket::waitForConnected(*(QLocalSocket **)(param_1 + 0x10),30000);
        if (bVar4) break;
        uVar8 = thunk_FUN_1400c9380(local_100);
        cVar3 = thunk_FUN_1400c8f40(uVar8,40000);
        thunk_FUN_1400c7da0(local_100);
        if (cVar3 != '\0') {
          local_258 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_1d0);
          local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar22) | 0x40);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
          *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
               *(int *)(local_258 + 4) + -0x88;
          thunk_FUN_14002d040(&local_250,2);
          pQVar12 = (QString *)QIODevice::errorString(*(QIODevice **)(param_1 + 0x10));
          uVar8 = thunk_FUN_14002cab0(&local_258,"Failed to connect to IPC: ");
          pQVar15 = (QByteArray *)QString::toLocal8Bit(pQVar12);
          pcVar16 = QByteArray::data(pQVar15);
          thunk_FUN_14002cab0(uVar8,pcVar16);
          QByteArray::~QByteArray((QByteArray *)local_150);
          QString::~QString((QString *)&local_298);
          uVar8 = thunk_FUN_140030920(&local_258,local_58);
          uVar11 = thunk_FUN_1400c9380(local_78);
          thunk_FUN_1400c8570(uVar11,40000,uVar8,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                             );
          thunk_FUN_1400c7da0(local_78);
          if (0xf < local_40) {
            thunk_FUN_14002dff0(local_58[0],local_40 + 1);
          }
          thunk_FUN_14002d2f0(local_1d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_1d0);
          uVar22 = uVar22 | 0x40;
        }
        bVar4 = 0 < iVar5;
        iVar5 = iVar5 + -1;
      } while (bVar4);
      plVar1 = *(longlong **)(param_1 + 0x20);
      lVar20 = 0;
      if (plVar1 != (longlong *)0x0) {
        (**(code **)(*plVar1 + 0x18))(plVar1,1);
      }
      local_280[0] = operator_new(0x30);
      if (local_280[0] != (undefined ***)0x0) {
        lVar20 = thunk_FUN_14016fee0(local_280[0],param_1 + 0x10);
      }
      *(longlong *)(param_1 + 0x20) = lVar20;
      if ((lVar20 == 0) || (cVar3 = thunk_FUN_140175350(lVar20), cVar3 == '\0')) {
        uVar8 = 0;
        uVar11 = thunk_FUN_1400c9380(local_78);
        cVar3 = thunk_FUN_1400c8f40(uVar11);
        thunk_FUN_1400c7da0(local_78);
        if (cVar3 != '\0') {
          local_258 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_1d0);
          local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar22) | 0x80);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
          *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
               *(int *)(local_258 + 4) + -0x88;
          thunk_FUN_14002d040(&local_250,2);
          thunk_FUN_14002cab0(&local_258,"Failed to create RPC object");
          uVar11 = thunk_FUN_140030920(&local_258,local_58);
          uVar17 = thunk_FUN_1400c9380(local_78);
          thunk_FUN_1400c8570(uVar17,40000,uVar11,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                             );
          thunk_FUN_1400c7da0(local_78);
          thunk_FUN_14002d390(local_58);
          thunk_FUN_14002d2f0(local_1d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_1d0);
        }
        QLocalSocket::abort(*(QLocalSocket **)(param_1 + 0x10));
        if (*(HANDLE *)(param_1 + 0x38) != (HANDLE)0xffffffffffffffff) {
          TerminateProcess(*(HANDLE *)(param_1 + 0x38),0);
          CloseHandle(*(HANDLE *)(param_1 + 0x38));
          *(undefined8 *)(param_1 + 0x38) = 0xffffffffffffffff;
        }
        DestroyEnvironmentBlock(local_168);
        CloseHandle(local_270);
      }
      else {
        uVar8 = 1;
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onUserInfo",param_1,"onUserInfo");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onSystemProxies",param_1,
                            "onSystemProxies");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onFoundText",param_1,"onFoundText");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onFoundHtml",param_1,"onFoundHtml");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onFoundImage",param_1,"onFoundImage");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onFoundUrls",param_1,"onFoundUrls");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"onFoundOther",param_1,"onFoundOther");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"getClipboardUrlsResult",param_1,
                            "getClipboardUrlsResult");
        thunk_FUN_140170640(*(undefined8 *)(param_1 + 0x20),"uiRequest",param_1,"onUiRequest");
        local_298 = QString::fromAscii_helper("getClipboardUrls",0x10);
        thunk_FUN_140174b00(*(undefined8 *)(param_1 + 0x20),&local_298);
        QString::~QString((QString *)&local_298);
      }
      goto LAB_1400940c4;
    }
    uVar8 = thunk_FUN_1400c9380(local_280);
    cVar3 = thunk_FUN_1400c8f40(uVar8,40000);
    thunk_FUN_1400c7da0(local_280);
    if (cVar3 != '\0') {
      local_258 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1d0);
      local_2a8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_2a8._4_4_,uVar21) | 8);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_290[0] = &local_250;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1e8 = 0;
      local_1e0 = 4;
      DVar6 = GetLastError();
      pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14002cab0(&local_258,"Failed to CreateProcessAsUser: ");
      pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
      uVar8 = thunk_FUN_14002cab0(pbVar10,", hNewToken: ");
      pQVar15 = (QByteArray *)QString::toLocal8Bit(local_160);
      pcVar16 = QByteArray::data(pQVar15);
      thunk_FUN_14002cab0(uVar8,pcVar16);
      QByteArray::~QByteArray((QByteArray *)local_290);
      uVar8 = thunk_FUN_140030920(&local_258,local_78);
      uVar11 = thunk_FUN_1400c9380(local_290);
      thunk_FUN_1400c8570(uVar11,40000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-subAgentIo\\bridge.cpp"
                         );
      thunk_FUN_1400c7da0(local_290);
      if (0xf < local_60) {
        uVar18 = local_78[0];
        if (0xfff < local_60 + 1) {
          if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar18 = *(ulonglong *)(local_78[0] - 8);
          if (local_78[0] <= uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_78[0] - uVar18 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_78[0] - uVar18) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar18);
      }
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_250);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_248);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1d0);
    }
    DestroyEnvironmentBlock(local_168);
  }
  CloseHandle(local_270);
  uVar8 = 0;
LAB_1400940c4:
  QString::~QString((QString *)&local_268);
  QString::~QString(local_148);
  QString::~QString(local_160);
  thunk_FUN_140039310(&local_2a0);
  QString::~QString(local_158);
  return uVar8;
}

// =============================================================================
// AUDIO
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401b4230
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

bool FUN_1401b4230(longlong param_1,int *param_2,uint *param_3)

{
  char *pcVar1;
  int *piVar2;
  undefined ***pppuVar3;
  uint *puVar4;
  byte bVar5;
  ulonglong uVar6;
  char cVar7;
  int iVar8;
  ulonglong uVar9;
  longlong *plVar10;
  void *pvVar11;
  void *pvVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined ****ppppuVar15;
  longlong lVar16;
  longlong lVar18;
  undefined1 *puVar19;
  byte *pbVar20;
  byte *_Src;
  int *piVar21;
  uint uVar22;
  byte *pbVar23;
  bool bVar24;
  undefined1 auStack_2a8 [32];
  undefined4 local_288;
  undefined ***local_278;
  undefined ***local_270;
  uint local_268;
  int *local_260;
  int *local_258;
  QTypedArrayData<unsigned_short> *local_250;
  QTypedArrayData<unsigned_short> *local_248;
  QMutexLocker local_240 [8];
  uint *local_238;
  longlong local_230;
  undefined8 local_228;
  void *local_220;
  undefined1 local_218 [16];
  undefined1 local_208 [12];
  int iStack_1fc;
  undefined *local_1f8;
  undefined **local_1f0;
  basic_ostream<char,std::char_traits<char>_> local_1e8 [96];
  undefined8 local_188;
  undefined4 local_180;
  basic_ios<char,std::char_traits<char>_> local_170 [104];
  int local_108;
  int local_104;
  int local_100;
  undefined1 local_f8;
  undefined7 uStack_f7;
  undefined ***local_e8;
  ulonglong local_e0;
  undefined1 local_d8;
  undefined7 uStack_d7;
  undefined ***local_c8;
  ulonglong local_c0;
  byte local_b8;
  undefined7 uStack_b7;
  undefined ***local_a8;
  ulonglong local_a0;
  byte local_98;
  undefined7 uStack_97;
  undefined ***local_88;
  ulonglong local_80;
  int local_78;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  longlong lVar17;
  
  local_228 = 0xfffffffffffffffe;
  local_48 = DAT_14036a720 ^ (ulonglong)auStack_2a8;
  local_268 = 0;
  local_258 = param_2;
  local_238 = param_3;
  uVar9 = thunk_FUN_1401b5420();
  if (*param_3 < uVar9) {
    *param_3 = 0;
    bVar24 = true;
  }
  else {
    *param_2 = 0;
    local_260 = param_2 + 1;
    if (0x1d0 < *param_3) {
      local_230 = param_1 + 0x10;
      uVar22 = 0;
      do {
        lVar17 = local_230;
        piVar21 = local_260;
        local_e0 = 0xf;
        local_e8 = (undefined ***)0x0;
        local_f8 = 0;
        local_c0 = 0xf;
        local_c8 = (undefined ***)0x0;
        local_d8 = 0;
        local_a0 = 0xf;
        local_a8 = (undefined ***)0x0;
        local_b8 = 0;
        local_80 = 0xf;
        local_88 = (undefined ***)0x0;
        local_98 = 0;
        iVar8 = _Mtx_lock(local_230);
        if (iVar8 != 0) {
          std::_Throw_C_error(iVar8);
        }
        if (*(longlong *)(param_1 + 0x80) == 0) {
          iVar8 = _Mtx_unlock(lVar17);
          if (iVar8 != 0) {
            std::_Throw_C_error(iVar8);
          }
          thunk_FUN_14002d390(&local_98);
          thunk_FUN_14002d390(&local_b8);
          thunk_FUN_14002d390(&local_d8);
          thunk_FUN_14002d390(&local_f8);
          param_2 = local_258;
          break;
        }
        plVar10 = (longlong *)thunk_FUN_1401b3d00(param_1 + 0x60,local_68);
        pvVar11 = (void *)0x0;
        if ((undefined8 *)*plVar10 != (undefined8 *)0x0) {
          pvVar11 = *(void **)*plVar10;
        }
        piVar2 = *(int **)(*(longlong *)((longlong)pvVar11 + 8) +
                          (*(longlong *)((longlong)pvVar11 + 0x10) - 1U & plVar10[2]) * 8);
        local_108 = *piVar2;
        local_104 = piVar2[1];
        local_100 = piVar2[2];
        thunk_FUN_1400d7790(&local_f8,piVar2 + 4);
        thunk_FUN_1400d7790(&local_d8,piVar2 + 0xc);
        thunk_FUN_1400d7790(&local_b8,piVar2 + 0x14);
        thunk_FUN_1400d7790(&local_98);
        local_78 = piVar2[0x24];
        lVar16 = *(longlong *)
                  (*(longlong *)(param_1 + 0x68) +
                  (*(longlong *)(param_1 + 0x70) - 1U & *(ulonglong *)(param_1 + 0x78)) * 8);
        thunk_FUN_14002d390(lVar16 + 0x70);
        thunk_FUN_14002d390(lVar16 + 0x50);
        thunk_FUN_14002d390(lVar16 + 0x30);
        thunk_FUN_14002d390(lVar16 + 0x10);
        plVar10 = (longlong *)(param_1 + 0x80);
        *plVar10 = *plVar10 + -1;
        if (*plVar10 == 0) {
          *(undefined8 *)(param_1 + 0x78) = 0;
        }
        else {
          *(longlong *)(param_1 + 0x78) = *(longlong *)(param_1 + 0x78) + 1;
        }
        iVar8 = _Mtx_unlock(lVar17);
        if (iVar8 != 0) {
          std::_Throw_C_error(iVar8);
        }
        pvVar11 = DAT_140376db8;
        if (DAT_140376db8 == (void *)0x0) {
          QMutexLocker::QMutexLocker(local_240,(QBasicMutex *)&DAT_140376dc8);
          pvVar11 = DAT_140376db8;
          if ((DAT_140376db8 == (void *)0x0) &&
             (pvVar12 = operator_new(8), pvVar11 = (void *)0x0, local_220 = pvVar12,
             pvVar12 != (void *)0x0)) {
            lVar17 = -1;
            do {
              lVar16 = lVar17 + 1;
              pcVar1 = &DAT_1402d004d + lVar17;
              lVar17 = lVar16;
            } while (*pcVar1 != '\0');
            local_250 = QString::fromAscii_helper("audio",(int)lVar16);
            thunk_FUN_14016a3f0(pvVar12);
            QString::~QString((QString *)&local_250);
            pvVar11 = pvVar12;
          }
          DAT_140376db8 = pvVar11;
          pvVar11 = DAT_140376db8;
          QMutexLocker::~QMutexLocker(local_240);
        }
        lVar17 = -1;
        do {
          lVar18 = lVar17 + 1;
          lVar16 = lVar17 + 1;
          lVar17 = lVar18;
        } while ("agentAudio"[lVar16] != '\0');
        local_248 = QString::fromAscii_helper("agentAudio",(int)lVar18);
        cVar7 = thunk_FUN_14016a4c0(pvVar11,&local_248);
        QString::~QString((QString *)&local_248);
        if (cVar7 != '\0') {
          uVar13 = thunk_FUN_1400c9380(local_218);
          cVar7 = thunk_FUN_1400c8f40(uVar13,0);
          thunk_FUN_1400c7da0(local_218);
          if (cVar7 != '\0') {
            local_1f8 = &DAT_1402738f0;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_170);
            local_268 = uVar22 | 1;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1f8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1f0,false);
            *(undefined ***)((longlong)&local_1f8 + (longlong)*(int *)(local_1f8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1fc + (longlong)*(int *)(local_1f8 + 4)) =
                 *(int *)(local_1f8 + 4) + -0x88;
            local_278 = &local_1f0;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1f0);
            local_1f0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_188 = 0;
            local_180 = 4;
            uVar13 = thunk_FUN_14002cab0(&local_1f8,"Fetch session from device \'");
            uVar13 = thunk_FUN_1400d3c50(uVar13,&local_98);
            uVar13 = thunk_FUN_14002cab0(uVar13,"\' for process \'");
            thunk_FUN_1400d3c50(uVar13,&local_f8);
            uVar13 = thunk_FUN_140030920(&local_1f8,local_68);
            uVar14 = thunk_FUN_1400c9380(local_208);
            local_288 = 0x4b;
            thunk_FUN_1400c8570(uVar14,0,uVar13,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumerator.cpp"
                               );
            thunk_FUN_1400c7da0(local_208);
            if (0xf < local_50) {
              uVar9 = local_68[0];
              if (0xfff < local_50 + 1) {
                if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar9 = *(ulonglong *)(local_68[0] - 8);
                if (local_68[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_68[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_68[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d06(uVar9);
            }
            *(undefined ***)((longlong)&local_1f8 + (longlong)*(int *)(local_1f8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1fc + (longlong)*(int *)(local_1f8 + 4)) =
                 *(int *)(local_1f8 + 4) + -0x88;
            local_1f0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14002e520(&local_1f0);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_1f0);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_1e8);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_170);
          }
        }
        *piVar21 = local_108;
        piVar21[1] = local_104;
        piVar21[2] = local_100;
        local_270 = local_e8;
        local_278 = (undefined ***)0x7f;
        ppppuVar15 = &local_270;
        if ((undefined ***)0x7e < local_e8) {
          ppppuVar15 = &local_278;
        }
        pppuVar3 = *ppppuVar15;
        puVar19 = &local_f8;
        if (0xf < local_e0) {
          puVar19 = (undefined1 *)CONCAT71(uStack_f7,local_f8);
        }
        memcpy(piVar21 + 3,puVar19,(size_t)pppuVar3);
        memset((void *)((longlong)pppuVar3 + 0xcU + (longlong)piVar21),0,0x80 - (longlong)pppuVar3);
        local_278 = local_c8;
        local_270 = (undefined ***)0x7f;
        ppppuVar15 = &local_278;
        if ((undefined ***)0x7e < local_c8) {
          ppppuVar15 = &local_270;
        }
        pppuVar3 = *ppppuVar15;
        puVar19 = &local_d8;
        if (0xf < local_c0) {
          puVar19 = (undefined1 *)CONCAT71(uStack_d7,local_d8);
        }
        memcpy(piVar21 + 0x23,puVar19,(size_t)pppuVar3);
        memset((void *)((longlong)pppuVar3 + (longlong)(piVar21 + 0x23)),0,0x80 - (longlong)pppuVar3
              );
        uVar9 = local_a0;
        local_278 = local_a8;
        local_270 = (undefined ***)0x3f;
        ppppuVar15 = &local_278;
        if ((undefined ***)0x3e < local_a8) {
          ppppuVar15 = &local_270;
        }
        pppuVar3 = *ppppuVar15;
        pbVar23 = (byte *)CONCAT71(uStack_b7,local_b8);
        pbVar20 = &local_b8;
        if (0xf < local_a0) {
          pbVar20 = pbVar23;
        }
        memcpy(piVar21 + 0x43,pbVar20,(size_t)pppuVar3);
        memset((void *)((longlong)pppuVar3 + (longlong)(piVar21 + 0x43)),0,0x40 - (longlong)pppuVar3
              );
        uVar6 = local_80;
        bVar5 = local_98;
        local_278 = local_88;
        local_270 = (undefined ***)0x7f;
        ppppuVar15 = &local_278;
        if ((undefined ***)0x7e < local_88) {
          ppppuVar15 = &local_270;
        }
        pppuVar3 = *ppppuVar15;
        pbVar20 = (byte *)CONCAT71(uStack_97,local_98);
        _Src = &local_98;
        if (0xf < local_80) {
          _Src = pbVar20;
        }
        piVar21 = local_260 + 0x53;
        memcpy(piVar21,_Src,(size_t)pppuVar3);
        memset((void *)((longlong)pppuVar3 + (longlong)piVar21),0,0x80 - (longlong)pppuVar3);
        puVar4 = local_238;
        local_260[0x73] = local_78;
        *local_238 = *local_238 - 0x1d0;
        *local_258 = *local_258 + 1;
        local_260 = local_260 + 0x74;
        if (0xf < uVar6) {
          pbVar23 = pbVar20;
          if (0xfff < uVar6 + 1) {
            if ((bVar5 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            pbVar23 = *(byte **)(pbVar20 + -8);
            if (pbVar20 <= pbVar23) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if ((ulonglong)((longlong)pbVar20 - (longlong)pbVar23) < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < (ulonglong)((longlong)pbVar20 - (longlong)pbVar23)) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(pbVar23);
          pbVar23 = (byte *)CONCAT71(uStack_b7,local_b8);
          uVar9 = local_a0;
        }
        local_80 = 0xf;
        local_88 = (undefined ***)0x0;
        local_98 = 0;
        if (0xf < uVar9) {
          pbVar20 = pbVar23;
          if (0xfff < uVar9 + 1) {
            if ((local_b8 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            pbVar20 = *(byte **)(pbVar23 + -8);
            if (pbVar23 <= pbVar20) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if ((ulonglong)((longlong)pbVar23 - (longlong)pbVar20) < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < (ulonglong)((longlong)pbVar23 - (longlong)pbVar20)) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(pbVar20);
        }
        local_a0 = 0xf;
        local_a8 = (undefined ***)0x0;
        local_b8 = 0;
        thunk_FUN_14002d390(&local_d8);
        thunk_FUN_14002d390(&local_f8);
        param_2 = local_258;
        uVar22 = local_268;
      } while (0x1d0 < *puVar4);
    }
    bVar24 = *param_2 != 0;
  }
  return bVar24;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401b5690
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401b5690(longlong param_1,uint *param_2)

{
  char cVar1;
  undefined8 uVar2;
  basic_ostream<char,std::char_traits<char>_> *this;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  char *pcVar6;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  QTypedArrayData<unsigned_short> *local_168;
  undefined ***local_160 [2];
  undefined8 local_150;
  undefined *local_148;
  undefined **local_140;
  basic_ostream<char,std::char_traits<char>_> local_138 [96];
  undefined8 local_d8;
  undefined4 local_d0;
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  uint *local_58;
  ulonglong local_50 [3];
  ulonglong local_38;
  ulonglong local_30;
  
  local_150 = 0xfffffffffffffffe;
  local_30 = DAT_14036a720 ^ (ulonglong)auStack_198;
  local_168 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_168 & 0xffffffff00000000);
  local_58 = param_2;
  uVar2 = thunk_FUN_1401b54b0();
  local_168 = QString::fromAscii_helper("agentAudio",10);
  cVar1 = thunk_FUN_14016a4c0(uVar2,&local_168);
  QString::~QString((QString *)&local_168);
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_1400c9380(local_160);
    cVar1 = thunk_FUN_1400c8f40(uVar2,0);
    thunk_FUN_1400c7da0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_168._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_160[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      pcVar6 = "Stop";
      if (param_2[0x24] == 1) {
        pcVar6 = "Start";
      }
      uVar2 = thunk_FUN_14002cab0(&local_148,pcVar6);
      uVar2 = thunk_FUN_14002cab0(uVar2," session from device \'");
      uVar2 = thunk_FUN_1400d3c50(uVar2,param_2 + 0x1c);
      uVar2 = thunk_FUN_14002cab0(uVar2,"\' for process \'");
      uVar2 = thunk_FUN_1400d3c50(uVar2,param_2 + 4);
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14002cab0(uVar2,&DAT_1402d0124);
      pbVar3 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,*param_2);
      thunk_FUN_14002cab0(pbVar3,&DAT_1402a9b34);
      uVar2 = thunk_FUN_140030920(&local_148,local_50);
      uVar4 = thunk_FUN_1400c9380(local_160);
      local_178 = 0x8c;
      thunk_FUN_1400c8570(uVar4,0,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumerator.cpp"
                         );
      thunk_FUN_1400c7da0(local_160);
      if (0xf < local_38) {
        uVar5 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar5);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  uVar2 = thunk_FUN_1401b22c0(&local_148,param_2);
  FUN_1401b1780(param_1 + 0x60,uVar2,param_1 + 0x10);
  thunk_FUN_14002d390(param_2 + 0x1c);
  thunk_FUN_14002d390(param_2 + 0x14);
  thunk_FUN_14002d390(param_2 + 0xc);
  thunk_FUN_14002d390(param_2 + 4);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401b5a70
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401b5a70(longlong param_1,uint *param_2)

{
  ulonglong uVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  QTypedArrayData<unsigned_short> *pQVar5;
  ulonglong uVar6;
  undefined1 auStack_1b8 [32];
  undefined4 local_198;
  undefined8 local_188;
  undefined8 local_180;
  undefined *local_178;
  basic_streambuf<char,std::char_traits<char>_> local_170 [128];
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  QTypedArrayData<unsigned_short> *local_88 [3];
  ulonglong local_70;
  uint local_68;
  undefined4 uStack_64;
  undefined8 local_60;
  uint local_58;
  ulonglong local_50;
  undefined8 local_48;
  undefined8 local_40;
  ulonglong local_38;
  
  local_188 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_1b8;
  local_88[0] = (QTypedArrayData<unsigned_short> *)((ulonglong)local_88[0] & 0xffffffff00000000);
  local_180 = param_2;
  if (*(longlong *)(param_2 + 6) != *(longlong *)(param_2 + 8)) {
    uVar3 = thunk_FUN_1401b54b0();
    local_88[0] = QString::fromAscii_helper("agentAudio",10);
    cVar2 = thunk_FUN_14016a4c0(uVar3,local_88);
    QString::~QString((QString *)local_88);
    if (cVar2 != '\0') {
      uVar3 = thunk_FUN_1400c9380(local_88);
      cVar2 = thunk_FUN_1400c8f40(uVar3,0);
      thunk_FUN_1400c7da0(local_88);
      if (cVar2 != '\0') {
        local_178 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        local_88[0] = (QTypedArrayData<unsigned_short> *)CONCAT44(local_88[0]._4_4_,1);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,local_170,false);
        *(undefined ***)(local_170 + (longlong)*(int *)(local_178 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
             *(int *)(local_178 + 4) + -0x88;
        thunk_FUN_14002d040(local_170,2);
        uVar3 = thunk_FUN_1401b6500(param_2,local_88);
        uVar4 = thunk_FUN_14002cab0(&local_178,"AudioStream push frame: ");
        thunk_FUN_1400d3c50(uVar4,uVar3);
        if (0xf < local_70) {
          pQVar5 = local_88[0];
          if (0xfff < local_70 + 1) {
            if (((ulonglong)local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            pQVar5 = *(QTypedArrayData<unsigned_short> **)(local_88[0] + -8);
            if (local_88[0] <= pQVar5) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if ((ulonglong)((longlong)local_88[0] - (longlong)pQVar5) < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < (ulonglong)((longlong)local_88[0] - (longlong)pQVar5)) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(pQVar5);
        }
        uVar3 = thunk_FUN_140030920(&local_178,&local_68);
        uVar4 = thunk_FUN_1400c9380(local_88);
        local_198 = 0x95;
        thunk_FUN_1400c8570(uVar4,0,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumerator.cpp"
                           );
        thunk_FUN_1400c7da0(local_88);
        if (0xf < local_50) {
          uVar1 = CONCAT44(uStack_64,local_68);
          uVar6 = uVar1;
          if (0xfff < local_50 + 1) {
            if ((local_68 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(uVar1 - 8);
            if (uVar1 <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (uVar1 - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < uVar1 - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar6);
        }
        thunk_FUN_14002d2f0(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
    local_68 = *param_2;
    local_60 = *(undefined8 *)(param_2 + 2);
    local_58 = param_2[4];
    local_50 = *(ulonglong *)(param_2 + 6);
    local_48 = *(undefined8 *)(param_2 + 8);
    local_40 = *(undefined8 *)(param_2 + 10);
    param_2[6] = 0;
    param_2[7] = 0;
    param_2[8] = 0;
    param_2[9] = 0;
    param_2[10] = 0;
    param_2[0xb] = 0;
    FUN_1401b1870(param_1 + 0x88,&local_68,param_1 + 0x10);
  }
  thunk_FUN_1401b3800(param_2 + 6);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401b5f30
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401b5f30(longlong *param_1,char param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  char *pcVar5;
  undefined4 uVar6;
  undefined1 auStack_1a8 [32];
  undefined4 local_188;
  undefined8 local_178;
  undefined ***local_170 [2];
  int iStack_15c;
  undefined *local_158;
  undefined **local_150;
  basic_ostream<char,std::char_traits<char>_> local_148 [96];
  undefined8 local_e8;
  undefined4 local_e0;
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  undefined8 local_68;
  ulonglong local_60 [3];
  ulonglong local_48;
  ulonglong local_40;
  
  local_68 = 0xfffffffffffffffe;
  local_40 = DAT_14036a720 ^ (ulonglong)auStack_1a8;
  local_178 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_178 & 0xffffffff00000000);
  uVar2 = thunk_FUN_1401b54b0();
  local_178 = QString::fromAscii_helper("agentAudio",10);
  cVar1 = thunk_FUN_14016a4c0(uVar2,&local_178);
  QString::~QString((QString *)&local_178);
  uVar6 = 0;
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_1400c9380(local_170);
    cVar1 = thunk_FUN_1400c8f40(uVar2,10000);
    thunk_FUN_1400c7da0(local_170);
    uVar6 = 0;
    if (cVar1 != '\0') {
      local_158 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      uVar6 = 1;
      local_178 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_178._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_170[0] = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      pcVar5 = "disabling";
      if (param_2 != '\0') {
        pcVar5 = "enabling";
      }
      uVar2 = thunk_FUN_14002cab0(&local_158,"AudioEnumerator::setAudioEnabled: ");
      uVar2 = thunk_FUN_14002cab0(uVar2,pcVar5);
      thunk_FUN_14002cab0(uVar2," catching audio");
      uVar2 = thunk_FUN_140030920(&local_158,local_60);
      uVar3 = thunk_FUN_1400c9380(local_170);
      local_188 = 0xd0;
      thunk_FUN_1400c8570(uVar3,10000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumerator.cpp"
                         );
      thunk_FUN_1400c7da0(local_170);
      if (0xf < local_48) {
        uVar4 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar4 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar4);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14002e520(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
  }
  if (*(char *)((longlong)param_1 + 9) == param_2) {
    uVar2 = thunk_FUN_1401b54b0();
    local_178 = QString::fromAscii_helper("agentAudio",10);
    cVar1 = thunk_FUN_14016a4c0(uVar2,&local_178);
    QString::~QString((QString *)&local_178);
    if (cVar1 != '\0') {
      uVar2 = thunk_FUN_1400c9380(local_170);
      cVar1 = thunk_FUN_1400c8f40(uVar2,10000);
      thunk_FUN_1400c7da0(local_170);
      if (cVar1 != '\0') {
        local_158 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        local_178 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_178._4_4_,uVar6) | 2);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        thunk_FUN_14002d040(&local_150,2);
        thunk_FUN_14002cab0(&local_158,"AudioEnumerator::setAudioEnabled: state is unchanged, skip")
        ;
        uVar2 = thunk_FUN_140030920(&local_158,local_60);
        uVar3 = thunk_FUN_1400c9380(local_170);
        local_188 = 0xd2;
        thunk_FUN_1400c8570(uVar3,10000,uVar2,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumerator.cpp"
                           );
        thunk_FUN_1400c7da0(local_170);
        if (0xf < local_48) {
          uVar4 = local_60[0];
          if (0xfff < local_48 + 1) {
            if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar4 = *(ulonglong *)(local_60[0] - 8);
            if (local_60[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_60[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_60[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar4);
        }
        thunk_FUN_14002d2f0(local_d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
      }
    }
  }
  else {
    *(char *)((longlong)param_1 + 9) = param_2;
    if (param_2 == '\0') {
      (**(code **)(*param_1 + 0x18))(param_1);
    }
    else {
      (**(code **)(*param_1 + 0x20))();
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d2670
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d2670(longlong param_1)

{
  longlong *plVar1;
  int *piVar2;
  longlong lVar3;
  LPVOID pvVar4;
  char cVar5;
  int iVar6;
  HRESULT HVar7;
  DWORD DVar8;
  undefined8 uVar9;
  basic_ostream<char,std::char_traits<char>_> *pbVar10;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar11;
  undefined8 uVar12;
  ulonglong uVar13;
  basic_ios<char,std::char_traits<char>_> *this;
  undefined1 auStackY_4b8 [32];
  QTypedArrayData<unsigned_short> *local_480;
  LPVOID local_478;
  LPVOID local_470;
  longlong local_468 [2];
  undefined1 local_458 [8];
  longlong *local_450;
  undefined1 local_448 [16];
  undefined1 local_438 [16];
  undefined1 local_428 [16];
  undefined1 local_418 [16];
  undefined1 local_408 [16];
  undefined1 local_3f8 [12];
  int iStack_3ec;
  undefined *local_3e8;
  basic_streambuf<char,std::char_traits<char>_> local_3e0 [128];
  basic_ios<char,std::char_traits<char>_> local_360 [100];
  int iStack_2fc;
  undefined *local_2f8;
  basic_streambuf<char,std::char_traits<char>_> local_2f0 [128];
  basic_ios<char,std::char_traits<char>_> local_270 [100];
  int iStack_20c;
  undefined *local_208;
  basic_streambuf<char,std::char_traits<char>_> local_200 [128];
  basic_ios<char,std::char_traits<char>_> local_180 [104];
  longlong local_118 [7];
  longlong *local_e0;
  LPVOID local_d8;
  ulonglong local_c8 [3];
  ulonglong local_b0;
  ulonglong local_a8 [3];
  ulonglong local_90;
  ulonglong local_88 [3];
  ulonglong local_70;
  undefined **local_68 [7];
  undefined ***local_30;
  ulonglong local_28;
  
  local_468[1] = 0xfffffffffffffffe;
  local_28 = DAT_14036a720 ^ (ulonglong)auStackY_4b8;
  *(undefined1 *)(param_1 + 8) = 1;
  thunk_FUN_1401d4180();
  iVar6 = _Mtx_lock(param_1 + 0x1f0);
  if (iVar6 != 0) {
    std::_Throw_C_error(iVar6);
  }
  *(undefined1 *)(param_1 + 0x1e8) = 0;
  iVar6 = _Mtx_unlock(param_1 + 0x1f0);
  if (iVar6 != 0) {
    std::_Throw_C_error(iVar6);
  }
  local_468[0] = param_1;
  uVar9 = thunk_FUN_1401be660(local_458,local_468);
  thunk_FUN_1401c24d0(param_1 + 0x1d8,uVar9);
  if (local_450 != (longlong *)0x0) {
    LOCK();
    plVar1 = local_450 + 1;
    lVar3 = *plVar1;
    *(int *)plVar1 = (int)*plVar1 + -1;
    UNLOCK();
    if ((int)lVar3 == 1) {
      (**(code **)*local_450)();
      LOCK();
      piVar2 = (int *)((longlong)local_450 + 0xc);
      iVar6 = *piVar2;
      *piVar2 = *piVar2 + -1;
      UNLOCK();
      if (iVar6 == 1) {
        (**(code **)(*local_450 + 8))();
      }
    }
  }
  HVar7 = CoInitializeEx((LPVOID)0x0,2);
  if (HVar7 < 0) {
    uVar9 = thunk_FUN_1401b54b0();
    local_480 = QString::fromAscii_helper("agentAudio",10);
    cVar5 = thunk_FUN_14016a4c0(uVar9,&local_480);
    QString::~QString((QString *)&local_480);
    if (cVar5 == '\0') {
      return;
    }
    uVar9 = thunk_FUN_1400c9380(local_448);
    cVar5 = thunk_FUN_1400c8f40(uVar9,40000);
    thunk_FUN_1400c7da0(local_448);
    if (cVar5 == '\0') {
      return;
    }
    local_3e8 = &DAT_1402738f0;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_360)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_3e8,local_3e0,false);
    *(undefined ***)(local_3e0 + (longlong)*(int *)(local_3e8 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_3ec + (longlong)*(int *)(local_3e8 + 4)) =
         *(int *)(local_3e8 + 4) + -0x88;
    thunk_FUN_14002d040(local_3e0,2);
    DVar8 = GetLastError();
    pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14002cab0(&local_3e8,
                                  "AudioEnumeratorImpl::init: failed to initialize. Reason: \'");
    pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,DVar8);
    thunk_FUN_14002cab0(pbVar11,&DAT_1402bd320);
    uVar9 = thunk_FUN_140030920(&local_3e8,local_c8);
    uVar12 = thunk_FUN_1400c9380(local_438);
    thunk_FUN_1400c8570(uVar12,40000,uVar9,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                       );
    thunk_FUN_1400c7da0(local_438);
    if (0xf < local_b0) {
      uVar13 = local_c8[0];
      if (0xfff < local_b0 + 1) {
        if ((local_c8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar13 = *(ulonglong *)(local_c8[0] - 8);
        if (local_c8[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_c8[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_c8[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d06(uVar13);
    }
    thunk_FUN_14002d2f0(local_360);
    this = local_360;
  }
  else {
    local_478 = (LPVOID)0x0;
    HVar7 = CoCreateInstance((IID *)&DAT_1402d1c98,(LPUNKNOWN)0x0,1,(IID *)&DAT_1402d1cb0,&local_478
                            );
    pvVar4 = local_478;
    if (HVar7 < 0) {
      uVar9 = thunk_FUN_1401b54b0();
      cVar5 = thunk_FUN_1401b3920(uVar9);
      if (cVar5 == '\0') {
        return;
      }
      uVar9 = thunk_FUN_1400c9380(local_428);
      cVar5 = thunk_FUN_1400c8f40(uVar9,40000);
      thunk_FUN_1400c7da0(local_428);
      if (cVar5 == '\0') {
        return;
      }
      local_2f8 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_270);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_2f8,local_2f0,false);
      *(undefined ***)(local_2f0 + (longlong)*(int *)(local_2f8 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_2fc + (longlong)*(int *)(local_2f8 + 4)) =
           *(int *)(local_2f8 + 4) + -0x88;
      thunk_FUN_14002d040(local_2f0,2);
      DVar8 = GetLastError();
      pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(&local_2f8,
                                    "AudioEnumeratorImpl::init: failed to create device enumirator. Reason: \'"
                                   );
      pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,DVar8);
      thunk_FUN_14002cab0(pbVar11,&DAT_1402bd320);
      uVar9 = thunk_FUN_140030920(&local_2f8,local_a8);
      uVar12 = thunk_FUN_1400c9380(local_418);
      thunk_FUN_1400c8570(uVar12,40000,uVar9,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_418);
      if (0xf < local_90) {
        uVar13 = local_a8[0];
        if (0xfff < local_90 + 1) {
          if ((local_a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar13 = *(ulonglong *)(local_a8[0] - 8);
          if (local_a8[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_a8[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_a8[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar13);
      }
      thunk_FUN_14002d2f0(local_270);
      this = local_270;
    }
    else {
      local_68[0] = std::
                    _Func_impl<<lambda_f5942bf8dc2e8f10c8d3896f3e7f289b>,std::allocator<int>,void,IMMDeviceEnumerator*___ptr64>
                    ::vftable;
      local_30 = local_68;
      thunk_FUN_1401bf700(local_118,local_68);
      local_d8 = pvVar4;
      thunk_FUN_1401c2590(param_1 + 200,local_118);
      if (local_d8 != (LPVOID)0x0) {
        local_470 = local_d8;
        if (local_e0 == (longlong *)0x0) {
                    /* WARNING: Subroutine does not return */
          std::_Xbad_function_call();
        }
        (**(code **)(*local_e0 + 0x10))(local_e0,&local_470);
      }
      if (local_e0 != (longlong *)0x0) {
        (**(code **)(*local_e0 + 0x20))
                  (local_e0,CONCAT71((int7)((ulonglong)local_118 >> 8),local_e0 != local_118));
        local_e0 = (longlong *)0x0;
      }
      if (local_30 != (undefined ***)0x0) {
        (*(code *)(*local_30)[4])
                  (local_30,CONCAT71((int7)((ulonglong)local_68 >> 8),local_30 != local_68));
      }
      uVar9 = thunk_FUN_1401b54b0();
      cVar5 = thunk_FUN_1401b3920(uVar9);
      if (cVar5 == '\0') {
        return;
      }
      uVar9 = thunk_FUN_1400c9380(local_408);
      cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
      thunk_FUN_1400c7da0(local_408);
      if (cVar5 == '\0') {
        return;
      }
      local_208 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_180);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_208,local_200,false);
      *(undefined ***)(local_200 + (longlong)*(int *)(local_208 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_20c + (longlong)*(int *)(local_208 + 4)) =
           *(int *)(local_208 + 4) + -0x88;
      thunk_FUN_14002d040(local_200,2);
      cVar5 = *(char *)(param_1 + 0x2d0);
      pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_14002cab0(&local_208,
                                    "AudioEnumeratorImpl::init: audio enumerator inited, recordOnlyWhenCaptureActive="
                                   );
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,(uint)(cVar5 != '\0'));
      uVar9 = thunk_FUN_140030920(&local_208,local_88);
      uVar12 = thunk_FUN_1400c9380(local_3f8);
      thunk_FUN_1400c8570(uVar12,10000,uVar9,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_3f8);
      if (0xf < local_70) {
        uVar13 = local_88[0];
        if (0xfff < local_70 + 1) {
          if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar13 = *(ulonglong *)(local_88[0] - 8);
          if (local_88[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_88[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_88[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar13);
      }
      thunk_FUN_14002d2f0(local_180);
      this = local_180;
    }
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1401d6050
// -----------------------------------------------------------------------------
/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1401d6050(longlong param_1)

{
  int *piVar1;
  longlong *plVar2;
  undefined8 *puVar3;
  longlong *plVar4;
  char cVar5;
  char cVar6;
  int iVar7;
  void *pvVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  longlong *plVar11;
  longlong *plVar12;
  basic_ostream<char,std::char_traits<char>_> *this;
  longlong *plVar13;
  longlong lVar14;
  ulonglong uVar15;
  basic_ios<char,std::char_traits<char>_> *this_00;
  longlong lVar16;
  longlong lVar17;
  longlong *plVar18;
  char *pcVar19;
  longlong *plVar20;
  uint uVar21;
  longlong *plVar22;
  undefined1 auStack_1228 [32];
  undefined4 local_1208;
  uint local_11f8;
  undefined ***local_11f0;
  QTypedArrayData<unsigned_short> *local_11e8;
  QTypedArrayData<unsigned_short> *local_11e0;
  QTypedArrayData<unsigned_short> *local_11d8;
  QTypedArrayData<unsigned_short> *local_11d0;
  QTypedArrayData<unsigned_short> *local_11c8;
  QTypedArrayData<unsigned_short> *local_11c0;
  QTypedArrayData<unsigned_short> *local_11b8;
  QTypedArrayData<unsigned_short> *local_11b0;
  QTypedArrayData<unsigned_short> *local_11a8;
  QTypedArrayData<unsigned_short> *local_11a0;
  QTypedArrayData<unsigned_short> *local_1198;
  QTypedArrayData<unsigned_short> *local_1190;
  longlong local_1188;
  char local_1180;
  longlong local_1178;
  undefined8 local_1170;
  QMutexLocker local_1160 [8];
  QMutexLocker local_1158 [8];
  undefined ***local_1150;
  undefined8 local_1148;
  void *local_1140;
  longlong *local_1138;
  longlong *local_1130;
  void *local_1120;
  undefined1 local_1118 [16];
  undefined1 local_1108 [16];
  undefined1 local_10f8 [16];
  undefined1 local_10e8 [16];
  undefined1 local_10d8 [16];
  undefined1 local_10c8 [16];
  undefined1 local_10b8 [16];
  undefined1 local_10a8 [16];
  undefined1 local_1098 [16];
  longlong local_1088 [2];
  undefined1 local_1078 [16];
  undefined1 local_1068 [16];
  undefined1 local_1058 [16];
  undefined1 local_1048 [16];
  undefined1 local_1038 [16];
  undefined1 local_1028 [16];
  undefined1 local_1018 [16];
  undefined1 local_1008 [16];
  undefined1 local_ff8 [16];
  undefined1 local_fe8 [16];
  undefined1 local_fd8 [16];
  undefined1 local_fc8 [16];
  undefined1 local_fb8 [12];
  int iStack_fac;
  undefined *local_fa8;
  undefined **local_fa0;
  basic_ostream<char,std::char_traits<char>_> local_f98 [96];
  undefined8 local_f38;
  undefined4 local_f30;
  basic_ios<char,std::char_traits<char>_> local_f20 [100];
  int iStack_ebc;
  undefined *local_eb8;
  undefined **local_eb0 [13];
  undefined8 local_e48;
  undefined4 local_e40;
  basic_ios<char,std::char_traits<char>_> local_e30 [100];
  int iStack_dcc;
  undefined *local_dc8;
  basic_streambuf<char,std::char_traits<char>_> local_dc0 [128];
  basic_ios<char,std::char_traits<char>_> local_d40 [100];
  int iStack_cdc;
  undefined *local_cd8;
  basic_streambuf<char,std::char_traits<char>_> local_cd0 [128];
  basic_ios<char,std::char_traits<char>_> local_c50 [100];
  int iStack_bec;
  undefined *local_be8;
  basic_streambuf<char,std::char_traits<char>_> local_be0 [128];
  basic_ios<char,std::char_traits<char>_> local_b60 [100];
  int iStack_afc;
  undefined *local_af8;
  basic_streambuf<char,std::char_traits<char>_> local_af0 [128];
  basic_ios<char,std::char_traits<char>_> local_a70 [100];
  int iStack_a0c;
  undefined *local_a08;
  basic_streambuf<char,std::char_traits<char>_> local_a00 [128];
  basic_ios<char,std::char_traits<char>_> local_980 [100];
  int iStack_91c;
  undefined *local_918;
  basic_streambuf<char,std::char_traits<char>_> local_910 [128];
  basic_ios<char,std::char_traits<char>_> local_890 [100];
  int iStack_82c;
  undefined *local_828;
  basic_streambuf<char,std::char_traits<char>_> local_820 [128];
  basic_ios<char,std::char_traits<char>_> local_7a0 [104];
  undefined1 local_738 [32];
  undefined1 local_718 [136];
  basic_ios<char,std::char_traits<char>_> local_690 [104];
  undefined1 local_628 [136];
  basic_ios<char,std::char_traits<char>_> local_5a0 [104];
  undefined1 local_538 [136];
  undefined1 local_4b0 [152];
  undefined1 local_418 [152];
  undefined1 local_380 [152];
  undefined1 local_2e8 [160];
  undefined8 local_248;
  longlong *plStack_240;
  undefined1 local_238 [8];
  int local_230;
  undefined1 local_228 [32];
  undefined1 local_208 [32];
  undefined1 local_1e8 [32];
  undefined1 local_1c8 [32];
  int local_1a8;
  ulonglong local_198 [3];
  ulonglong local_180;
  undefined8 local_178 [3];
  ulonglong local_160;
  undefined8 local_158 [3];
  ulonglong local_140;
  ulonglong local_138 [3];
  ulonglong local_120;
  undefined1 local_118 [32];
  undefined1 local_f8 [32];
  undefined1 local_d8 [32];
  undefined1 local_b8 [32];
  undefined1 local_98 [32];
  undefined1 local_78 [32];
  undefined1 local_58 [32];
  ulonglong local_38;
  undefined8 local_30;
  
  local_30 = 0x1401d606c;
  local_1148 = 0xfffffffffffffffe;
  local_38 = DAT_14036a720 ^ (ulonglong)auStack_1228;
  uVar21 = 0;
  local_11f8 = 0;
  local_1188 = param_1 + 0x1f0;
  iVar7 = _Mtx_lock();
  if (iVar7 != 0) {
    std::_Throw_C_error(iVar7);
  }
  local_1180 = '\x01';
  cVar5 = *(char *)(param_1 + 0x1e8);
  do {
    if (cVar5 != '\0') {
      uVar9 = thunk_FUN_1401b54b0();
      cVar5 = thunk_FUN_1401b3920(uVar9);
      if (cVar5 != '\0') {
        uVar9 = thunk_FUN_1400c9380(local_fd8);
        cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
        thunk_FUN_1400c7da0(local_fd8);
        if (cVar5 != '\0') {
          thunk_FUN_14002cec0(local_718,2,1);
          thunk_FUN_14002cab0(local_718,"AudioEnumeratorImpl::update: stopped updated thread");
          uVar9 = thunk_FUN_140030920(local_718,local_78);
          uVar10 = thunk_FUN_1400c9380(local_fc8);
          local_1208 = 0x2a8;
          thunk_FUN_1400c8570(uVar10,10000,uVar9,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_fc8);
          thunk_FUN_14002d390(local_78);
          thunk_FUN_14002d2f0(local_690);
          this_00 = local_690;
LAB_1401d79c0:
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (this_00);
        }
      }
LAB_1401d79c7:
      if ((local_1180 != '\0') && (iVar7 = _Mtx_unlock(local_1188), iVar7 != 0)) {
        std::_Throw_C_error(iVar7);
      }
      return;
    }
    thunk_FUN_1401bea00(param_1 + 0x240);
    pvVar8 = DAT_140376db8;
    if (DAT_140376db8 == (void *)0x0) {
      QMutexLocker::QMutexLocker(local_1160,(QBasicMutex *)&DAT_140376dc8);
      if (DAT_140376db8 == (void *)0x0) {
        pvVar8 = operator_new(8);
        local_1140 = pvVar8;
        if (pvVar8 == (void *)0x0) {
          DAT_140376db8 = (void *)0x0;
        }
        else {
          lVar14 = -1;
          do {
            lVar16 = lVar14 + 1;
            pcVar19 = &DAT_1402d004d + lVar14;
            lVar14 = lVar16;
          } while (*pcVar19 != '\0');
          local_11e8 = QString::fromAscii_helper("audio",(int)lVar16);
          thunk_FUN_14016a3f0(pvVar8);
          QString::~QString((QString *)&local_11e8);
          DAT_140376db8 = pvVar8;
        }
      }
      pvVar8 = DAT_140376db8;
      QMutexLocker::~QMutexLocker(local_1160);
    }
    lVar14 = -1;
    do {
      lVar17 = lVar14 + 1;
      lVar16 = lVar14 + 1;
      lVar14 = lVar17;
    } while ("agentAudio"[lVar16] != '\0');
    local_11e0 = QString::fromAscii_helper("agentAudio",(int)lVar17);
    cVar5 = thunk_FUN_14016a4c0(pvVar8,&local_11e0);
    QString::~QString((QString *)&local_11e0);
    if (cVar5 != '\0') {
      uVar9 = thunk_FUN_1400c9380(local_1118);
      cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
      thunk_FUN_1400c7da0(local_1118);
      if (cVar5 != '\0') {
        local_fa8 = &DAT_1402738f0;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f20);
        uVar21 = uVar21 | 1;
        local_11f8 = uVar21;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_fa8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_fa0,false);
        *(undefined ***)((longlong)&local_fa8 + (longlong)*(int *)(local_fa8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_fac + (longlong)*(int *)(local_fa8 + 4)) =
             *(int *)(local_fa8 + 4) + -0x88;
        local_1150 = &local_fa0;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_fa0);
        local_fa0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_f38 = 0;
        local_f30 = 4;
        thunk_FUN_14002cab0(&local_fa8,"AudioEnumeratorImpl::update: woke up");
        uVar9 = thunk_FUN_140030920(&local_fa8,local_198);
        uVar10 = thunk_FUN_1400c9380(local_1108);
        local_1208 = 0x259;
        thunk_FUN_1400c8570(uVar10,10000,uVar9,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                           );
        thunk_FUN_1400c7da0(local_1108);
        if (0xf < local_180) {
          uVar15 = local_198[0];
          if (0xfff < local_180 + 1) {
            if ((local_198[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar15 = *(ulonglong *)(local_198[0] - 8);
            if (local_198[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_198[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_198[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d06(uVar15);
        }
        *(undefined ***)((longlong)&local_fa8 + (longlong)*(int *)(local_fa8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_fac + (longlong)*(int *)(local_fa8 + 4)) =
             *(int *)(local_fa8 + 4) + -0x88;
        local_fa0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14002e520(&local_fa0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_fa0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_f98);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f20);
      }
    }
    if (*(char *)(param_1 + 0x1e8) != '\0') {
      uVar9 = thunk_FUN_1401b54b0();
      local_1190 = QString::fromAscii_helper("agentAudio",10);
      cVar5 = thunk_FUN_14016a4c0(uVar9,&local_1190);
      QString::~QString((QString *)&local_1190);
      if (cVar5 == '\0') goto LAB_1401d79c7;
      uVar9 = thunk_FUN_1400c9380(local_ff8);
      cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
      thunk_FUN_1400c7da0(local_ff8);
      if (cVar5 == '\0') goto LAB_1401d79c7;
      local_828 = &DAT_1402738f0;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_7a0);
      local_11f8 = uVar21 | 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_828,local_820,false);
      *(undefined ***)(local_820 + (longlong)*(int *)(local_828 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_82c + (longlong)*(int *)(local_828 + 4)) =
           *(int *)(local_828 + 4) + -0x88;
      thunk_FUN_14002d040(local_820,2);
      thunk_FUN_14002cab0(&local_828,"AudioEnumeratorImpl::update: stopping updated thread");
      uVar9 = thunk_FUN_140030920(&local_828,local_138);
      uVar10 = thunk_FUN_1400c9380(local_fe8);
      local_1208 = 0x25b;
      thunk_FUN_1400c8570(uVar10,10000,uVar9,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                         );
      thunk_FUN_1400c7da0(local_fe8);
      if (0xf < local_120) {
        uVar15 = local_138[0];
        if (0xfff < local_120 + 1) {
          if ((local_138[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_138[0] - 8);
          if (local_138[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_138[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_138[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d06(uVar15);
      }
      thunk_FUN_14002d2f0(local_7a0);
      this_00 = local_7a0;
      goto LAB_1401d79c0;
    }
    lVar14 = *(longlong *)(param_1 + 0x2a0);
    while (lVar14 != 0) {
      plVar22 = (longlong *)(param_1 + 0x298);
      local_1150 = *(undefined ****)*plVar22;
      iVar7 = *(int *)((longlong)local_1150 + 0x1c);
      plVar13 = (longlong *)*plVar22;
      plVar2 = (longlong *)plVar13[1];
      cVar5 = *(char *)((longlong)plVar2 + 0x19);
      plVar18 = plVar13;
      plVar20 = plVar13;
      plVar12 = plVar2;
      while (cVar5 == '\0') {
        if (*(int *)((longlong)plVar12 + 0x1c) < iVar7) {
          plVar11 = (longlong *)plVar12[2];
          plVar12 = plVar18;
        }
        else {
          if ((*(char *)((longlong)plVar20 + 0x19) != '\0') &&
             (iVar7 < *(int *)((longlong)plVar12 + 0x1c))) {
            plVar20 = plVar12;
          }
          plVar11 = (longlong *)*plVar12;
        }
        plVar18 = plVar12;
        plVar12 = plVar11;
        cVar5 = *(char *)((longlong)plVar11 + 0x19);
      }
      plVar12 = plVar20;
      if (*(char *)((longlong)plVar20 + 0x19) != '\0') {
        plVar12 = plVar13 + 1;
      }
      cVar5 = *(char *)(*plVar12 + 0x19);
      plVar12 = (longlong *)*plVar12;
      while (plVar11 = plVar18, cVar5 == '\0') {
        if (iVar7 < *(int *)((longlong)plVar12 + 0x1c)) {
          plVar11 = (longlong *)*plVar12;
          plVar20 = plVar12;
        }
        else {
          plVar11 = (longlong *)plVar12[2];
        }
        plVar12 = plVar11;
        cVar5 = *(char *)((longlong)plVar11 + 0x19);
      }
      while (plVar11 != plVar20) {
        if (*(char *)((longlong)plVar11 + 0x19) == '\0') {
          plVar12 = (longlong *)plVar11[2];
          if (*(char *)((longlong)plVar12 + 0x19) == '\0') {
            cVar5 = *(char *)(*plVar12 + 0x19);
            plVar11 = plVar12;
            plVar12 = (longlong *)*plVar12;
            while (cVar5 == '\0') {
              cVar5 = *(char *)(*plVar12 + 0x19);
              plVar11 = plVar12;
              plVar12 = (longlong *)*plVar12;
            }
          }
          else {
            cVar5 = *(char *)(plVar11[1] + 0x19);
            plVar4 = (longlong *)plVar11[1];
            plVar12 = plVar11;
            while ((plVar11 = plVar4, cVar5 == '\0' && (plVar12 == (longlong *)plVar11[2]))) {
              cVar5 = *(char *)(plVar11[1] + 0x19);
              plVar4 = (longlong *)plVar11[1];
              plVar12 = plVar11;
            }
          }
        }
      }
      local_1138 = (longlong *)*plVar13;
      if ((plVar18 == local_1138) && (local_1130 = plVar13, plVar20 == plVar13)) {
        cVar5 = *(char *)((longlong)plVar2 + 0x19);
        while (cVar5 == '\0') {
          thunk_FUN_1401c5a30(plVar22);
          plVar13 = (longlong *)*plVar2;
          FUN_140002d06(plVar2);
          plVar2 = plVar13;
          cVar5 = *(char *)((longlong)plVar13 + 0x19);
        }
        *(longlong *)(*plVar22 + 8) = *plVar22;
        *(longlong *)*plVar22 = *plVar22;
        *(longlong *)(*plVar22 + 0x10) = *plVar22;
        *(undefined8 *)(param_1 + 0x2a0) = 0;
      }
      else {
        while (plVar18 != plVar20) {
          if (*(char *)((longlong)plVar18 + 0x19) == '\0') {
            plVar13 = (longlong *)plVar18[2];
            if (*(char *)((longlong)plVar13 + 0x19) == '\0') {
              cVar5 = *(char *)(*plVar13 + 0x19);
              plVar18 = plVar13;
              plVar13 = (longlong *)*plVar13;
              while (cVar5 == '\0') {
                cVar5 = *(char *)(*plVar13 + 0x19);
                plVar18 = plVar13;
                plVar13 = (longlong *)*plVar13;
              }
            }
            else {
              cVar5 = *(char *)(plVar18[1] + 0x19);
              plVar2 = (longlong *)plVar18[1];
              plVar13 = plVar18;
              while ((plVar18 = plVar2, cVar5 == '\0' && (plVar13 == (longlong *)plVar18[2]))) {
                cVar5 = *(char *)(plVar18[1] + 0x19);
                plVar2 = (longlong *)plVar18[1];
                plVar13 = plVar18;
              }
            }
          }
          thunk_FUN_1401d1280(plVar22);
        }
      }
      pvVar8 = DAT_140376db8;
      if (DAT_140376db8 == (void *)0x0) {
        QMutexLocker::QMutexLocker(local_1158,(QBasicMutex *)&DAT_140376dc8);
        if (DAT_140376db8 == (void *)0x0) {
          pvVar8 = operator_new(8);
          local_1120 = pvVar8;
          if (pvVar8 == (void *)0x0) {
            DAT_140376db8 = (void *)0x0;
          }
          else {
            lVar14 = -1;
            do {
              lVar16 = lVar14 + 1;
              pcVar19 = &DAT_1402d004d + lVar14;
              lVar14 = lVar16;
            } while (*pcVar19 != '\0');
            local_11d8 = QString::fromAscii_helper("audio",(int)lVar16);
            thunk_FUN_14016a3f0(pvVar8);
            QString::~QString((QString *)&local_11d8);
            DAT_140376db8 = pvVar8;
          }
        }
        pvVar8 = DAT_140376db8;
        QMutexLocker::~QMutexLocker(local_1158);
      }
      lVar14 = -1;
      do {
        lVar17 = lVar14 + 1;
        lVar16 = lVar14 + 1;
        lVar14 = lVar17;
      } while ("agentAudio"[lVar16] != '\0');
      local_11d0 = QString::fromAscii_helper("agentAudio",(int)lVar17);
      cVar5 = thunk_FUN_14016a4c0(pvVar8);
      QString::~QString((QString *)&local_11d0);
      if (cVar5 != '\0') {
        uVar9 = thunk_FUN_1400c9380(local_10f8);
        cVar5 = thunk_FUN_1400c8f40(uVar9);
        thunk_FUN_1400c7da0(local_10f8);
        if (cVar5 != '\0') {
          local_eb8 = &DAT_1402738f0;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_e30);
          uVar21 = uVar21 | 4;
          local_11f8 = uVar21;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_eb8,
                     (basic_streambuf<char,std::char_traits<char>_> *)local_eb0,false);
          *(undefined ***)((longlong)local_eb0 + (longlong)*(int *)(local_eb8 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_ebc + (longlong)*(int *)(local_eb8 + 4)) =
               *(int *)(local_eb8 + 4) + -0x88;
          local_11f0 = local_eb0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)local_eb0);
          local_eb0[0] = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                         vftable;
          local_e48 = 0;
          local_e40 = 4;
          this = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14002cab0(&local_eb8,"AudioEnumeratorImpl::update: processing task ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,iVar7);
          uVar9 = thunk_FUN_140030920(&local_eb8,local_178);
          uVar10 = thunk_FUN_1400c9380(local_10e8);
          local_1208 = 0x264;
          thunk_FUN_1400c8570(uVar10,10000,uVar9,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                             );
          thunk_FUN_1400c7da0(local_10e8);
          if (0xf < local_160) {
            thunk_FUN_14002dff0(local_178[0]);
          }
          thunk_FUN_14002d2f0(local_e30);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_e30);
        }
      }
      if (iVar7 == 0) {
        uVar9 = thunk_FUN_1401b54b0();
        lVar14 = -1;
        do {
          lVar17 = lVar14 + 1;
          lVar16 = lVar14 + 1;
          lVar14 = lVar17;
        } while ("agentAudio"[lVar16] != '\0');
        local_11c8 = QString::fromAscii_helper("agentAudio",(int)lVar17);
        cVar5 = thunk_FUN_14016a4c0(uVar9,&local_11c8);
        QString::~QString((QString *)&local_11c8);
        if (cVar5 != '\0') {
          uVar9 = thunk_FUN_1400c9380(local_10d8);
          cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
          thunk_FUN_1400c7da0(local_10d8);
          if (cVar5 != '\0') {
            local_dc8 = &DAT_1402738f0;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_d40);
            uVar21 = uVar21 | 8;
            local_11f8 = uVar21;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_dc8,local_dc0,false);
            *(undefined ***)(local_dc0 + (longlong)*(int *)(local_dc8 + 4) + -8) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_dcc + (longlong)*(int *)(local_dc8 + 4)) =
                 *(int *)(local_dc8 + 4) + -0x88;
            thunk_FUN_14002d040(local_dc0,2);
            thunk_FUN_14002cab0(&local_dc8,"AudioEnumeratorImpl::update: updating devices");
            uVar9 = thunk_FUN_140030920(&local_dc8,local_158);
            uVar10 = thunk_FUN_1400c9380(local_1048);
            local_1208 = 0x266;
            thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                               );
            thunk_FUN_1400c7da0(local_1048);
            if (0xf < local_140) {
              thunk_FUN_14002dff0(local_158[0],local_140 + 1);
            }
            thunk_FUN_14002d2f0(local_d40);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_d40);
          }
        }
        thunk_FUN_1401ec580(param_1 + 0x118);
        thunk_FUN_1401d4180();
        thunk_FUN_1401fea90();
        thunk_FUN_1401fe180();
        if (*(char *)(param_1 + 0x110) != '\0') {
          uVar9 = thunk_FUN_1401d2420(param_1,local_538);
          uVar9 = thunk_FUN_1401e7be0(&local_1178,uVar9);
          thunk_FUN_1401c27a0(param_1 + 0x118,uVar9);
          if (local_1178 != 0) {
            thunk_FUN_1401b8b70(local_1178,local_1170,&local_1178);
            thunk_FUN_1401cdc00(&local_1178);
          }
        }
      }
      else if (iVar7 == 1) {
        uVar9 = thunk_FUN_1401b54b0();
        lVar14 = -1;
        do {
          lVar17 = lVar14 + 1;
          lVar16 = lVar14 + 1;
          lVar14 = lVar17;
        } while ("agentAudio"[lVar16] != '\0');
        local_11c0 = QString::fromAscii_helper("agentAudio",(int)lVar17);
        cVar5 = thunk_FUN_14016a4c0(uVar9,&local_11c0);
        QString::~QString((QString *)&local_11c0);
        if (cVar5 != '\0') {
          uVar9 = thunk_FUN_1400c9380(local_10c8);
          cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
          thunk_FUN_1400c7da0(local_10c8);
          if (cVar5 != '\0') {
            local_cd8 = &DAT_1402738f0;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_c50);
            uVar21 = uVar21 | 0x10;
            local_11f8 = uVar21;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_cd8,local_cd0,false);
            *(undefined ***)(local_cd0 + (longlong)*(int *)(local_cd8 + 4) + -8) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_cdc + (longlong)*(int *)(local_cd8 + 4)) =
                 *(int *)(local_cd8 + 4) + -0x88;
            thunk_FUN_14002d040(local_cd0,2);
            thunk_FUN_14002cab0(&local_cd8,"AudioEnumeratorImpl::update: updating sessions");
            uVar9 = thunk_FUN_140030920(&local_cd8,local_118);
            uVar10 = thunk_FUN_1400c9380(local_10b8);
            local_1208 = 0x26d;
            thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                               );
            thunk_FUN_1400c7da0(local_10b8);
            thunk_FUN_14002d390(local_118);
            thunk_FUN_14002d2f0(local_c50);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_c50);
          }
        }
        if (*(longlong *)(param_1 + 0x2c8) != 0) {
          do {
            plVar13 = (longlong *)thunk_FUN_1401caaf0(param_1 + 0x2a8,local_738);
            lVar14 = 0;
            if ((longlong *)*plVar13 != (longlong *)0x0) {
              lVar14 = *(longlong *)*plVar13;
            }
            puVar3 = *(undefined8 **)
                      (*(longlong *)(lVar14 + 8) +
                      (*(longlong *)(lVar14 + 0x10) - 1U & plVar13[2]) * 8);
            local_248 = 0;
            plStack_240 = (longlong *)0x0;
            lVar14 = puVar3[1];
            uVar9 = *puVar3;
            if (lVar14 != 0) {
              LOCK();
              piVar1 = (int *)(lVar14 + 8);
              *piVar1 = *piVar1 + 1;
              UNLOCK();
              uVar21 = local_11f8;
            }
            thunk_FUN_1401c85a0(&local_248,uVar9);
            thunk_FUN_1401c0150(local_238);
            thunk_FUN_1401c14d0(*(undefined8 *)
                                 (*(longlong *)(param_1 + 0x2b0) +
                                 (*(longlong *)(param_1 + 0x2b8) - 1U &
                                 *(ulonglong *)(param_1 + 0x2c0)) * 8));
            plVar13 = (longlong *)(param_1 + 0x2c8);
            *plVar13 = *plVar13 + -1;
            if (*plVar13 == 0) {
              *(undefined8 *)(param_1 + 0x2c0) = 0;
            }
            else {
              *(longlong *)(param_1 + 0x2c0) = *(longlong *)(param_1 + 0x2c0) + 1;
            }
            uVar9 = thunk_FUN_1401b54b0();
            lVar14 = -1;
            do {
              lVar17 = lVar14 + 1;
              lVar16 = lVar14 + 1;
              lVar14 = lVar17;
            } while ("agentAudio"[lVar16] != '\0');
            local_11b8 = QString::fromAscii_helper("agentAudio",(int)lVar17);
            cVar5 = thunk_FUN_14016a4c0(uVar9,&local_11b8);
            QString::~QString((QString *)&local_11b8);
            if (cVar5 != '\0') {
              uVar9 = thunk_FUN_1400c9380(local_10a8);
              cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
              thunk_FUN_1400c7da0(local_10a8);
              if (cVar5 != '\0') {
                local_be8 = &DAT_1402738f0;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_b60);
                uVar21 = uVar21 | 0x20;
                local_11f8 = uVar21;
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_be8,local_be0,false
                          );
                *(undefined ***)(local_be0 + (longlong)*(int *)(local_be8 + 4) + -8) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_bec + (longlong)*(int *)(local_be8 + 4)) =
                     *(int *)(local_be8 + 4) + -0x88;
                thunk_FUN_14002d040(local_be0,2);
                pcVar19 = "ending";
                if (local_1a8 == 1) {
                  pcVar19 = "beginning";
                }
                uVar9 = thunk_FUN_14002cab0(&local_be8,"AudioEnumeratorImpl::update: ");
                uVar9 = thunk_FUN_14002cab0(uVar9,pcVar19);
                uVar9 = thunk_FUN_14002cab0(uVar9," recording from device \'");
                uVar9 = thunk_FUN_1400d3c50(uVar9,local_1c8);
                uVar9 = thunk_FUN_14002cab0(uVar9,"\' for process \'");
                uVar9 = thunk_FUN_1400d3c50(uVar9,local_228);
                thunk_FUN_14002cab0(uVar9,&DAT_1402bd320);
                uVar9 = thunk_FUN_140030920(&local_be8,local_f8);
                uVar10 = thunk_FUN_1400c9380(local_1098);
                local_1208 = 0x275;
                thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                                   );
                thunk_FUN_1400c7da0(local_1098);
                thunk_FUN_14002d390(local_f8);
                thunk_FUN_14002d2f0(local_b60);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_b60);
              }
            }
            if (*(char *)(param_1 + 0x2d0) == '\0') {
              if (local_1a8 == 1) {
                thunk_FUN_1401b22c0(local_380,local_238);
                thunk_FUN_1401cab50(param_1,local_248);
              }
              else if (local_1a8 == 0) {
                thunk_FUN_1401b22c0(local_2e8,local_238);
                thunk_FUN_1401cf150(param_1,local_248);
              }
            }
            else if (local_1a8 == 1) {
              thunk_FUN_1401bbd10(param_1 + 0x2d8,local_1088);
              thunk_FUN_1401c2850(local_1088[0] + 0x30);
              if (*(char *)(param_1 + 0x2d1) == '\0') {
                cVar5 = thunk_FUN_1401d25d0(param_1);
                if (cVar5 == '\0') {
                  uVar9 = thunk_FUN_1401b54b0();
                  lVar14 = -1;
                  do {
                    lVar17 = lVar14 + 1;
                    lVar16 = lVar14 + 1;
                    lVar14 = lVar17;
                  } while ("agentAudio"[lVar16] != '\0');
                  local_11a8 = QString::fromAscii_helper("agentAudio",(int)lVar17);
                  cVar5 = thunk_FUN_14016a4c0(uVar9,&local_11a8);
                  QString::~QString((QString *)&local_11a8);
                  if (cVar5 != '\0') {
                    uVar9 = thunk_FUN_1400c9380(local_1058);
                    cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
                    thunk_FUN_1400c7da0(local_1058);
                    if (cVar5 != '\0') {
                      local_a08 = &DAT_1402738f0;
                      std::basic_ios<char,std::char_traits<char>_>::
                      basic_ios<char,std::char_traits<char>_>(local_980);
                      uVar21 = uVar21 | 0x80;
                      local_11f8 = uVar21;
                      std::basic_ostream<char,std::char_traits<char>_>::
                      basic_ostream<char,std::char_traits<char>_>
                                ((basic_ostream<char,std::char_traits<char>_> *)&local_a08,local_a00
                                 ,false);
                      *(undefined ***)(local_a00 + (longlong)*(int *)(local_a08 + 4) + -8) =
                           std::
                           basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                           vftable;
                      *(int *)((longlong)&iStack_a0c + (longlong)*(int *)(local_a08 + 4)) =
                           *(int *)(local_a08 + 4) + -0x88;
                      thunk_FUN_14002d040(local_a00,2);
                      pcVar19 = "render";
                      if (local_230 == 0) {
                        pcVar19 = "capture";
                      }
                      uVar9 = thunk_FUN_14002cab0(&local_a08,
                                                  "AudioEnumeratorImpl::update: tracking session without recording while gate is closed, process=\'"
                                                 );
                      uVar9 = thunk_FUN_1400d3c50(uVar9,local_228);
                      uVar9 = thunk_FUN_14002cab0(uVar9,"\', device=\'");
                      uVar9 = thunk_FUN_1400d3c50(uVar9,local_1c8);
                      uVar9 = thunk_FUN_14002cab0(uVar9,"\', direction=");
                      thunk_FUN_14002cab0(uVar9,pcVar19);
                      uVar9 = thunk_FUN_140030920(&local_a08,local_b8);
                      uVar10 = thunk_FUN_1400c9380(local_fb8);
                      local_1208 = 0x286;
                      thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                                         );
                      thunk_FUN_1400c7da0(local_fb8);
                      thunk_FUN_14002d390(local_b8);
                      thunk_FUN_14002d2f0(local_980);
                      std::basic_ios<char,std::char_traits<char>_>::
                      ~basic_ios<char,std::char_traits<char>_>(local_980);
                    }
                  }
                }
                else {
                  *(undefined1 *)(param_1 + 0x2d1) = 1;
                  uVar9 = thunk_FUN_1401b54b0();
                  lVar14 = -1;
                  do {
                    lVar17 = lVar14 + 1;
                    lVar16 = lVar14 + 1;
                    lVar14 = lVar17;
                  } while ("agentAudio"[lVar16] != '\0');
                  local_11b0 = QString::fromAscii_helper("agentAudio",(int)lVar17);
                  cVar5 = thunk_FUN_14016a4c0(uVar9,&local_11b0);
                  QString::~QString((QString *)&local_11b0);
                  if (cVar5 != '\0') {
                    uVar9 = thunk_FUN_1400c9380(local_1078);
                    cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
                    thunk_FUN_1400c7da0(local_1078);
                    if (cVar5 != '\0') {
                      local_af8 = &DAT_1402738f0;
                      std::basic_ios<char,std::char_traits<char>_>::
                      basic_ios<char,std::char_traits<char>_>(local_a70);
                      uVar21 = uVar21 | 0x40;
                      local_11f8 = uVar21;
                      std::basic_ostream<char,std::char_traits<char>_>::
                      basic_ostream<char,std::char_traits<char>_>
                                ((basic_ostream<char,std::char_traits<char>_> *)&local_af8,local_af0
                                 ,false);
                      *(undefined ***)(local_af0 + (longlong)*(int *)(local_af8 + 4) + -8) =
                           std::
                           basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                           vftable;
                      *(int *)((longlong)&iStack_afc + (longlong)*(int *)(local_af8 + 4)) =
                           *(int *)(local_af8 + 4) + -0x88;
                      thunk_FUN_14002d040(local_af0,2);
                      thunk_FUN_14002cab0(&local_af8,
                                          "AudioEnumeratorImpl::update: recording gate is opened (capture session detected)"
                                         );
                      uVar9 = thunk_FUN_140030920(&local_af8,local_d8);
                      uVar10 = thunk_FUN_1400c9380(local_1068);
                      local_1208 = 0x27f;
                      thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                                         );
                      thunk_FUN_1400c7da0(local_1068);
                      thunk_FUN_14002d390(local_d8);
                      thunk_FUN_14002d2f0(local_a70);
                      std::basic_ios<char,std::char_traits<char>_>::
                      ~basic_ios<char,std::char_traits<char>_>(local_a70);
                    }
                  }
                  thunk_FUN_1401d4ed0(param_1);
                }
              }
              else {
                thunk_FUN_1401c0150(local_4b0,local_238);
                thunk_FUN_1401cab50(param_1,local_248);
              }
            }
            else if (local_1a8 == 0) {
              if (*(char *)(param_1 + 0x2d1) != '\0') {
                thunk_FUN_1401c0150(local_418,local_238);
                thunk_FUN_1401cf150(param_1,local_248);
              }
              thunk_FUN_1401d1070(param_1 + 0x2d8);
              cVar5 = *(char *)(param_1 + 0x2d1);
              if (cVar5 == '\0') {
LAB_1401d7417:
                uVar9 = thunk_FUN_1401b54b0();
                lVar14 = -1;
                do {
                  lVar17 = lVar14 + 1;
                  lVar16 = lVar14 + 1;
                  lVar14 = lVar17;
                } while ("agentAudio"[lVar16] != '\0');
                local_1198 = QString::fromAscii_helper("agentAudio",(int)lVar17);
                cVar5 = thunk_FUN_14016a4c0(uVar9,&local_1198);
                QString::~QString((QString *)&local_1198);
                if (cVar5 != '\0') {
                  uVar9 = thunk_FUN_1400c9380(local_1018);
                  cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
                  thunk_FUN_1400c7da0(local_1018);
                  if (cVar5 != '\0') {
                    local_918 = &DAT_1402738f0;
                    std::basic_ios<char,std::char_traits<char>_>::
                    basic_ios<char,std::char_traits<char>_>(local_890);
                    uVar21 = uVar21 | 0x100;
                    local_11f8 = uVar21;
                    std::basic_ostream<char,std::char_traits<char>_>::
                    basic_ostream<char,std::char_traits<char>_>
                              ((basic_ostream<char,std::char_traits<char>_> *)&local_918,local_910,
                               false);
                    *(undefined ***)(local_910 + (longlong)*(int *)(local_918 + 4) + -8) =
                         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>
                         ::vftable;
                    *(int *)((longlong)&iStack_91c + (longlong)*(int *)(local_918 + 4)) =
                         *(int *)(local_918 + 4) + -0x88;
                    thunk_FUN_14002d040(local_910,2);
                    pcVar19 = "render";
                    if (local_230 == 0) {
                      pcVar19 = "capture";
                    }
                    uVar9 = thunk_FUN_14002cab0(&local_918,
                                                "AudioEnumeratorImpl::update: removed tracked session while gate is already closed, process=\'"
                                               );
                    uVar9 = thunk_FUN_1400d3c50(uVar9,local_228);
                    uVar9 = thunk_FUN_14002cab0(uVar9,"\', device=\'");
                    uVar9 = thunk_FUN_1400d3c50(uVar9,local_1c8);
                    uVar9 = thunk_FUN_14002cab0(uVar9,"\', direction=");
                    thunk_FUN_14002cab0(uVar9,pcVar19);
                    uVar9 = thunk_FUN_140030920(&local_918,local_58);
                    uVar10 = thunk_FUN_1400c9380(local_1008);
                    local_1208 = 0x298;
                    thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                                       );
                    thunk_FUN_1400c7da0(local_1008);
                    thunk_FUN_14002d390(local_58);
                    thunk_FUN_14002d2f0(local_890);
                    std::basic_ios<char,std::char_traits<char>_>::
                    ~basic_ios<char,std::char_traits<char>_>(local_890);
                  }
                }
              }
              else {
                cVar6 = thunk_FUN_1401d25d0(param_1);
                if (cVar6 == '\0') {
                  uVar9 = thunk_FUN_1401b54b0();
                  lVar14 = -1;
                  do {
                    lVar17 = lVar14 + 1;
                    lVar16 = lVar14 + 1;
                    lVar14 = lVar17;
                  } while ("agentAudio"[lVar16] != '\0');
                  local_11a0 = QString::fromAscii_helper("agentAudio",(int)lVar17);
                  cVar5 = thunk_FUN_14016a4c0(uVar9,&local_11a0);
                  QString::~QString((QString *)&local_11a0);
                  if (cVar5 != '\0') {
                    uVar9 = thunk_FUN_1400c9380(local_1038);
                    cVar5 = thunk_FUN_1400c8f40(uVar9,10000);
                    thunk_FUN_1400c7da0(local_1038);
                    if (cVar5 != '\0') {
                      thunk_FUN_14002cec0(local_628,2,1);
                      thunk_FUN_14002cab0(local_628,
                                          "AudioEnumeratorImpl::update: recording gate is closed (no capture sessions)"
                                         );
                      uVar9 = thunk_FUN_140030920(local_628,local_98);
                      uVar10 = thunk_FUN_1400c9380(local_1028);
                      local_1208 = 0x292;
                      thunk_FUN_1400c8570(uVar10,10000,uVar9,
                                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-audioEnumerator\\audioEnumeratorImpl_win.cpp"
                                         );
                      thunk_FUN_1400c7da0(local_1028);
                      thunk_FUN_14002d390(local_98);
                      thunk_FUN_14002d2f0(local_5a0);
                      std::basic_ios<char,std::char_traits<char>_>::
                      ~basic_ios<char,std::char_traits<char>_>(local_5a0);
                    }
                  }
                  thunk_FUN_1401d4fc0(param_1);
                  *(undefined1 *)(param_1 + 0x2d1) = 0;
                }
                else if (cVar5 == '\0') goto LAB_1401d7417;
              }
            }
            thunk_FUN_14002d390(local_1c8);
            thunk_FUN_14002d390(local_1e8);
            thunk_FUN_14002d390(local_208);
            thunk_FUN_14002d390(local_228);
            plVar13 = plStack_240;
            if (plStack_240 != (longlong *)0x0) {
              LOCK();
              plVar2 = plStack_240 + 1;
              lVar14 = *plVar2;
              *(int *)plVar2 = (int)*plVar2 + -1;
              UNLOCK();
              uVar21 = local_11f8;
              if ((int)lVar14 == 1) {
                (**(code **)*plStack_240)(plStack_240);
                LOCK();
                piVar1 = (int *)((longlong)plVar13 + 0xc);
                iVar7 = *piVar1;
                *piVar1 = *piVar1 + -1;
                UNLOCK();
                uVar21 = local_11f8;
                if (iVar7 == 1) {
                  (**(code **)(*plVar13 + 8))(plVar13);
                  uVar21 = local_11f8;
                }
              }
            }
          } while (*(longlong *)(param_1 + 0x2c8) != 0);
        }
      }
      lVar14 = *(longlong *)(param_1 + 0x2a0);
    }
    cVar5 = *(char *)(param_1 + 0x1e8);
  } while( true );
}

