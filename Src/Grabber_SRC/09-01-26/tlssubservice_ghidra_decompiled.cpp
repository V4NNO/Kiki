/*
 * tlssubservice.exe / grabberSubAgent - pseudocod decompilat cu Ghidra MCP
 * Data analizei: 2026-09-01
 *
 * IMPORTANT:
 * - Acesta nu este codul-sursa original si nu este garantat compilabil.
 * - Tipurile undefined*, numele FUN_/DAT_/LAB_ si prototipurile sunt cele
 *   produse de decompilatorul Ghidra.
 * - Sunt incluse numai functii existente in programul analizat; nu au fost
 *   inventate functii si nu au fost rescrise implementari.
 * - Selectia acopera pornirea, modul proxy, IPC/local RPC, informatiile
 *   utilizatorului, clipboard, HTTP si componentele UI/self-control.
 */

// =============================================================================
// Ghidra: entry @ 0x140002f3b
// =============================================================================
void entry(void)

{
  __security_init_cookie();
  FUN_1400b8390();
  return;
}

// =============================================================================
// Ghidra: FUN_1400b8390 @ 0x1400b8390
// =============================================================================
int FUN_1400b8390(void)

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
  if (DAT_140767220 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_140767220 == 0) {
    DAT_140767220 = 1;
    iVar6 = _initterm_e(&DAT_1400dace0,&DAT_1400db120);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_1400da000,&DAT_1400dabd0);
    DAT_140767220 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_1400b9914();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_1400b9920();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1400b9cc0(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
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
// Ghidra: FUN_140017280 @ 0x140017280
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_140017280(int param_1,char **param_2)

{
  int iVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  uint uVar5;
  DWORD dwProcessId;
  uint uVar6;
  int iVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  QHashData *pQVar10;
  QHashData *pQVar11;
  basic_ostream<char,std::char_traits<char>_> *this;
  ulonglong uVar12;
  QString *pQVar13;
  QHashData *pQVar14;
  undefined1 auStackY_6b8 [32];
  QHashData *local_680;
  QString local_678 [8];
  longlong local_670;
  DWORD local_668 [2];
  QString local_660 [8];
  int local_658 [2];
  QString local_650 [8];
  QChar local_648 [8];
  QTypedArrayData<unsigned_short> *local_640;
  QTypedArrayData<unsigned_short> *local_638;
  QTypedArrayData<unsigned_short> *local_630;
  QTypedArrayData<unsigned_short> *local_628;
  Data *local_620;
  QTypedArrayData<unsigned_short> *local_618;
  QTypedArrayData<unsigned_short> *local_610;
  QTypedArrayData<unsigned_short> *local_608;
  QTypedArrayData<unsigned_short> *local_600;
  QTypedArrayData<unsigned_short> *local_5f8;
  QTypedArrayData<unsigned_short> *local_5f0;
  char *local_5e8;
  QApplication local_5e0 [16];
  QString local_5d0 [8];
  QString local_5c8 [8];
  QString local_5c0 [8];
  QString local_5b8 [8];
  QString local_5b0 [8];
  char local_5a8 [8];
  QString local_5a0 [24];
  undefined8 local_588;
  undefined1 local_580 [16];
  undefined1 local_570 [16];
  undefined1 local_560 [16];
  undefined1 local_550 [324];
  int iStack_40c;
  undefined *local_408;
  basic_streambuf<char,std::char_traits<char>_> local_400 [128];
  basic_ios<char,std::char_traits<char>_> local_380 [100];
  int iStack_31c;
  undefined *local_318;
  basic_streambuf<char,std::char_traits<char>_> local_310 [128];
  basic_ios<char,std::char_traits<char>_> local_290 [344];
  undefined1 local_138 [128];
  ulonglong local_b8 [3];
  ulonglong local_a0;
  ulonglong local_98 [3];
  ulonglong local_80;
  ulonglong local_38;
  
  local_588 = 0xfffffffffffffffe;
  local_38 = DAT_140763590 ^ (ulonglong)auStackY_6b8;
  uVar6 = 0;
  bVar3 = false;
  local_658[0] = param_1;
  local_630 = QString::fromAscii_helper("KickidlerGrabber",0x10);
  QCoreApplication::setApplicationName((QString *)&local_630);
  QString::~QString((QString *)&local_630);
  local_5e8 = local_5a8;
  uVar8 = QFile::decodeName(local_5a8);
  uVar9 = thunk_FUN_140014760();
  thunk_FUN_140029960(uVar9,1,uVar8);
  uVar8 = thunk_FUN_140016aa0(local_5a0,0);
  thunk_FUN_14002c950(uVar8);
  QString::~QString(local_5a0);
  QString::QString(local_660);
  QString::QString(local_678);
  if (local_658[0] == 2) {
    local_628 = QString::fromAscii_helper("-get-network-proxies",0x14);
    bVar3 = true;
    bVar2 = QString::operator==((QString *)&local_628,param_2[1]);
    if (!bVar2) goto LAB_1400173bd;
    bVar2 = true;
  }
  else {
LAB_1400173bd:
    bVar2 = false;
  }
  if (bVar3) {
    QString::~QString((QString *)&local_628);
  }
  if (bVar2) {
    FUN_1400148a0();
    QString::~QString(local_678);
    QString::~QString(local_660);
    return 0;
  }
  QApplication::QApplication(local_5e0,local_658,param_2,0x50602);
  QGuiApplication::setQuitOnLastWindowClosed(false);
  uVar8 = QCoreApplication::arguments();
  FUN_140014570(&local_680,uVar8);
  if (*(int *)local_620 == 0) {
LAB_140017472:
    pQVar13 = (QString *)(local_620 + ((longlong)*(int *)(local_620 + 0xc) + 2) * 8);
    iVar7 = *(int *)(local_620 + 8);
    while ((QString *)(local_620 + ((longlong)iVar7 + 2) * 8) != pQVar13) {
      pQVar13 = pQVar13 + -8;
      QString::~QString(pQVar13);
    }
    QListData::dispose(local_620);
  }
  else if (*(int *)local_620 != -1) {
    LOCK();
    *(int *)local_620 = *(int *)local_620 + -1;
    UNLOCK();
    if (*(int *)local_620 == 0) goto LAB_140017472;
  }
  local_640 = QString::fromAscii_helper("-logging-postfix",0x10);
  if (1 < *(uint *)(local_680 + 0x10)) {
    pQVar10 = QHashData::detach_helper
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140006055,
                         (_func_void_Node_ptr *)&LAB_1400020ea,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_140017522:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ea);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar11 = local_680 + 0x10;
      iVar7 = *(int *)pQVar11;
      *(int *)pQVar11 = *(int *)pQVar11 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_140017522;
    }
    local_680 = pQVar10;
  }
  uVar12 = 0;
  if (*(int *)(local_680 + 0x20) != 0) {
    uVar5 = qHash((QString *)&local_640,*(uint *)(local_680 + 0x24));
    uVar12 = (ulonglong)uVar5;
  }
  if (*(uint *)(local_680 + 0x20) == 0) {
    pQVar10 = (QHashData *)&local_680;
  }
  else {
    pQVar10 = (QHashData *)
              (*(longlong *)(local_680 + 8) + (uVar12 % (ulonglong)*(uint *)(local_680 + 0x20)) * 8)
    ;
    if (*(QHashData **)pQVar10 != local_680) {
      if (*(longlong *)*(QHashData **)pQVar10 == 0) {
        qt_assert("*node == e || (*node)->next",
                  "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qhash.h",0x375);
      }
      pQVar11 = *(QHashData **)pQVar10;
      pQVar14 = local_680;
      if (pQVar11 != local_680) {
        do {
          if (((int)uVar12 == *(int *)(pQVar11 + 8)) &&
             (bVar3 = operator==((QString *)&local_640,(QString *)(pQVar11 + 0x10)),
             pQVar14 = local_680, bVar3)) break;
          pQVar10 = *(QHashData **)pQVar10;
          pQVar11 = *(QHashData **)pQVar10;
        } while (pQVar11 != pQVar14);
      }
    }
  }
  pQVar10 = *(QHashData **)pQVar10;
  QString::~QString((QString *)&local_640);
  QString::QString((QString *)&local_670);
  pQVar11 = local_680;
  if (1 < *(uint *)(local_680 + 0x10)) {
    pQVar11 = QHashData::detach_helper
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140006055,
                         (_func_void_Node_ptr *)&LAB_1400020ea,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_140017649:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ea);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar14 = local_680 + 0x10;
      iVar7 = *(int *)pQVar14;
      *(int *)pQVar14 = *(int *)pQVar14 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_140017649;
    }
  }
  local_680 = pQVar11;
  if (pQVar10 != local_680) {
    QString::operator=((QString *)&local_670,(QString *)(pQVar10 + 0x18));
  }
  SetProcessShutdownParameters(0,0);
  local_668[0] = 0xffffffff;
  dwProcessId = GetCurrentProcessId();
  ProcessIdToSessionId(dwProcessId,local_668);
  local_618 = QString::fromAscii_helper("subAgent",8);
  pQVar13 = (QString *)thunk_FUN_140011a70(local_5c8,&local_618,&DAT_1400dbf2c);
  QChar::QChar(local_648,0x20);
  pQVar13 = (QString *)QString::arg(pQVar13,local_5d0,local_668[0],0);
  QString::operator=(local_678,pQVar13);
  QString::~QString(local_5d0);
  QString::~QString(local_5c8);
  QString::~QString((QString *)&local_618);
  bVar3 = *(int *)(local_670 + 4) != 0;
  if (bVar3) {
    pQVar13 = (QString *)thunk_FUN_140011b00(local_5c0,&DAT_1400dbf30,&local_670);
  }
  else {
    local_610 = QString::fromAscii_helper("",0);
    pQVar13 = (QString *)&local_610;
  }
  QString::append(local_678,pQVar13);
  if (bVar3) {
    QString::~QString(local_5c0);
  }
  else {
    QString::~QString((QString *)&local_610);
  }
  local_5f0 = QString::fromAscii_helper("_",1);
  local_5f8 = QString::fromAscii_helper(".",1);
  local_600 = QString::fromAscii_helper("_",1);
  local_608 = QString::fromAscii_helper(":",1);
  pQVar13 = QString::replace(local_678,(QString *)&local_608,(QString *)&local_600,1);
  QString::replace(pQVar13,(QString *)&local_5f8,(QString *)&local_5f0,1);
  QString::~QString((QString *)&local_608);
  QString::~QString((QString *)&local_600);
  QString::~QString((QString *)&local_5f8);
  QString::~QString((QString *)&local_5f0);
  uVar8 = thunk_FUN_140016aa0(local_5b0,8);
  uVar8 = thunk_FUN_140011a70(local_5b8,uVar8,&DAT_1400dbf38);
  thunk_FUN_140011a00(local_650,uVar8,local_678);
  QString::~QString(local_5b8);
  QString::~QString(local_5b0);
  uVar8 = QString::QString((QString *)&local_5e8,local_650);
  thunk_FUN_140034c50(uVar8,0xa00000);
  qInstallMessageHandler((_func_void_QtMsgType_QMessageLogContext_ptr_QString_ptr *)&LAB_140004435);
  local_638 = QString::fromAscii_helper("-ipc-name",9);
  if (1 < *(uint *)(local_680 + 0x10)) {
    pQVar10 = QHashData::detach_helper
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140006055,
                         (_func_void_Node_ptr *)&LAB_1400020ea,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_1400179be:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ea);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar11 = local_680 + 0x10;
      iVar7 = *(int *)pQVar11;
      *(int *)pQVar11 = *(int *)pQVar11 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_1400179be;
    }
    local_680 = pQVar10;
  }
  if (*(int *)(local_680 + 0x20) != 0) {
    uVar6 = qHash((QString *)&local_638,*(uint *)(local_680 + 0x24));
  }
  if (*(uint *)(local_680 + 0x20) == 0) {
    pQVar10 = (QHashData *)&local_680;
  }
  else {
    pQVar10 = (QHashData *)
              (*(longlong *)(local_680 + 8) +
              ((ulonglong)uVar6 % (ulonglong)*(uint *)(local_680 + 0x20)) * 8);
    if (*(QHashData **)pQVar10 != local_680) {
      if (*(longlong *)*(QHashData **)pQVar10 == 0) {
        qt_assert("*node == e || (*node)->next",
                  "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qhash.h",0x375);
      }
      pQVar11 = *(QHashData **)pQVar10;
      pQVar14 = local_680;
      if (pQVar11 != local_680) {
        do {
          if ((uVar6 == *(uint *)(pQVar11 + 8)) &&
             (bVar3 = operator==((QString *)&local_638,(QString *)(pQVar11 + 0x10)),
             pQVar14 = local_680, bVar3)) break;
          pQVar10 = *(QHashData **)pQVar10;
          pQVar11 = *(QHashData **)pQVar10;
        } while (pQVar11 != pQVar14);
      }
    }
  }
  pQVar10 = *(QHashData **)pQVar10;
  QString::~QString((QString *)&local_638);
  pQVar11 = local_680;
  if (1 < *(uint *)(local_680 + 0x10)) {
    pQVar11 = QHashData::detach_helper
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140006055,
                         (_func_void_Node_ptr *)&LAB_1400020ea,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_140017ae3:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ea);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar14 = local_680 + 0x10;
      iVar7 = *(int *)pQVar14;
      *(int *)pQVar14 = *(int *)pQVar14 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_140017ae3;
    }
  }
  local_680 = pQVar11;
  if (pQVar10 != local_680) {
    QString::operator=(local_660,(QString *)(pQVar10 + 0x18));
    uVar8 = thunk_FUN_140034660(local_560);
    cVar4 = thunk_FUN_140034220(uVar8,20000);
    thunk_FUN_140033080(local_560);
    if (cVar4 != '\0') {
      local_318 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_290);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_318,local_310,false);
      *(undefined ***)(local_310 + (longlong)*(int *)(local_318 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_31c + (longlong)*(int *)(local_318 + 4)) =
           *(int *)(local_318 + 4) + -0x88;
      thunk_FUN_140010dc0(local_310,2);
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010590(&local_318,"start agent for ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,local_668[0]);
      uVar8 = thunk_FUN_140016ea0(&local_318,local_98);
      uVar9 = thunk_FUN_140034660(local_550);
      thunk_FUN_140033850(uVar9,20000,uVar8,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                         );
      thunk_FUN_140033080(local_550);
      if (0xf < local_80) {
        uVar12 = local_98[0];
        if (0xfff < local_80 + 1) {
          if ((local_98[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar12 = *(ulonglong *)(local_98[0] - 8);
          if (local_98[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_98[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_98[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c2(uVar12);
      }
      thunk_FUN_140011330(local_290);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_290);
    }
    thunk_FUN_140019e70(local_138);
    thunk_FUN_14001c2c0(local_138,local_660);
    iVar7 = QApplication::exec();
    thunk_FUN_14001a520(local_138);
    QString::~QString(local_650);
    QString::~QString((QString *)&local_670);
    if (*(int *)(local_680 + 0x10) != 0) {
      if (*(int *)(local_680 + 0x10) == -1) goto LAB_140017f35;
      LOCK();
      pQVar10 = local_680 + 0x10;
      iVar1 = *(int *)pQVar10;
      *(int *)pQVar10 = *(int *)pQVar10 + -1;
      UNLOCK();
      if (iVar1 != 1) goto LAB_140017f35;
    }
    QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ea);
LAB_140017f35:
    QApplication::~QApplication(local_5e0);
    QString::~QString(local_678);
    QString::~QString(local_660);
    return iVar7;
  }
  uVar8 = thunk_FUN_140034660(local_580);
  cVar4 = thunk_FUN_140034220(uVar8,40000);
  thunk_FUN_140033080(local_580);
  if (cVar4 != '\0') {
    local_408 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_380)
    ;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_408,local_400,false);
    *(undefined ***)(local_400 + (longlong)*(int *)(local_408 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_40c + (longlong)*(int *)(local_408 + 4)) =
         *(int *)(local_408 + 4) + -0x88;
    thunk_FUN_140010dc0(local_400,2);
    thunk_FUN_140010590(&local_408,"IPC name not specified");
    uVar8 = thunk_FUN_140016ea0(&local_408,local_b8);
    uVar9 = thunk_FUN_140034660(local_570);
    thunk_FUN_140033850(uVar9,40000,uVar8,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033080(local_570);
    if (0xf < local_a0) {
      uVar12 = local_b8[0];
      if (0xfff < local_a0 + 1) {
        if ((local_b8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar12 = *(ulonglong *)(local_b8[0] - 8);
        if (local_b8[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_b8[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_b8[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c2(uVar12);
    }
    thunk_FUN_140011330(local_380);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_380);
  }
  QString::~QString(local_650);
  QString::~QString((QString *)&local_670);
  if (*(int *)(local_680 + 0x10) != 0) {
    if (*(int *)(local_680 + 0x10) == -1) goto LAB_140017cee;
    LOCK();
    pQVar10 = local_680 + 0x10;
    iVar7 = *(int *)pQVar10;
    *(int *)pQVar10 = *(int *)pQVar10 + -1;
    UNLOCK();
    if (iVar7 != 1) goto LAB_140017cee;
  }
  QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ea);
LAB_140017cee:
  QApplication::~QApplication(local_5e0);
  QString::~QString(local_678);
  QString::~QString(local_660);
  return 1;
}

// =============================================================================
// Ghidra: FUN_1400148a0 @ 0x1400148a0
// =============================================================================
void FUN_1400148a0(void)

{
  char *pcVar1;
  int iVar2;
  uint uVar3;
  bool bVar4;
  bool bVar5;
  ushort uVar6;
  ProxyType PVar7;
  QMapDataBase *pQVar8;
  ulonglong *puVar9;
  QVariant *pQVar10;
  QString *pQVar11;
  QMapDataBase *pQVar12;
  QJsonDocument *this;
  undefined8 uVar13;
  basic_ostream<char,std::char_traits<char>_> *this_00;
  longlong lVar14;
  longlong lVar15;
  longlong lVar16;
  QMapDataBase *pQVar17;
  QNetworkProxy *pQVar18;
  QMapDataBase *local_res8;
  QTypedArrayData<unsigned_short> *local_res10;
  QTypedArrayData<unsigned_short> *local_res18;
  QTypedArrayData<unsigned_short> *local_res20;
  QTypedArrayData<unsigned_short> *local_188;
  QTypedArrayData<unsigned_short> *local_180;
  code *local_178;
  Data *local_170;
  QListData local_168 [8];
  QNetworkProxy *local_160;
  QNetworkProxy *local_158;
  undefined4 local_150;
  QNetworkProxyQuery local_148 [8];
  QString local_140 [8];
  QString local_138 [8];
  QString local_130 [8];
  QByteArray local_128 [8];
  QVariant local_120 [8];
  QVariant local_118 [16];
  QVariant local_108 [16];
  QVariant local_f8 [16];
  QVariant local_e8 [16];
  QVariant local_d8 [16];
  QVariant local_c8 [16];
  QVariant local_b8 [16];
  undefined8 local_a8;
  QVariant local_a0 [16];
  QVariant local_90 [16];
  QVariant local_80 [16];
  QVariant local_70 [16];
  QVariant local_60 [32];
  
  local_a8 = 0xfffffffffffffffe;
  QNetworkProxyQuery::QNetworkProxyQuery(local_148);
  QNetworkProxyFactory::systemProxyForQuery((QNetworkProxyQuery *)&local_170);
  QNetworkProxyQuery::~QNetworkProxyQuery(local_148);
  local_178 = shared_null_exref;
  thunk_FUN_140010a70(local_168,&local_170);
  local_160 = (QNetworkProxy *)QListData::begin(local_168);
  local_158 = (QNetworkProxy *)QListData::end(local_168);
  for (; pQVar18 = local_160, local_150 = 1, local_160 != local_158; local_160 = local_160 + 8) {
    local_res8 = (QMapDataBase *)shared_null_exref;
    PVar7 = QNetworkProxy::type(local_160);
    QVariant::QVariant(local_118,PVar7);
    lVar16 = -1;
    do {
      lVar14 = lVar16 + 1;
      pcVar1 = &UNK_1400dbea5 + lVar16;
      lVar16 = lVar14;
    } while (*pcVar1 != '\0');
    local_res10 = QString::fromAscii_helper("type",(int)lVar14);
    if (1 < *(uint *)local_res8) {
      pQVar8 = QMapDataBase::createData();
      if (*(longlong *)(local_res8 + 0x10) != 0) {
        puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140014a13:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_1400134c0();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_1400134c0();
          }
          QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
        }
        QMapDataBase::freeData(pQVar12);
      }
      else if (*(int *)local_res8 != -1) {
        LOCK();
        iVar2 = *(int *)local_res8;
        *(int *)local_res8 = *(int *)local_res8 + -1;
        UNLOCK();
        if (iVar2 == 1) goto LAB_140014a13;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014ad5:
      pQVar10 = (QVariant *)QVariant::QVariant(local_a0);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_140014b4c:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_1400134c0();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_1400134c0();
            }
            QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar12);
        }
        else if (*(int *)local_res8 != -1) {
          LOCK();
          iVar2 = *(int *)local_res8;
          *(int *)local_res8 = *(int *)local_res8 + -1;
          UNLOCK();
          if (iVar2 == 1) goto LAB_140014b4c;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014c30:
        pQVar12 = (QMapDataBase *)thunk_FUN_140013070(local_res8,&local_res10,pQVar10,pQVar8,bVar4);
      }
      else {
        do {
          pQVar8 = pQVar17;
          bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_res10);
          if (bVar4) {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
          }
          else {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
            pQVar12 = pQVar8;
          }
          bVar4 = !bVar4;
        } while (pQVar17 != (QMapDataBase *)0x0);
        if ((pQVar12 == (QMapDataBase *)0x0) ||
           (bVar5 = operator<((QString *)&local_res10,(QString *)(pQVar12 + 0x18)), bVar5))
        goto LAB_140014c30;
        QVariant::operator=((QVariant *)(pQVar12 + 0x20),pQVar10);
      }
      QVariant::~QVariant(local_a0);
    }
    else {
      pQVar12 = (QMapDataBase *)0x0;
      pQVar8 = *(QMapDataBase **)(local_res8 + 0x10);
      do {
        bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_res10);
        if (bVar4) {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
        }
        else {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
          pQVar12 = pQVar8;
        }
        pQVar8 = pQVar17;
      } while (pQVar17 != (QMapDataBase *)0x0);
      if ((pQVar12 == (QMapDataBase *)0x0) ||
         (bVar4 = operator<((QString *)&local_res10,(QString *)(pQVar12 + 0x18)), bVar4))
      goto LAB_140014ad5;
    }
    QVariant::operator=((QVariant *)(pQVar12 + 0x20),local_118);
    QString::~QString((QString *)&local_res10);
    QVariant::~QVariant(local_118);
    pQVar11 = (QString *)QNetworkProxy::hostName(pQVar18);
    QVariant::QVariant(local_108,pQVar11);
    lVar16 = -1;
    do {
      lVar15 = lVar16 + 1;
      lVar14 = lVar16 + 1;
      lVar16 = lVar15;
    } while ("hostName"[lVar14] != '\0');
    local_res18 = QString::fromAscii_helper("hostName",(int)lVar15);
    if (1 < *(uint *)local_res8) {
      pQVar8 = QMapDataBase::createData();
      if (*(longlong *)(local_res8 + 0x10) != 0) {
        puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140014d29:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_1400134c0();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_1400134c0();
          }
          QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
        }
        QMapDataBase::freeData(pQVar12);
      }
      else if (*(int *)local_res8 != -1) {
        LOCK();
        iVar2 = *(int *)local_res8;
        *(int *)local_res8 = *(int *)local_res8 + -1;
        UNLOCK();
        if (iVar2 == 1) goto LAB_140014d29;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014de5:
      pQVar10 = (QVariant *)QVariant::QVariant(local_90);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_140014e5c:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_1400134c0();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_1400134c0();
            }
            QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar12);
        }
        else if (*(int *)local_res8 != -1) {
          LOCK();
          iVar2 = *(int *)local_res8;
          *(int *)local_res8 = *(int *)local_res8 + -1;
          UNLOCK();
          if (iVar2 == 1) goto LAB_140014e5c;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014f40:
        pQVar12 = (QMapDataBase *)thunk_FUN_140013070(local_res8,&local_res18,pQVar10,pQVar8,bVar4);
      }
      else {
        do {
          pQVar8 = pQVar17;
          bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_res18);
          if (bVar4) {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
          }
          else {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
            pQVar12 = pQVar8;
          }
          bVar4 = !bVar4;
        } while (pQVar17 != (QMapDataBase *)0x0);
        if ((pQVar12 == (QMapDataBase *)0x0) ||
           (bVar5 = operator<((QString *)&local_res18,(QString *)(pQVar12 + 0x18)), bVar5))
        goto LAB_140014f40;
        QVariant::operator=((QVariant *)(pQVar12 + 0x20),pQVar10);
      }
      QVariant::~QVariant(local_90);
    }
    else {
      pQVar12 = (QMapDataBase *)0x0;
      pQVar8 = *(QMapDataBase **)(local_res8 + 0x10);
      do {
        bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_res18);
        if (bVar4) {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
        }
        else {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
          pQVar12 = pQVar8;
        }
        pQVar8 = pQVar17;
      } while (pQVar17 != (QMapDataBase *)0x0);
      if ((pQVar12 == (QMapDataBase *)0x0) ||
         (bVar4 = operator<((QString *)&local_res18,(QString *)(pQVar12 + 0x18)), bVar4))
      goto LAB_140014de5;
    }
    QVariant::operator=((QVariant *)(pQVar12 + 0x20),local_108);
    QString::~QString((QString *)&local_res18);
    QVariant::~QVariant(local_108);
    QString::~QString(local_140);
    pQVar11 = (QString *)QNetworkProxy::password(pQVar18);
    QVariant::QVariant(local_f8,pQVar11);
    lVar16 = -1;
    do {
      lVar15 = lVar16 + 1;
      lVar14 = lVar16 + 1;
      lVar16 = lVar15;
    } while ("password"[lVar14] != '\0');
    local_res20 = QString::fromAscii_helper("password",(int)lVar15);
    if (1 < *(uint *)local_res8) {
      pQVar8 = QMapDataBase::createData();
      if (*(longlong *)(local_res8 + 0x10) != 0) {
        puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140015043:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_1400134c0();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_1400134c0();
          }
          QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
        }
        QMapDataBase::freeData(pQVar12);
      }
      else if (*(int *)local_res8 != -1) {
        LOCK();
        iVar2 = *(int *)local_res8;
        *(int *)local_res8 = *(int *)local_res8 + -1;
        UNLOCK();
        if (iVar2 == 1) goto LAB_140015043;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140015105:
      pQVar10 = (QVariant *)QVariant::QVariant(local_80);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_14001517c:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_1400134c0();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_1400134c0();
            }
            QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar12);
        }
        else if (*(int *)local_res8 != -1) {
          LOCK();
          iVar2 = *(int *)local_res8;
          *(int *)local_res8 = *(int *)local_res8 + -1;
          UNLOCK();
          if (iVar2 == 1) goto LAB_14001517c;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140015260:
        pQVar12 = (QMapDataBase *)thunk_FUN_140013070(local_res8,&local_res20,pQVar10,pQVar8,bVar4);
      }
      else {
        do {
          pQVar8 = pQVar17;
          bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_res20);
          if (bVar4) {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
          }
          else {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
            pQVar12 = pQVar8;
          }
          bVar4 = !bVar4;
        } while (pQVar17 != (QMapDataBase *)0x0);
        if ((pQVar12 == (QMapDataBase *)0x0) ||
           (bVar5 = operator<((QString *)&local_res20,(QString *)(pQVar12 + 0x18)), bVar5))
        goto LAB_140015260;
        QVariant::operator=((QVariant *)(pQVar12 + 0x20),pQVar10);
      }
      QVariant::~QVariant(local_80);
    }
    else {
      pQVar12 = (QMapDataBase *)0x0;
      pQVar8 = *(QMapDataBase **)(local_res8 + 0x10);
      do {
        bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_res20);
        if (bVar4) {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
        }
        else {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
          pQVar12 = pQVar8;
        }
        pQVar8 = pQVar17;
      } while (pQVar17 != (QMapDataBase *)0x0);
      if ((pQVar12 == (QMapDataBase *)0x0) ||
         (bVar4 = operator<((QString *)&local_res20,(QString *)(pQVar12 + 0x18)), bVar4))
      goto LAB_140015105;
    }
    QVariant::operator=((QVariant *)(pQVar12 + 0x20),local_f8);
    QString::~QString((QString *)&local_res20);
    QVariant::~QVariant(local_f8);
    QString::~QString(local_138);
    uVar6 = QNetworkProxy::port(pQVar18);
    QVariant::QVariant(local_e8,(uint)uVar6);
    lVar16 = -1;
    do {
      lVar14 = lVar16 + 1;
      pcVar1 = &UNK_1400dbecd + lVar16;
      lVar16 = lVar14;
    } while (*pcVar1 != '\0');
    local_188 = QString::fromAscii_helper("port",(int)lVar14);
    if (1 < *(uint *)local_res8) {
      pQVar8 = QMapDataBase::createData();
      if (*(longlong *)(local_res8 + 0x10) != 0) {
        puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140015361:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_1400134c0();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_1400134c0();
          }
          QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
        }
        QMapDataBase::freeData(pQVar12);
      }
      else if (*(int *)local_res8 != -1) {
        LOCK();
        iVar2 = *(int *)local_res8;
        *(int *)local_res8 = *(int *)local_res8 + -1;
        UNLOCK();
        if (iVar2 == 1) goto LAB_140015361;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140015421:
      pQVar10 = (QVariant *)QVariant::QVariant(local_70);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_140015498:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_1400134c0();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_1400134c0();
            }
            QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
          }
          QMapDataBase::freeData(pQVar12);
        }
        else if (*(int *)local_res8 != -1) {
          LOCK();
          iVar2 = *(int *)local_res8;
          *(int *)local_res8 = *(int *)local_res8 + -1;
          UNLOCK();
          if (iVar2 == 1) goto LAB_140015498;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_14001557c:
        pQVar12 = (QMapDataBase *)thunk_FUN_140013070(local_res8,&local_188,pQVar10,pQVar8,bVar4);
      }
      else {
        do {
          pQVar8 = pQVar17;
          bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_188);
          if (bVar4) {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
          }
          else {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
            pQVar12 = pQVar8;
          }
          bVar4 = !bVar4;
        } while (pQVar17 != (QMapDataBase *)0x0);
        if ((pQVar12 == (QMapDataBase *)0x0) ||
           (bVar5 = operator<((QString *)&local_188,(QString *)(pQVar12 + 0x18)), bVar5))
        goto LAB_14001557c;
        QVariant::operator=((QVariant *)(pQVar12 + 0x20),pQVar10);
      }
      QVariant::~QVariant(local_70);
    }
    else {
      pQVar12 = (QMapDataBase *)0x0;
      pQVar8 = *(QMapDataBase **)(local_res8 + 0x10);
      do {
        bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_188);
        if (bVar4) {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
        }
        else {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
          pQVar12 = pQVar8;
        }
        pQVar8 = pQVar17;
      } while (pQVar17 != (QMapDataBase *)0x0);
      if ((pQVar12 == (QMapDataBase *)0x0) ||
         (bVar4 = operator<((QString *)&local_188,(QString *)(pQVar12 + 0x18)), bVar4))
      goto LAB_140015421;
    }
    QVariant::operator=((QVariant *)(pQVar12 + 0x20),local_e8);
    QString::~QString((QString *)&local_188);
    QVariant::~QVariant(local_e8);
    pQVar11 = (QString *)QNetworkProxy::user(pQVar18);
    QVariant::QVariant(local_d8,pQVar11);
    lVar16 = -1;
    do {
      lVar14 = lVar16 + 1;
      pcVar1 = &UNK_1400dbed5 + lVar16;
      lVar16 = lVar14;
    } while (*pcVar1 != '\0');
    local_180 = QString::fromAscii_helper("user",(int)lVar14);
    if (1 < *(uint *)local_res8) {
      pQVar8 = QMapDataBase::createData();
      if (*(longlong *)(local_res8 + 0x10) != 0) {
        puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140015672:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_1400134c0();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_1400134c0();
          }
          QMapDataBase::freeTree(pQVar12,*(QMapNodeBase **)(pQVar12 + 0x10),8);
        }
        QMapDataBase::freeData(pQVar12);
      }
      else if (*(int *)local_res8 != -1) {
        LOCK();
        iVar2 = *(int *)local_res8;
        *(int *)local_res8 = *(int *)local_res8 + -1;
        UNLOCK();
        if (iVar2 == 1) goto LAB_140015672;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140015731:
      pQVar10 = (QVariant *)QVariant::QVariant(local_60);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        if (*(uint *)local_res8 == 0) {
LAB_1400157a7:
          thunk_FUN_140013430(local_res8);
        }
        else if (*(uint *)local_res8 != 0xffffffff) {
          LOCK();
          uVar3 = *(uint *)local_res8;
          *(uint *)local_res8 = *(uint *)local_res8 - 1;
          UNLOCK();
          if (uVar3 == 1) goto LAB_1400157a7;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_14001583c:
        pQVar12 = (QMapDataBase *)thunk_FUN_140013070(local_res8,&local_180,pQVar10,pQVar8,bVar4);
      }
      else {
        do {
          pQVar8 = pQVar17;
          bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_180);
          if (bVar4) {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
          }
          else {
            pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
            pQVar12 = pQVar8;
          }
          bVar4 = !bVar4;
        } while (pQVar17 != (QMapDataBase *)0x0);
        if ((pQVar12 == (QMapDataBase *)0x0) ||
           (bVar5 = operator<((QString *)&local_180,(QString *)(pQVar12 + 0x18)), bVar5))
        goto LAB_14001583c;
        QVariant::operator=((QVariant *)(pQVar12 + 0x20),pQVar10);
      }
      QVariant::~QVariant(local_60);
    }
    else {
      pQVar12 = (QMapDataBase *)0x0;
      pQVar8 = *(QMapDataBase **)(local_res8 + 0x10);
      do {
        bVar4 = operator<((QString *)(pQVar8 + 0x18),(QString *)&local_180);
        if (bVar4) {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 0x10);
        }
        else {
          pQVar17 = *(QMapDataBase **)(pQVar8 + 8);
          pQVar12 = pQVar8;
        }
        pQVar8 = pQVar17;
      } while (pQVar17 != (QMapDataBase *)0x0);
      if ((pQVar12 == (QMapDataBase *)0x0) ||
         (bVar4 = operator<((QString *)&local_180,(QString *)(pQVar12 + 0x18)), bVar4))
      goto LAB_140015731;
    }
    QVariant::operator=((QVariant *)(pQVar12 + 0x20),local_d8);
    QString::~QString((QString *)&local_180);
    QVariant::~QVariant(local_d8);
    QString::~QString(local_130);
    QVariant::QVariant(local_c8,(QMap<QString,QVariant> *)&local_res8);
    thunk_FUN_140012a70(&local_178,local_c8);
    QVariant::~QVariant(local_c8);
    if (*(uint *)local_res8 == 0) {
LAB_1400158e7:
      thunk_FUN_140013430();
    }
    else if (*(uint *)local_res8 != 0xffffffff) {
      LOCK();
      uVar3 = *(uint *)local_res8;
      *(uint *)local_res8 = *(uint *)local_res8 - 1;
      UNLOCK();
      if (uVar3 == 1) goto LAB_1400158e7;
    }
  }
  local_150 = 1;
  thunk_FUN_1400110d0(local_168);
  QVariant::QVariant(local_b8,(QList<QVariant> *)&local_178);
  this = (QJsonDocument *)QJsonDocument::fromVariant(local_120);
  uVar13 = QJsonDocument::toJson(this);
  this_00 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_140011650(cout_exref,uVar13);
  std::basic_ostream<char,std::char_traits<char>_>::operator<<
            (this_00,(_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                      *)&LAB_1400025cc);
  QByteArray::~QByteArray(local_128);
  QJsonDocument::~QJsonDocument((QJsonDocument *)local_120);
  QVariant::~QVariant(local_b8);
  if (*(int *)local_178 != 0) {
    if (*(int *)local_178 == -1) goto LAB_1400159c6;
    LOCK();
    iVar2 = *(int *)local_178;
    *(int *)local_178 = *(int *)local_178 + -1;
    UNLOCK();
    if (iVar2 != 1) goto LAB_1400159c6;
  }
  thunk_FUN_140013220(&local_178,local_178);
LAB_1400159c6:
  if (*(int *)local_170 != 0) {
    if (*(int *)local_170 == -1) {
      return;
    }
    LOCK();
    iVar2 = *(int *)local_170;
    *(int *)local_170 = *(int *)local_170 + -1;
    UNLOCK();
    if (iVar2 != 1) {
      return;
    }
  }
  pQVar18 = (QNetworkProxy *)(local_170 + ((longlong)*(int *)(local_170 + 0xc) + 2) * 8);
  iVar2 = *(int *)(local_170 + 8);
  while ((QNetworkProxy *)(local_170 + ((longlong)iVar2 + 2) * 8) != pQVar18) {
    pQVar18 = pQVar18 + -8;
    QNetworkProxy::~QNetworkProxy(pQVar18);
  }
  QListData::dispose(local_170);
  return;
}

// =============================================================================
// Ghidra: FUN_140015eb0 @ 0x140015eb0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140015eb0(int param_1,undefined8 param_2,QString *param_3)

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
  local_10 = DAT_140763590 ^ (ulonglong)auStack_168;
  local_138[0] = 0;
  if (param_1 == 0) {
    uVar2 = thunk_FUN_140034660(local_138);
    cVar1 = thunk_FUN_140034220(uVar2,10000);
    thunk_FUN_140033080(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140010dc0(local_120,2);
    uVar2 = thunk_FUN_140010590(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140010590(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140010590(uVar2,&DAT_1400dbd64);
    uVar2 = thunk_FUN_140016ea0(&local_128,local_30);
    uVar5 = thunk_FUN_140034660(local_138);
    local_148 = 0x43;
    thunk_FUN_140033850(uVar5,10000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033080(local_138);
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
      FUN_1400025c2(uVar6);
    }
  }
  else if (param_1 == 1) {
    uVar2 = thunk_FUN_140034660(local_138);
    cVar1 = thunk_FUN_140034220(uVar2,30000);
    thunk_FUN_140033080(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140010dc0(local_120,2);
    uVar2 = thunk_FUN_140010590(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140010590(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140010590(uVar2,&DAT_1400dbd64);
    uVar2 = thunk_FUN_140016ea0(&local_128,local_30);
    uVar5 = thunk_FUN_140034660(local_138);
    local_148 = 0x46;
    thunk_FUN_140033850(uVar5,30000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033080(local_138);
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
      FUN_1400025c2(uVar6);
    }
  }
  else if (param_1 == 2) {
    uVar2 = thunk_FUN_140034660(local_138);
    cVar1 = thunk_FUN_140034220(uVar2,40000);
    thunk_FUN_140033080(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140010dc0(local_120,2);
    uVar2 = thunk_FUN_140010590(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140010590(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140010590(uVar2,&DAT_1400dbd64);
    uVar2 = thunk_FUN_140016ea0(&local_128,local_30);
    uVar5 = thunk_FUN_140034660(local_138);
    local_148 = 0x49;
    thunk_FUN_140033850(uVar5,40000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033080(local_138);
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
      FUN_1400025c2(uVar6);
    }
  }
  else if (param_1 == 3) {
    uVar2 = thunk_FUN_140034660(local_138);
    cVar1 = thunk_FUN_140034220(uVar2,50000);
    thunk_FUN_140033080(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 8;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140010dc0(local_120,2);
    uVar2 = thunk_FUN_140010590(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140010590(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140010590(uVar2,&DAT_1400dbd64);
    uVar2 = thunk_FUN_140016ea0(&local_128,local_30);
    uVar5 = thunk_FUN_140034660(local_138);
    local_148 = 0x4c;
    thunk_FUN_140033850(uVar5,50000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033080(local_138);
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
      FUN_1400025c2(uVar6);
    }
  }
  else {
    uVar2 = thunk_FUN_140034660(local_138);
    cVar1 = thunk_FUN_140034220(uVar2,20000);
    thunk_FUN_140033080(local_138);
    if (cVar1 == '\0') {
      return;
    }
    local_128 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_138[0] = 0x10;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
    *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
         *(int *)(local_128 + 4) + -0x88;
    thunk_FUN_140010dc0(local_120,2);
    uVar2 = thunk_FUN_140010590(&local_128,"qt say: [");
    pQVar3 = (QByteArray *)QString::toLocal8Bit(param_3);
    pcVar4 = QByteArray::data(pQVar3);
    thunk_FUN_140010590(uVar2,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_138);
    thunk_FUN_140010590(uVar2,&DAT_1400dbd64);
    uVar2 = thunk_FUN_140016ea0(&local_128,local_30);
    uVar5 = thunk_FUN_140034660(local_138);
    local_148 = 0x4f;
    thunk_FUN_140033850(uVar5,20000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033080(local_138);
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
      FUN_1400025c2(uVar6);
    }
  }
  thunk_FUN_140011330(local_a0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0);
  return;
}

// =============================================================================
// Ghidra: FUN_140019e70 @ 0x140019e70
// =============================================================================
QObject * FUN_140019e70(QObject *param_1)

{
  int iVar1;
  QByteArray *pQVar2;
  char *pcVar3;
  char *pcVar4;
  longlong local_res10;
  QByteArray local_res18 [8];
  
  QObject::QObject(param_1,(QObject *)0x0);
  *(undefined ***)param_1 = grabberSubAgent::io::Service::vftable;
  QLocalServer::QLocalServer((QLocalServer *)(param_1 + 0x10),(QObject *)0x0);
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x50) = 0;
  *(undefined8 *)(param_1 + 0x58) = 0;
  QTimer::QTimer((QTimer *)(param_1 + 0x60),(QObject *)0x0);
  QMetaObject::normalizedType((char *)&local_res10);
  QByteArray::constData((QByteArray *)&local_res10);
  pQVar2 = (QByteArray *)QMetaObject::normalizedType((char *)local_res18);
  iVar1 = *(int *)(local_res10 + 4);
  if (iVar1 == *(int *)(*(longlong *)pQVar2 + 4)) {
    pcVar3 = QByteArray::constData(pQVar2);
    pcVar4 = QByteArray::constData((QByteArray *)&local_res10);
    iVar1 = memcmp(pcVar4,pcVar3,(longlong)iVar1);
    if (iVar1 != 0) goto LAB_140019f46;
  }
  else {
LAB_140019f46:
    qt_assert_x("qRegisterNormalizedMetaType",
                "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
                ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
  }
  QByteArray::~QByteArray(local_res18);
  QMetaType::registerNormalizedType(&local_res10,&LAB_140007cc5,&LAB_140002e50,4,0x17,0);
  QByteArray::~QByteArray((QByteArray *)&local_res10);
  QMetaObject::normalizedType((char *)&local_res10);
  QByteArray::constData((QByteArray *)&local_res10);
  pQVar2 = (QByteArray *)QMetaObject::normalizedType((char *)local_res18);
  iVar1 = *(int *)(local_res10 + 4);
  if (iVar1 == *(int *)(*(longlong *)pQVar2 + 4)) {
    pcVar3 = QByteArray::constData(pQVar2);
    pcVar4 = QByteArray::constData((QByteArray *)&local_res10);
    iVar1 = memcmp(pcVar4,pcVar3,(longlong)iVar1);
    if (iVar1 == 0) goto LAB_14001a037;
  }
  qt_assert_x("qRegisterNormalizedMetaType",
              "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
              ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
LAB_14001a037:
  QByteArray::~QByteArray(local_res18);
  QMetaType::registerNormalizedType(&local_res10,&LAB_140007a81,&LAB_140007d29,4,0x17,0);
  QByteArray::~QByteArray((QByteArray *)&local_res10);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_14001c2c0 @ 0x14001c2c0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001c2c0(char *param_1,QString *param_2)

{
  QLocalServer *this;
  undefined8 *puVar1;
  longlong *plVar2;
  char cVar3;
  bool bVar4;
  undefined8 *puVar5;
  char *pcVar6;
  QObject *pQVar7;
  undefined8 uVar8;
  QByteArray *this_00;
  undefined8 uVar9;
  longlong *plVar10;
  longlong *plVar11;
  char *pcVar12;
  ulonglong uVar13;
  uint uVar14;
  undefined1 auStackY_348 [32];
  undefined ***local_318;
  uint local_310;
  undefined8 local_308;
  int *local_300;
  QObject local_2f8 [8];
  QByteArray local_2f0 [8];
  Connection local_2e8 [8];
  undefined **local_2e0;
  undefined8 local_2d8;
  undefined8 local_2d0;
  char *local_2c8;
  undefined1 local_2c0;
  undefined8 local_2b8;
  undefined1 local_2b0 [16];
  undefined1 local_2a0 [16];
  undefined1 local_290 [16];
  undefined1 local_280 [20];
  int iStack_26c;
  undefined *local_268;
  undefined **local_260;
  basic_ostream<char,std::char_traits<char>_> local_258 [96];
  undefined8 local_1f8;
  undefined4 local_1f0;
  basic_ios<char,std::char_traits<char>_> local_1e0 [100];
  int iStack_17c;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  ulonglong local_88 [3];
  ulonglong local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_2b8 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStackY_348;
  plVar11 = (longlong *)0x0;
  uVar14 = 0;
  local_310 = 0;
  puVar5 = (undefined8 *)thunk_FUN_140020eb0();
  puVar1 = *(undefined8 **)(param_1 + 0x40);
  if ((puVar1 != puVar5) && (*(undefined8 **)(param_1 + 0x40) = puVar5, puVar1 != (undefined8 *)0x0)
     ) {
    (**(code **)*puVar1)(puVar1,1);
  }
  this = (QLocalServer *)(param_1 + 0x10);
  pcVar6 = qFlagLocation("1newConnection()");
  pQVar7 = (QObject *)qFlagLocation("2newConnection()");
  QObject::connect(local_2f8,(char *)this,pQVar7,param_1,(ConnectionType)pcVar6);
  QMetaObject::Connection::~Connection((Connection *)local_2f8);
  uVar8 = thunk_FUN_140034660(local_2b0);
  cVar3 = thunk_FUN_140034220(uVar8,20000);
  thunk_FUN_140033080(local_2b0);
  if (cVar3 != '\0') {
    local_268 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1e0)
    ;
    uVar14 = 1;
    local_310 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_318 = &local_260;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1f8 = 0;
    local_1f0 = 4;
    uVar8 = thunk_FUN_140010590(&local_268,"Starting to listen: ");
    this_00 = (QByteArray *)QString::toLocal8Bit(param_2);
    pcVar6 = QByteArray::data(this_00);
    thunk_FUN_140010590(uVar8,pcVar6);
    QByteArray::~QByteArray(local_2f0);
    uVar8 = thunk_FUN_140016ea0(&local_268,local_88);
    uVar9 = thunk_FUN_140034660(local_2a0);
    thunk_FUN_140033850(uVar9,20000,uVar8,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033080(local_2a0);
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
      FUN_1400025c2(uVar13);
    }
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012800(&local_260);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_258);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_1e0);
  }
  bVar4 = QLocalServer::listen(this,param_2);
  if (bVar4) {
    *(uint *)(param_1 + 0x7c) = *(uint *)(param_1 + 0x7c) | 1;
    QTimer::setInterval((QTimer *)(param_1 + 0x60),60000);
    QTimer::start((QTimer *)(param_1 + 0x60));
    thunk_FUN_140018fd0(local_2e8,param_1 + 0x60,timeout_exref,param_1);
    QMetaObject::Connection::~Connection(local_2e8);
    local_318 = operator_new(0x118);
    plVar10 = plVar11;
    if (local_318 != (void *)0x0) {
      plVar10 = (longlong *)thunk_FUN_1400863b0(local_318);
    }
    plVar2 = *(longlong **)(param_1 + 0x48);
    if ((plVar2 != plVar10) && (*(longlong **)(param_1 + 0x48) = plVar10, plVar2 != (longlong *)0x0)
       ) {
      (**(code **)(*plVar2 + 0x18))(plVar2,1);
    }
    local_318 = operator_new(0xd0);
    if (local_318 != (void *)0x0) {
      plVar11 = (longlong *)thunk_FUN_14007dca0(local_318);
    }
    thunk_FUN_140020140(param_1 + 0x50,plVar11);
    if (*(longlong *)(param_1 + 0x48) == 0) {
      qt_assert("d","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qscopedpointer.h",
                0x70);
    }
    plVar11 = *(longlong **)(param_1 + 0x48);
    local_308 = *(undefined8 *)(param_1 + 0x50);
    local_300 = *(int **)(param_1 + 0x58);
    if (local_300 != (int *)0x0) {
      LOCK();
      *local_300 = *local_300 + 1;
      UNLOCK();
      LOCK();
      local_300[1] = local_300[1] + 1;
      UNLOCK();
    }
    (**(code **)(*plVar11 + 0x80))(plVar11,&local_308);
    pcVar6 = *(char **)self_exref;
    pcVar12 = qFlagLocation("1quit()");
    pQVar7 = (QObject *)qFlagLocation("2destroyed()");
    QObject::connect((QObject *)&local_318,(char *)this,pQVar7,pcVar6,(ConnectionType)pcVar12);
    QMetaObject::Connection::~Connection((Connection *)&local_318);
    return;
  }
  uVar8 = thunk_FUN_140034660(local_290);
  cVar3 = thunk_FUN_140034220(uVar8,30000);
  thunk_FUN_140033080(local_290);
  if (cVar3 != '\0') {
    local_178 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_f0);
    local_310 = uVar14 | 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    local_318 = &local_170;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
    local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_108 = 0;
    local_100 = 4;
    thunk_FUN_140010590(&local_178,"local server does not listen");
    uVar8 = thunk_FUN_140016ea0(&local_178,local_68);
    uVar9 = thunk_FUN_140034660(local_280);
    thunk_FUN_140033850(uVar9,30000,uVar8,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033080(local_280);
    if (0xf < local_50) {
      uVar13 = local_68[0];
      if (0xfff < local_50 + 1) {
        if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar13 = *(ulonglong *)(local_68[0] - 8);
        if (local_68[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_68[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_68[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c2(uVar13);
    }
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012800(&local_170);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_168);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_f0)
    ;
  }
  local_2e0 = std::exception::vftable;
  local_2d8 = 0;
  local_2d0 = 0;
  local_2c8 = "server refuse listening";
  local_2c0 = 1;
  __std_exception_copy(&local_2c8,&local_2d8);
  local_2e0 = std::runtime_error::vftable;
                    /* WARNING: Subroutine does not return */
  _CxxThrowException(&local_2e0,(ThrowInfo *)&DAT_140761da8);
}

// =============================================================================
// Ghidra: FUN_14001cb90 @ 0x14001cb90
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001cb90(longlong param_1)

{
  longlong *plVar1;
  char *pcVar2;
  char *pcVar3;
  char cVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  char *pcVar7;
  QObject *pQVar8;
  ulonglong uVar9;
  undefined1 auStackY_198 [32];
  QTypedArrayData<unsigned_short> *local_168;
  QTypedArrayData<unsigned_short> *local_160;
  QVariant local_158 [12];
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
  local_30 = DAT_140763590 ^ (ulonglong)auStackY_198;
  uVar5 = 0;
  local_160 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_160 & 0xffffffff00000000);
  if (-1 < *(int *)(param_1 + 0x70)) {
    QTimer::stop((QTimer *)(param_1 + 0x60));
  }
  if (*(longlong *)(param_1 + 0x20) == 0) {
    (**(code **)(*(longlong *)(param_1 + 0x10) + 0x60))();
    thunk_FUN_140020200(param_1 + 0x20);
    QObject::setParent(*(QObject **)(param_1 + 0x20),(QObject *)0x0);
    plVar1 = *(longlong **)(param_1 + 0x30);
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x18))(plVar1,1);
    }
    local_168 = operator_new(0x30);
    if (local_168 != (QTypedArrayData<unsigned_short> *)0x0) {
      uVar5 = thunk_FUN_1400775f0(local_168,param_1 + 0x20);
    }
    *(undefined8 *)(param_1 + 0x30) = uVar5;
    cVar4 = thunk_FUN_14007cb00(uVar5);
    if (cVar4 == '\0') {
      uVar5 = thunk_FUN_140034660(local_158);
      cVar4 = thunk_FUN_140034220(uVar5,40000);
      thunk_FUN_140033080(local_158);
      if (cVar4 != '\0') {
        local_148 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_160 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_160._4_4_,2);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_140010dc0(&local_140,2);
        thunk_FUN_140010590(&local_148,"Failed to create RPC object");
        uVar5 = thunk_FUN_140016ea0(&local_148,local_50);
        uVar6 = thunk_FUN_140034660(local_158);
        thunk_FUN_140033850(uVar6,40000,uVar5,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                           );
        thunk_FUN_140033080(local_158);
        if (0xf < local_38) {
          uVar9 = local_50[0];
          if (0xfff < local_38 + 1) {
            if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar9 = *(ulonglong *)(local_50[0] - 8);
            if (local_50[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_50[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_50[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c2(uVar9);
        }
        thunk_FUN_140011330(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
      QLocalSocket::abort(*(QLocalSocket **)(param_1 + 0x20));
    }
    else {
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"getUserInfo",param_1,"onGetUserInfo");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"getSystemProxies",param_1,
                          "onGetSystemProxies");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"resetClipboard",param_1,
                          "onResetClipboard");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setClipboardText",param_1,
                          "onSetClipboardText");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setClipboardHtml",param_1,
                          "onSetClipboardHtml");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setClipboardImage",param_1,
                          "onSetClipboardImage");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setClipboardMimeData",param_1,
                          "onSetClipboardMimeData");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"clearClipboard",param_1,
                          "onClearClipboard");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"getClipboardUrls",param_1,
                          "onGetClipboardUrls");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setSelfControlState",param_1,
                          "onSelfControlState");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setSelfControlDayActivity",param_1,
                          "onSelfControlDayActivity");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setSelfControlDayRating",param_1,
                          "onSelfControlDayRating");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setSelfControlWebToken",param_1,
                          "onSelfControlWebToken");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"notifySelfControlTasksChanged",param_1,
                          "onSelfControlTasksChanged");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setNotificationAlert",param_1,
                          "onNotificationAlert");
      thunk_FUN_140077dc0(*(undefined8 *)(param_1 + 0x30),"setWebinterfaceLink",param_1,
                          "onWebinterfaceLink");
      pcVar2 = *(char **)self_exref;
      pcVar3 = *(char **)(param_1 + 0x20);
      pcVar7 = qFlagLocation("1quit()");
      pQVar8 = (QObject *)qFlagLocation("2disconnected()");
      QObject::connect((QObject *)&local_168,pcVar3,pQVar8,pcVar2,(ConnectionType)pcVar7);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      if ((*(longlong *)(param_1 + 0x48) != 0) &&
         (cVar4 = (**(code **)(**(longlong **)(param_1 + 0x48) + 0x58))(), cVar4 != '\0')) {
        thunk_FUN_140018ab0(&local_168,*(undefined8 *)(param_1 + 0x48),thunk_FUN_140086210,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_168);
      }
      if (*(longlong *)(param_1 + 0x50) != 0) {
        thunk_FUN_140018c20(&local_168,*(longlong *)(param_1 + 0x50),thunk_FUN_140085fa0,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_168);
        thunk_FUN_140018c20(&local_168,*(undefined8 *)(param_1 + 0x50),thunk_FUN_140085fe0,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_168);
      }
      thunk_FUN_140018e70(&local_168,*(undefined8 *)(param_1 + 0x20),error_exref,0);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      thunk_FUN_140018f90(&local_168,*(undefined8 *)(param_1 + 0x20),stateChanged_exref,0);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      local_160 = QString::fromAscii_helper("test ping",9);
      QVariant::QVariant(local_158,(QString *)&local_160);
      QString::~QString((QString *)&local_160);
      local_168 = QString::fromAscii_helper("testPing",8);
      thunk_FUN_14007c750(*(undefined8 *)(param_1 + 0x30),&local_168,local_158);
      QString::~QString((QString *)&local_168);
      QVariant::~QVariant(local_158);
    }
  }
  else {
    uVar5 = thunk_FUN_140034660(local_158);
    cVar4 = thunk_FUN_140034220(uVar5,40000);
    thunk_FUN_140033080(local_158);
    if (cVar4 != '\0') {
      local_148 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_160 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_160._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_168 = (QTypedArrayData<unsigned_short> *)&local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      thunk_FUN_140010590(&local_148,"Attempted to connect to already connected IPC (!)");
      uVar5 = thunk_FUN_140016ea0(&local_148,local_50);
      uVar6 = thunk_FUN_140034660(local_158);
      thunk_FUN_140033850(uVar6,40000,uVar5,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                         );
      thunk_FUN_140033080(local_158);
      if (0xf < local_38) {
        uVar9 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar9 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c2(uVar9);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012800(&local_140);
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
// Ghidra: FUN_14001a7e0 @ 0x14001a7e0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001a7e0(undefined8 param_1,int param_2)

{
  char cVar1;
  undefined8 uVar2;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar3;
  ulonglong uVar4;
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
  local_28 = DAT_140763590 ^ (ulonglong)auStack_188;
  local_158 = 0;
  uVar2 = thunk_FUN_140034660(local_150);
  cVar1 = thunk_FUN_140034220(uVar2,10000);
  thunk_FUN_140033080(local_150);
  if (cVar1 != '\0') {
    local_138 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_b0);
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
    this = (basic_ostream<char,std::char_traits<char>_> *)
           thunk_FUN_140010590(&local_138,"IPC Socket got error: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
    uVar2 = thunk_FUN_140016ea0(&local_138,local_48);
    uVar3 = thunk_FUN_140034660(local_150);
    local_168 = 0xa5;
    thunk_FUN_140033850(uVar3,10000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033080(local_150);
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
      FUN_1400025c2(uVar4);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012800(&local_130);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_128);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14001aaa0 @ 0x14001aaa0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001aaa0(undefined8 param_1,int param_2)

{
  char cVar1;
  undefined8 uVar2;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar3;
  ulonglong uVar4;
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
  local_28 = DAT_140763590 ^ (ulonglong)auStack_188;
  local_158 = 0;
  uVar2 = thunk_FUN_140034660(local_150);
  cVar1 = thunk_FUN_140034220(uVar2,10000);
  thunk_FUN_140033080(local_150);
  if (cVar1 != '\0') {
    local_138 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_b0);
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
    this = (basic_ostream<char,std::char_traits<char>_> *)
           thunk_FUN_140010590(&local_138,"IPC Socket state: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
    uVar2 = thunk_FUN_140016ea0(&local_138,local_48);
    uVar3 = thunk_FUN_140034660(local_150);
    local_168 = 0xa7;
    thunk_FUN_140033850(uVar3,10000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033080(local_150);
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
      FUN_1400025c2(uVar4);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012800(&local_130);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_128);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14001d930 @ 0x14001d930
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001d930(void)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulonglong uVar4;
  undefined1 auStack_178 [32];
  undefined4 local_158;
  undefined4 local_148;
  undefined ***local_140 [2];
  undefined8 local_130;
  undefined *local_128;
  undefined **local_120;
  basic_ostream<char,std::char_traits<char>_> local_118 [96];
  undefined8 local_b8;
  undefined4 local_b0;
  basic_ios<char,std::char_traits<char>_> local_a0 [104];
  ulonglong local_38 [3];
  ulonglong local_20;
  ulonglong local_18;
  
  local_130 = 0xfffffffffffffffe;
  local_18 = DAT_140763590 ^ (ulonglong)auStack_178;
  local_148 = 0;
  uVar2 = thunk_FUN_140034660(local_140);
  cVar1 = thunk_FUN_140034220(uVar2,20000);
  thunk_FUN_140033080(local_140);
  if (cVar1 != '\0') {
    local_128 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_a0);
    local_148 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_128,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_120,false);
    *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
         *(int *)(local_128 + 4) + -0x88;
    local_140[0] = &local_120;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_120);
    local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_b8 = 0;
    local_b0 = 4;
    thunk_FUN_140010590(&local_128,"await conection timeout. aborting.");
    uVar2 = thunk_FUN_140016ea0(&local_128,local_38);
    uVar3 = thunk_FUN_140034660(local_140);
    local_158 = 0x16a;
    thunk_FUN_140033850(uVar3,20000,uVar2,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033080(local_140);
    if (0xf < local_20) {
      uVar4 = local_38[0];
      if (0xfff < local_20 + 1) {
        if ((local_38[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar4 = *(ulonglong *)(local_38[0] - 8);
        if (local_38[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_38[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_38[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c2(uVar4);
    }
    *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
         *(int *)(local_128 + 4) + -0x88;
    local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012800(&local_120);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_120);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_118);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0)
    ;
  }
  QCoreApplication::quit();
  return;
}

// =============================================================================
// Ghidra: FUN_14001a520 @ 0x14001a520
// =============================================================================
void FUN_14001a520(QObject *param_1)

{
  longlong *plVar1;
  undefined8 *puVar2;
  
  *(undefined ***)param_1 = grabberSubAgent::io::Service::vftable;
  plVar1 = *(longlong **)(param_1 + 0x38);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x18))(plVar1,1);
  }
  QTimer::~QTimer((QTimer *)(param_1 + 0x60));
  thunk_FUN_14001b900(*(undefined8 *)(param_1 + 0x58));
  plVar1 = *(longlong **)(param_1 + 0x48);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x18))(plVar1,1);
  }
  puVar2 = *(undefined8 **)(param_1 + 0x40);
  if (puVar2 != (undefined8 *)0x0) {
    (**(code **)*puVar2)(puVar2,1);
  }
  thunk_FUN_14001b9d0(*(undefined8 *)(param_1 + 0x28));
  QLocalServer::~QLocalServer((QLocalServer *)(param_1 + 0x10));
                    /* WARNING: Could not recover jumptable at 0x00014001a59c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  QObject::~QObject(param_1);
  return;
}

// =============================================================================
// Ghidra: FUN_14007b590 @ 0x14007b590
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14007b590(longlong param_1)

{
  int iVar1;
  QIODevice *this;
  QLocalSocket *pQVar2;
  char cVar3;
  LocalSocketError LVar4;
  LocalSocketState LVar5;
  undefined8 uVar6;
  char *pcVar7;
  __int64 _Var8;
  QString *pQVar9;
  basic_ostream<char,std::char_traits<char>_> *pbVar10;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar11;
  QByteArray *pQVar12;
  int *piVar13;
  undefined8 uVar14;
  ulonglong uVar15;
  basic_ios<char,std::char_traits<char>_> *this_00;
  int iVar16;
  uint uVar17;
  undefined1 auStack_588 [32];
  undefined4 local_568;
  uint local_558;
  undefined ***local_550;
  longlong local_548;
  undefined ***local_540;
  QString local_538 [8];
  int *local_530;
  QByteArray local_528 [8];
  QByteArray local_520 [8];
  undefined8 local_518;
  undefined1 local_510 [16];
  undefined1 local_500 [16];
  undefined1 local_4f0 [16];
  undefined1 local_4e0 [16];
  undefined1 local_4d0 [16];
  undefined1 local_4c0 [16];
  undefined1 local_4b0 [16];
  undefined1 local_4a0 [20];
  int iStack_48c;
  undefined *local_488;
  undefined **local_480;
  basic_ostream<char,std::char_traits<char>_> local_478 [96];
  undefined8 local_418;
  undefined4 local_410;
  basic_ios<char,std::char_traits<char>_> local_400 [100];
  int iStack_39c;
  undefined *local_398;
  undefined **local_390;
  basic_ostream<char,std::char_traits<char>_> local_388 [96];
  undefined8 local_328;
  undefined4 local_320;
  basic_ios<char,std::char_traits<char>_> local_310 [100];
  int iStack_2ac;
  undefined *local_2a8;
  undefined **local_2a0;
  basic_ostream<char,std::char_traits<char>_> local_298 [96];
  undefined8 local_238;
  undefined4 local_230;
  basic_ios<char,std::char_traits<char>_> local_220 [100];
  int iStack_1bc;
  undefined *local_1b8;
  undefined **local_1b0;
  basic_ostream<char,std::char_traits<char>_> local_1a8 [96];
  undefined8 local_148;
  undefined4 local_140;
  basic_ios<char,std::char_traits<char>_> local_130 [104];
  ulonglong local_c8 [3];
  ulonglong local_b0;
  ulonglong local_a8 [3];
  ulonglong local_90;
  ulonglong local_88 [3];
  ulonglong local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_518 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStack_588;
  local_558 = 0;
  iVar16 = 0;
  uVar6 = thunk_FUN_1400772b0(local_538);
  thunk_FUN_14007cb50(param_1,&local_548,uVar6);
  uVar17 = 0;
  if (*local_530 != 0) {
    if (*local_530 == -1) goto LAB_14007b627;
    LOCK();
    iVar1 = *local_530;
    *local_530 = *local_530 + -1;
    UNLOCK();
    uVar17 = local_558;
    if (iVar1 != 1) goto LAB_14007b627;
  }
  thunk_FUN_140013220(&local_530,local_530);
LAB_14007b627:
  QString::~QString(local_538);
  iVar1 = *(int *)(local_548 + 4);
  if (0 < iVar1) {
    do {
      this = *(QIODevice **)(param_1 + 0x10);
      pcVar7 = QByteArray::constData((QByteArray *)&local_548);
      _Var8 = QIODevice::write(this,pcVar7 + iVar16,(longlong)(iVar1 - iVar16));
      if ((int)_Var8 < 1) {
        uVar6 = thunk_FUN_140034660(local_4d0);
        cVar3 = thunk_FUN_140034220(uVar6,40000);
        thunk_FUN_140033080(local_4d0);
        if (cVar3 != '\0') {
          local_2a8 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_220);
          uVar17 = 1;
          local_558 = 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_2a8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_2a0,false);
          *(undefined ***)((longlong)&local_2a8 + (longlong)*(int *)(local_2a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
               *(int *)(local_2a8 + 4) + -0x88;
          local_540 = &local_2a0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_2a0);
          local_2a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_238 = 0;
          local_230 = 4;
          pQVar9 = (QString *)QIODevice::errorString(*(QIODevice **)(param_1 + 0x10));
          pQVar2 = *(QLocalSocket **)(param_1 + 0x10);
          LVar4 = QLocalSocket::error(pQVar2);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140010590(&local_2a8,"Error while writing to IPC: err=");
          pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,LVar4);
          uVar6 = thunk_FUN_140010590(pbVar11," err_str=");
          pQVar12 = (QByteArray *)QString::toLocal8Bit(pQVar9);
          pcVar7 = QByteArray::data(pQVar12);
          thunk_FUN_140010590(uVar6,pcVar7);
          QByteArray::~QByteArray(local_520);
          piVar13 = _errno();
          LVar5 = QLocalSocket::state(pQVar2);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140010590(uVar6," errno=");
          pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,*piVar13);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140010590(pbVar11," sockState=");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,LVar5);
          QString::~QString((QString *)&local_550);
          uVar6 = thunk_FUN_140016ea0(&local_2a8,local_88);
          uVar14 = thunk_FUN_140034660(local_4c0);
          local_568 = 0x50;
          thunk_FUN_140033850(uVar14,40000,uVar6,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-localRPC\\caller.cpp"
                             );
          thunk_FUN_140033080(local_4c0);
          if (0xf < local_70) {
            uVar15 = local_88[0];
            if (0xfff < local_70 + 1) {
              if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar15 = *(ulonglong *)(local_88[0] - 8);
              if (local_88[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_88[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_88[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar15);
          }
          *(undefined ***)((longlong)&local_2a8 + (longlong)*(int *)(local_2a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
               *(int *)(local_2a8 + 4) + -0x88;
          local_2a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_140012800(&local_2a0);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_2a0);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_298);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_220);
        }
        uVar6 = thunk_FUN_140034660(local_4b0);
        cVar3 = thunk_FUN_140034220(uVar6,20000);
        thunk_FUN_140033080(local_4b0);
        if (cVar3 != '\0') {
          local_1b8 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_130);
          local_558 = uVar17 | 2;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
          *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
               *(int *)(local_1b8 + 4) + -0x88;
          local_550 = &local_1b0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
          local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_148 = 0;
          local_140 = 4;
          thunk_FUN_140010590(&local_1b8,"Going to destroy IPC");
          uVar6 = thunk_FUN_140016ea0(&local_1b8,local_68);
          uVar14 = thunk_FUN_140034660(local_4a0);
          local_568 = 0x51;
          thunk_FUN_140033850(uVar14,20000,uVar6,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-localRPC\\caller.cpp"
                             );
          thunk_FUN_140033080(local_4a0);
          if (0xf < local_50) {
            uVar15 = local_68[0];
            if (0xfff < local_50 + 1) {
              if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar15 = *(ulonglong *)(local_68[0] - 8);
              if (local_68[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_68[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_68[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar15);
          }
          *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
               *(int *)(local_1b8 + 4) + -0x88;
          local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_140012800(&local_1b0);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_1a8);
          this_00 = local_130;
LAB_14007bfc2:
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (this_00);
        }
LAB_14007bfc8:
        QLocalSocket::abort(*(QLocalSocket **)(param_1 + 0x10));
        break;
      }
      cVar3 = (**(code **)(**(longlong **)(param_1 + 0x10) + 0xb8))
                        (*(longlong **)(param_1 + 0x10),10000);
      if (cVar3 == '\0') {
        uVar6 = thunk_FUN_140034660(local_510);
        cVar3 = thunk_FUN_140034220(uVar6,40000);
        thunk_FUN_140033080(local_510);
        if (cVar3 != '\0') {
          local_488 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_400);
          uVar17 = 4;
          local_558 = 4;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_488,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_480,false);
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_488 + *(int *)(local_488 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_48c + (longlong)*(int *)(local_488 + 4)) =
               *(int *)(local_488 + 4) + -0x88;
          local_550 = &local_480;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_480);
          local_480 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_418 = 0;
          local_410 = 4;
          pQVar9 = (QString *)QIODevice::errorString(*(QIODevice **)(param_1 + 0x10));
          pQVar2 = *(QLocalSocket **)(param_1 + 0x10);
          LVar4 = QLocalSocket::error(pQVar2);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140010590((basic_ostream<char,std::char_traits<char>_> *)&local_488,
                                        "Error while waitForBytesWritten: err=");
          pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,LVar4);
          uVar6 = thunk_FUN_140010590(pbVar11," err_str=");
          pQVar12 = (QByteArray *)QString::toLocal8Bit(pQVar9);
          pcVar7 = QByteArray::data(pQVar12);
          thunk_FUN_140010590(uVar6,pcVar7);
          QByteArray::~QByteArray(local_528);
          piVar13 = _errno();
          LVar5 = QLocalSocket::state(pQVar2);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140010590(uVar6," errno=");
          pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,*piVar13);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_140010590(pbVar11," sockState=");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,LVar5);
          QString::~QString((QString *)&local_540);
          uVar6 = thunk_FUN_140016ea0((basic_ostream<char,std::char_traits<char>_> *)&local_488,
                                      local_c8);
          uVar14 = thunk_FUN_140034660(local_500);
          local_568 = 0x58;
          thunk_FUN_140033850(uVar14,40000,uVar6,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-localRPC\\caller.cpp"
                             );
          thunk_FUN_140033080(local_500);
          if (0xf < local_b0) {
            uVar15 = local_c8[0];
            if (0xfff < local_b0 + 1) {
              if ((local_c8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar15 = *(ulonglong *)(local_c8[0] - 8);
              if (local_c8[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_c8[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_c8[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar15);
          }
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_488 + *(int *)(local_488 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_48c + (longlong)*(int *)(local_488 + 4)) =
               *(int *)(local_488 + 4) + -0x88;
          local_480 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_140012800(&local_480);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_480);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_478);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_400);
        }
        uVar6 = thunk_FUN_140034660(local_4f0);
        cVar3 = thunk_FUN_140034220(uVar6,20000);
        thunk_FUN_140033080(local_4f0);
        if (cVar3 != '\0') {
          local_398 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_310);
          local_558 = uVar17 | 8;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_398,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_390,false);
          *(undefined ***)((longlong)&local_398 + (longlong)*(int *)(local_398 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_39c + (longlong)*(int *)(local_398 + 4)) =
               *(int *)(local_398 + 4) + -0x88;
          local_550 = &local_390;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_390);
          local_390 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_328 = 0;
          local_320 = 4;
          thunk_FUN_140010590(&local_398,"Going to destroy IPC");
          uVar6 = thunk_FUN_140016ea0(&local_398,local_a8);
          uVar14 = thunk_FUN_140034660(local_4e0);
          local_568 = 0x59;
          thunk_FUN_140033850(uVar14,20000,uVar6,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-localRPC\\caller.cpp"
                             );
          thunk_FUN_140033080(local_4e0);
          if (0xf < local_90) {
            uVar15 = local_a8[0];
            if (0xfff < local_90 + 1) {
              if ((local_a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar15 = *(ulonglong *)(local_a8[0] - 8);
              if (local_a8[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_a8[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_a8[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar15);
          }
          *(undefined ***)((longlong)&local_398 + (longlong)*(int *)(local_398 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_39c + (longlong)*(int *)(local_398 + 4)) =
               *(int *)(local_398 + 4) + -0x88;
          local_390 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_140012800(&local_390);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_390);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_388);
          this_00 = local_310;
          goto LAB_14007bfc2;
        }
        goto LAB_14007bfc8;
      }
      iVar16 = iVar16 + (int)_Var8;
    } while (iVar16 < iVar1);
  }
  QByteArray::~QByteArray((QByteArray *)&local_548);
  return;
}

// =============================================================================
// Ghidra: FUN_140023ec0 @ 0x140023ec0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 * FUN_140023ec0(undefined ***param_1,undefined8 *param_2)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  DWORD DVar4;
  BOOL BVar5;
  __uint64 _Var6;
  undefined ***pppuVar7;
  undefined ***pppuVar8;
  QString *pQVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  longlong lVar12;
  undefined ***pppuVar13;
  QVariant *pQVar14;
  basic_ostream<char,std::char_traits<char>_> *pbVar15;
  ulonglong uVar16;
  undefined ***lpBuffer;
  uint uVar17;
  undefined1 auStack_1f8 [32];
  undefined4 local_1d8;
  QTypedArrayData<unsigned_short> *local_1c8;
  undefined ***local_1c0 [2];
  bool local_1b0;
  bool local_1af;
  uint local_1ac;
  QTypedArrayData<unsigned_short> *local_1a8;
  longlong local_1a0;
  longlong local_198;
  undefined ***local_190;
  undefined ***local_188;
  int iStack_17c;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  QString local_88 [8];
  undefined ***local_80;
  undefined8 local_78;
  undefined8 *local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_78 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStack_1f8;
  lpBuffer = (undefined ***)0x0;
  *param_2 = shared_null_exref;
  uVar17 = 1;
  local_1ac = 1;
  local_1af = false;
  local_190 = param_1;
  local_70 = param_2;
  QString::QString((QString *)&local_1a0);
  QString::QString((QString *)&local_198);
  thunk_FUN_1400236a0(param_1,local_88);
  local_1a8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1a8._4_4_,0x400);
  local_80 = (undefined ***)0x0;
  _Var6 = SUB168(ZEXT816(2) * ZEXT816(0x400),0);
  if (SUB168(ZEXT816(2) * ZEXT816(0x400),8) != 0) {
    _Var6 = 0xffffffffffffffff;
  }
  pppuVar7 = operator_new(_Var6);
  pppuVar13 = lpBuffer;
  if (pppuVar7 != (undefined ***)0x0) {
    local_80 = pppuVar7;
    thunk_FUN_1400025c2(0);
    pppuVar13 = pppuVar7;
  }
  cVar3 = GetUserNameExW(1,pppuVar13);
  DVar4 = GetLastError();
  if ((cVar3 == '\0') && (DVar4 == 0x534)) {
    local_1b0 = true;
  }
  else {
    local_1b0 = false;
    if (cVar3 == '\0') {
      uVar10 = thunk_FUN_140034660(local_1c0);
      cVar3 = thunk_FUN_140034220(uVar10,40000);
      thunk_FUN_140033080(local_1c0);
      if (cVar3 != '\0') {
        local_178 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        uVar17 = 5;
        local_1ac = 5;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        thunk_FUN_140010dc0(&local_170,2);
        pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140010590(&local_178,
                                      "GetUserNameEx failed for NameFullyQualifiedDN feature with error code: "
                                     );
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,DVar4);
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_1c0);
        local_1d8 = 0xb6;
        thunk_FUN_140033850(uVar11,40000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033080(local_1c0);
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
          FUN_1400025c2(uVar16);
        }
        thunk_FUN_140011330(local_f0);
LAB_14002431e:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
    else {
      pQVar9 = (QString *)QString::fromWCharArray((wchar_t *)&local_188,(int)pppuVar13);
      QString::operator=((QString *)&local_1a0,pQVar9);
      QString::~QString((QString *)&local_188);
      uVar10 = thunk_FUN_140034660(local_1c0);
      cVar3 = thunk_FUN_140034220(uVar10,20000);
      thunk_FUN_140033080(local_1c0);
      if (cVar3 != '\0') {
        local_178 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        uVar17 = 9;
        local_1ac = 9;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_188 = &local_170;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_108 = 0;
        local_100 = 4;
        thunk_FUN_140010590(&local_178,"AD info successfully fetched by GetUserNameEx");
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_1c0);
        local_1d8 = 0xbc;
        thunk_FUN_140033850(uVar11,20000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033080(local_1c0);
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
          FUN_1400025c2(uVar16);
        }
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_140012800(&local_170);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_168);
        goto LAB_14002431e;
      }
    }
  }
  local_1c8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1c8._4_4_,0x400);
  local_188 = (undefined ***)0x0;
  _Var6 = SUB168(ZEXT816(2) * ZEXT816(0x400),0);
  if (SUB168(ZEXT816(2) * ZEXT816(0x400),8) != 0) {
    _Var6 = 0xffffffffffffffff;
  }
  pppuVar8 = operator_new(_Var6);
  pppuVar7 = lpBuffer;
  if (pppuVar8 != (undefined ***)0x0) {
    local_188 = pppuVar8;
    thunk_FUN_1400025c2(0);
    pppuVar7 = pppuVar8;
  }
  cVar3 = GetUserNameExW(6,pppuVar7);
  if (cVar3 == '\0') {
    DVar4 = GetLastError();
    uVar10 = thunk_FUN_140034660(local_1c0);
    cVar3 = thunk_FUN_140034220(uVar10,40000);
    thunk_FUN_140033080(local_1c0);
    param_1 = local_190;
    if (cVar3 != '\0') {
      local_178 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_f0);
      uVar17 = uVar17 | 0x10;
      local_1ac = uVar17;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
           *(int *)(local_178 + 4) + -0x88;
      local_1c0[0] = &local_170;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_108 = 0;
      local_100 = 4;
      pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                thunk_FUN_140010590(&local_178,
                                    "GetUserNameEx failed for NameUniqueId feature with error code: "
                                   );
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,DVar4);
      uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
      uVar11 = thunk_FUN_140034660(local_1c0);
      local_1d8 = 0xcb;
      thunk_FUN_140033850(uVar11,40000,uVar10,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033080(local_1c0);
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
        FUN_1400025c2(uVar16);
      }
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
           *(int *)(local_178 + 4) + -0x88;
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012800(&local_170);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_168);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_f0);
      param_1 = local_190;
    }
  }
  else {
    pQVar9 = (QString *)QString::fromWCharArray((wchar_t *)&local_190,(int)pppuVar7);
    QString::operator=((QString *)&local_198,pQVar9);
    QString::~QString((QString *)&local_190);
  }
  thunk_FUN_1400025c2(pppuVar7);
  thunk_FUN_1400025c2(pppuVar13);
  bVar2 = local_1b0;
  if (local_1b0 == false) {
    if ((*(int *)(local_198 + 4) == 0) || (*(int *)(local_1a0 + 4) == 0)) {
      uVar10 = thunk_FUN_140034660(local_1c0);
      cVar3 = thunk_FUN_140034220(uVar10,30000);
      thunk_FUN_140033080(local_1c0);
      if (cVar3 != '\0') {
        local_178 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        uVar17 = uVar17 | 0x20;
        local_1ac = uVar17;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_1c0[0] = &local_170;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_108 = 0;
        local_100 = 4;
        thunk_FUN_140010590(&local_178,"failed to query AD info from AD, trying to get from LDAP");
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_1c0);
        local_1d8 = 0xd2;
        thunk_FUN_140033850(uVar11,30000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033080(local_1c0);
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
          FUN_1400025c2(uVar16);
        }
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_140012800(&local_170);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_168);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
      QString::QString((QString *)&local_1a8);
      QString::QString((QString *)&local_1c8);
      cVar3 = thunk_FUN_140025770(param_1,local_88,&local_1a8,&local_1c8);
      if (cVar3 != '\0') {
        if (*(int *)(local_1a0 + 4) == 0) {
          QString::operator=((QString *)&local_1a0,(QString *)&local_1a8);
        }
        if (*(int *)(local_198 + 4) == 0) {
          QString::operator=((QString *)&local_198,(QString *)&local_1c8);
        }
        uVar10 = thunk_FUN_140034660(local_1c0);
        cVar3 = thunk_FUN_140034220(uVar10,20000);
        thunk_FUN_140033080(local_1c0);
        if (cVar3 != '\0') {
          local_178 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_f0);
          uVar17 = uVar17 | 0x40;
          local_1ac = uVar17;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
          *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
               *(int *)(local_178 + 4) + -0x88;
          thunk_FUN_140010dc0(&local_170,2);
          thunk_FUN_140010590(&local_178,"AD info successfully fetched from LDAP");
          uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
          uVar11 = thunk_FUN_140034660(local_1c0);
          local_1d8 = 0xe2;
          thunk_FUN_140033850(uVar11,20000,uVar10,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033080(local_1c0);
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
            FUN_1400025c2(uVar16);
          }
          thunk_FUN_140011330(local_f0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_f0);
        }
      }
      QString::~QString((QString *)&local_1c8);
      QString::~QString((QString *)&local_1a8);
    }
    if ((*(int *)(local_198 + 4) == 0) || (*(int *)(local_1a0 + 4) == 0)) {
      uVar10 = thunk_FUN_140034660(local_1c0);
      cVar3 = thunk_FUN_140034220(uVar10,30000);
      thunk_FUN_140033080(local_1c0);
      if (cVar3 != '\0') {
        local_178 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        uVar17 = uVar17 | 0x80;
        local_1ac = uVar17;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_1c0[0] = &local_170;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_108 = 0;
        local_100 = 4;
        thunk_FUN_140010590(&local_178,
                            "failed to query AD info from LDAP, trying to get from registry");
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_1c0);
        local_1d8 = 0xea;
        thunk_FUN_140033850(uVar11,30000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033080(local_1c0);
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
          FUN_1400025c2(uVar16);
        }
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_140012800(&local_170);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_168);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
      QString::QString((QString *)&local_1a8);
      QString::QString((QString *)&local_1c8);
      cVar3 = thunk_FUN_1400269c0(param_1,local_88,&local_1a8,&local_1c8);
      if (cVar3 != '\0') {
        if (*(int *)(local_1a0 + 4) == 0) {
          QString::operator=((QString *)&local_1a0,(QString *)&local_1a8);
        }
        if (*(int *)(local_198 + 4) == 0) {
          QString::operator=((QString *)&local_198,(QString *)&local_1c8);
        }
        uVar10 = thunk_FUN_140034660(local_1c0);
        cVar3 = thunk_FUN_140034220(uVar10,20000);
        thunk_FUN_140033080(local_1c0);
        if (cVar3 != '\0') {
          local_178 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_f0);
          uVar17 = uVar17 | 0x100;
          local_1ac = uVar17;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
          *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
               *(int *)(local_178 + 4) + -0x88;
          thunk_FUN_140010dc0(&local_170,2);
          thunk_FUN_140010590(&local_178,"AD info successfully fetched from registry");
          uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
          uVar11 = thunk_FUN_140034660(local_1c0);
          local_1d8 = 0xfa;
          thunk_FUN_140033850(uVar11,20000,uVar10,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033080(local_1c0);
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
            FUN_1400025c2(uVar16);
          }
          thunk_FUN_140011330(local_f0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_f0);
        }
      }
      QString::~QString((QString *)&local_1c8);
      QString::~QString((QString *)&local_1a8);
    }
    if ((*(int *)(local_198 + 4) == 0) || (*(int *)(local_1a0 + 4) == 0)) {
      uVar10 = thunk_FUN_140034660(local_1c0);
      cVar3 = thunk_FUN_140034220(uVar10,40000);
      thunk_FUN_140033080(local_1c0);
      if (cVar3 != '\0') {
        thunk_FUN_140010c40(&local_178,2,1);
        thunk_FUN_140010590(&local_178,"Failed to obtain AD GUID and DN in anyway");
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_1c0);
        local_1d8 = 0x102;
        thunk_FUN_140033850(uVar11,40000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033080(local_1c0);
        thunk_FUN_1400113d0(local_68);
        thunk_FUN_140011330(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
      local_1af = true;
    }
    else {
      thunk_FUN_1400228d0(param_1,&local_1a0,param_2);
    }
    local_1c8 = QString::fromAscii_helper("fullName",8);
    uVar17 = uVar17 | 2;
    local_1ac = uVar17;
    lVar12 = thunk_FUN_140013c10(*param_2,&local_1c8);
    if (lVar12 != 0) {
      bVar1 = false;
      goto LAB_140024e4f;
    }
  }
  bVar1 = true;
LAB_140024e4f:
  if ((uVar17 & 2) != 0) {
    local_1ac = uVar17 & 0xfffffffd;
    QString::~QString((QString *)&local_1c8);
  }
  if (bVar1) {
    local_1c8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1c8._4_4_,0x400);
    local_190 = (undefined ***)0x0;
    _Var6 = SUB168(ZEXT816(2) * ZEXT816(0x400),0);
    if (SUB168(ZEXT816(2) * ZEXT816(0x400),8) != 0) {
      _Var6 = 0xffffffffffffffff;
    }
    pppuVar13 = operator_new(_Var6);
    if (pppuVar13 != (undefined ***)0x0) {
      local_190 = pppuVar13;
      thunk_FUN_1400025c2(0);
      lpBuffer = pppuVar13;
    }
    BVar5 = GetUserNameW((LPWSTR)lpBuffer,(LPDWORD)&local_1c8);
    if (BVar5 == 0) {
      uVar10 = thunk_FUN_140034660(local_1c0);
      cVar3 = thunk_FUN_140034220(uVar10,40000);
      thunk_FUN_140033080(local_1c0);
      if (cVar3 != '\0') {
        thunk_FUN_140010c40(&local_178,2,1);
        DVar4 = GetLastError();
        pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140010590(&local_178,"GetUserNameW failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,DVar4);
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_1c0);
        local_1d8 = 0x119;
        thunk_FUN_140033850(uVar11,40000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033080(local_1c0);
        thunk_FUN_1400113d0(local_68);
        thunk_FUN_140011330(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
    else {
      pQVar9 = (QString *)QString::fromWCharArray((wchar_t *)&local_188,(int)lpBuffer);
      QVariant::QVariant((QVariant *)local_1c0,pQVar9);
      local_1a8 = QString::fromAscii_helper("fullName",8);
      pQVar14 = (QVariant *)thunk_FUN_1400118f0(param_2,&local_1a8);
      QVariant::operator=(pQVar14,(QVariant *)local_1c0);
      QString::~QString((QString *)&local_1a8);
      QVariant::~QVariant((QVariant *)local_1c0);
      QString::~QString((QString *)&local_188);
    }
    thunk_FUN_1400025c2(lpBuffer);
  }
  if (*(int *)(local_1a0 + 4) != 0) {
    QVariant::QVariant((QVariant *)local_1c0,(QString *)&local_1a0);
    local_1c8 = QString::fromAscii_helper("distinguishedName",0x11);
    pQVar14 = (QVariant *)thunk_FUN_1400118f0(param_2,&local_1c8);
    QVariant::operator=(pQVar14,(QVariant *)local_1c0);
    QString::~QString((QString *)&local_1c8);
    QVariant::~QVariant((QVariant *)local_1c0);
  }
  if (*(int *)(local_198 + 4) != 0) {
    pQVar9 = (QString *)QString::toLower((QString *)&local_198);
    QVariant::QVariant((QVariant *)local_1c0,pQVar9);
    local_1c8 = QString::fromAscii_helper("GUID",4);
    pQVar14 = (QVariant *)thunk_FUN_1400118f0(param_2,&local_1c8);
    QVariant::operator=(pQVar14,(QVariant *)local_1c0);
    QString::~QString((QString *)&local_1c8);
    QVariant::~QVariant((QVariant *)local_1c0);
    QString::~QString((QString *)&local_190);
  }
  QVariant::QVariant((QVariant *)local_1c0,bVar2);
  local_1c8 = QString::fromAscii_helper("local",5);
  pQVar14 = (QVariant *)thunk_FUN_1400118f0(param_2,&local_1c8);
  QVariant::operator=(pQVar14,(QVariant *)local_1c0);
  QString::~QString((QString *)&local_1c8);
  QVariant::~QVariant((QVariant *)local_1c0);
  QVariant::QVariant((QVariant *)local_1c0,local_1af);
  local_1c8 = QString::fromAscii_helper("fail",4);
  pQVar14 = (QVariant *)thunk_FUN_1400118f0(param_2,&local_1c8);
  QVariant::operator=(pQVar14,(QVariant *)local_1c0);
  QString::~QString((QString *)&local_1c8);
  QVariant::~QVariant((QVariant *)local_1c0);
  pQVar9 = (QString *)QStandardPaths::writableLocation((StandardLocation)&local_190);
  QVariant::QVariant((QVariant *)local_1c0,pQVar9);
  local_1c8 = QString::fromAscii_helper("home",4);
  pQVar14 = (QVariant *)thunk_FUN_1400118f0(param_2,&local_1c8);
  QVariant::operator=(pQVar14,(QVariant *)local_1c0);
  QString::~QString((QString *)&local_1c8);
  QVariant::~QVariant((QVariant *)local_1c0);
  QString::~QString((QString *)&local_190);
  QString::~QString(local_88);
  QString::~QString((QString *)&local_198);
  QString::~QString((QString *)&local_1a0);
  return param_2;
}

// =============================================================================
// Ghidra: FUN_1400228d0 @ 0x1400228d0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1400228d0(longlong param_1,QString *param_2,undefined8 param_3)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  QString *pQVar4;
  QVariant *pQVar5;
  undefined8 uVar6;
  basic_ostream<char,std::char_traits<char>_> *pbVar7;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar8;
  undefined2 *puVar9;
  ulonglong uVar10;
  uint uVar11;
  uint uVar12;
  undefined1 auStack_1e8 [32];
  undefined4 local_1c8;
  undefined8 local_1b8;
  BSTR local_1b0;
  undefined ***local_1a8;
  undefined8 local_1a0;
  undefined *local_198;
  undefined **local_190;
  basic_ostream<char,std::char_traits<char>_> local_188 [96];
  undefined8 local_128;
  undefined4 local_120;
  basic_ios<char,std::char_traits<char>_> local_110 [104];
  undefined8 local_a8;
  undefined8 local_a0 [3];
  ulonglong local_88;
  ulonglong local_80 [3];
  ulonglong local_68;
  undefined2 local_60;
  undefined6 uStack_5e;
  undefined8 local_50;
  ulonglong local_48;
  ulonglong local_40;
  
  local_a8 = 0xfffffffffffffffe;
  local_40 = DAT_140763590 ^ (ulonglong)auStack_1e8;
  uVar11 = 0;
  local_1b8 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_1b8 & 0xffffffff00000000);
  if ((*(longlong *)(param_1 + 0x10) == 0) || (*(int *)(*(longlong *)param_2 + 4) == 0)) {
    uVar3 = thunk_FUN_140034660(local_a0);
    cVar1 = thunk_FUN_140034220(uVar3,40000);
    thunk_FUN_140033080(local_a0);
    if (cVar1 != '\0') {
      local_198 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_110);
      local_1b8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1b8._4_4_,0x10);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_198,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_190,false);
      *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
           *(int *)(local_198 + 4) + -0x88;
      local_1a8 = &local_190;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_190);
      local_190 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_128 = 0;
      local_120 = 4;
      iVar2 = *(int *)(*(longlong *)param_2 + 4);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010590(&local_198,"precondition check failed: ");
      pbVar8 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar7,*(void **)(param_1 + 0x10));
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010590(pbVar8,&DAT_1400de684);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,iVar2 == 0);
      uVar3 = thunk_FUN_140016ea0(&local_198,local_80);
      uVar6 = thunk_FUN_140034660(local_a0);
      local_1c8 = 0x98;
      thunk_FUN_140033850(uVar6,40000,uVar3,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033080(local_a0);
      if (0xf < local_68) {
        uVar10 = local_80[0];
        if (0xfff < local_68 + 1) {
          if ((local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_80[0] - 8);
          if (local_80[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_80[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_80[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c2(uVar10);
      }
      *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
           *(int *)(local_198 + 4) + -0x88;
      local_190 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012800(&local_190);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_190);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_188);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_110);
    }
  }
  else {
    local_1a0 = (longlong *)0x0;
    local_48 = 7;
    local_50 = 0;
    local_60 = 0;
    thunk_FUN_140022480(&local_60,L"LDAP://",7);
    uVar3 = QString::toStdWString(param_2);
    thunk_FUN_140022160(&local_60,uVar3,0,0xffffffffffffffff);
    if (7 < local_88) {
      thunk_FUN_140022700(local_a0,local_a0[0],local_88 + 1);
    }
    puVar9 = &local_60;
    if (7 < local_48) {
      puVar9 = (undefined2 *)CONCAT62(uStack_5e,local_60);
    }
    iVar2 = (**(code **)(param_1 + 0x10))(puVar9,&DAT_1400de258);
    if ((-1 < iVar2) && (local_1a0 != (longlong *)0x0)) {
      local_1b0 = (BSTR)0x0;
      iVar2 = (**(code **)(*local_1a0 + 0x120))(local_1a0,&local_1b0);
      if ((iVar2 < 0) || (local_1b0 == (BSTR)0x0)) {
        uVar3 = thunk_FUN_140034660(local_a0);
        cVar1 = thunk_FUN_140034220(uVar3,40000);
        thunk_FUN_140033080(local_a0);
        if (cVar1 != '\0') {
          local_198 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_110);
          uVar11 = 1;
          local_1b8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1b8._4_4_,1);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_198,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_190,false);
          *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
               *(int *)(local_198 + 4) + -0x88;
          thunk_FUN_140010dc0(&local_190,2);
          thunk_FUN_140010590(&local_198,"get_FirstName failed");
          uVar3 = thunk_FUN_140016ea0(&local_198,local_80);
          uVar6 = thunk_FUN_140034660(local_a0);
          local_1c8 = 0x79;
          thunk_FUN_140033850(uVar6,40000,uVar3,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033080(local_a0);
          if (0xf < local_68) {
            uVar10 = local_80[0];
            if (0xfff < local_68 + 1) {
              if ((local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar10 = *(ulonglong *)(local_80[0] - 8);
              if (local_80[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_80[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_80[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar10);
          }
          thunk_FUN_140011330(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        pQVar4 = (QString *)FUN_140022630(&local_1a8);
        QVariant::QVariant((QVariant *)local_a0,pQVar4);
        local_1b8 = QString::fromAscii_helper("firstName",9);
        pQVar5 = (QVariant *)thunk_FUN_1400118f0(param_3,&local_1b8);
        QVariant::operator=(pQVar5,(QVariant *)local_a0);
        QString::~QString((QString *)&local_1b8);
        QVariant::~QVariant((QVariant *)local_a0);
        QString::~QString((QString *)&local_1a8);
        SysFreeString(local_1b0);
        local_1b0 = (BSTR)0x0;
      }
      iVar2 = (**(code **)(*local_1a0 + 0x130))(local_1a0,&local_1b0);
      uVar12 = uVar11;
      if ((iVar2 < 0) || (local_1b0 == (BSTR)0x0)) {
        uVar3 = thunk_FUN_140034660(local_a0);
        cVar1 = thunk_FUN_140034220(uVar3,40000);
        thunk_FUN_140033080(local_a0);
        if (cVar1 != '\0') {
          local_198 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_110);
          uVar12 = uVar11 | 2;
          local_1b8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_1b8._4_4_,uVar11) | 2);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_198,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_190,false);
          *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
               *(int *)(local_198 + 4) + -0x88;
          thunk_FUN_140010dc0(&local_190,2);
          thunk_FUN_140010590(&local_198,"get_LastName failed");
          uVar3 = thunk_FUN_140016ea0(&local_198,local_80);
          uVar6 = thunk_FUN_140034660(local_a0);
          local_1c8 = 0x82;
          thunk_FUN_140033850(uVar6,40000,uVar3,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033080(local_a0);
          if (0xf < local_68) {
            uVar10 = local_80[0];
            if (0xfff < local_68 + 1) {
              if ((local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar10 = *(ulonglong *)(local_80[0] - 8);
              if (local_80[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_80[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_80[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar10);
          }
          thunk_FUN_140011330(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        pQVar4 = (QString *)FUN_140022630(&local_1a8);
        QVariant::QVariant((QVariant *)local_a0,pQVar4);
        local_1b8 = QString::fromAscii_helper("lastName",8);
        pQVar5 = (QVariant *)thunk_FUN_1400118f0(param_3,&local_1b8);
        QVariant::operator=(pQVar5,(QVariant *)local_a0);
        QString::~QString((QString *)&local_1b8);
        QVariant::~QVariant((QVariant *)local_a0);
        QString::~QString((QString *)&local_1a8);
        SysFreeString(local_1b0);
        local_1b0 = (BSTR)0x0;
      }
      iVar2 = (**(code **)(*local_1a0 + 0x110))(local_1a0,&local_1b0);
      if ((iVar2 < 0) || (local_1b0 == (BSTR)0x0)) {
        uVar3 = thunk_FUN_140034660(local_a0);
        cVar1 = thunk_FUN_140034220(uVar3,40000);
        thunk_FUN_140033080(local_a0);
        if (cVar1 != '\0') {
          local_198 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_110);
          local_1b8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_1b8._4_4_,uVar12) | 4);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_198,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_190,false);
          *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
               *(int *)(local_198 + 4) + -0x88;
          thunk_FUN_140010dc0(&local_190,2);
          thunk_FUN_140010590(&local_198,"get_FullName failed");
          uVar3 = thunk_FUN_140016ea0(&local_198,local_80);
          uVar6 = thunk_FUN_140034660(local_a0);
          local_1c8 = 0x8b;
          thunk_FUN_140033850(uVar6,40000,uVar3,
                              "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033080(local_a0);
          if (0xf < local_68) {
            uVar10 = local_80[0];
            if (0xfff < local_68 + 1) {
              if ((local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar10 = *(ulonglong *)(local_80[0] - 8);
              if (local_80[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_80[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_80[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c2(uVar10);
          }
          thunk_FUN_140011330(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        pQVar4 = (QString *)FUN_140022630(&local_1a8);
        QVariant::QVariant((QVariant *)local_a0,pQVar4);
        local_1b8 = QString::fromAscii_helper("fullName",8);
        pQVar5 = (QVariant *)thunk_FUN_1400118f0(param_3,&local_1b8);
        QVariant::operator=(pQVar5,(QVariant *)local_a0);
        QString::~QString((QString *)&local_1b8);
        QVariant::~QVariant((QVariant *)local_a0);
        QString::~QString((QString *)&local_1a8);
        SysFreeString(local_1b0);
        local_1b0 = (BSTR)0x0;
      }
      (**(code **)(*local_1a0 + 0x10))();
      if (7 < local_48) {
        thunk_FUN_140022700(&local_60,CONCAT62(uStack_5e,local_60),local_48 + 1);
      }
      return 1;
    }
    uVar3 = thunk_FUN_140034660(local_a0);
    cVar1 = thunk_FUN_140034220(uVar3,40000);
    thunk_FUN_140033080(local_a0);
    if (cVar1 != '\0') {
      local_198 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_110);
      local_1b8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1b8._4_4_,8);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_198,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_190,false);
      *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
           *(int *)(local_198 + 4) + -0x88;
      thunk_FUN_140010dc0(&local_190,2);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010590(&local_198,"ADsGetObject failed: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,iVar2);
      uVar3 = thunk_FUN_140016ea0(&local_198,local_80);
      uVar6 = thunk_FUN_140034660(local_a0);
      local_1c8 = 0x92;
      thunk_FUN_140033850(uVar6,40000,uVar3,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033080(local_a0);
      if (0xf < local_68) {
        uVar10 = local_80[0];
        if (0xfff < local_68 + 1) {
          if ((local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_80[0] - 8);
          if (local_80[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_80[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_80[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c2(uVar10);
      }
      thunk_FUN_140011330(local_110);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_110);
    }
    if (7 < local_48) {
      thunk_FUN_140022700(&local_60,CONCAT62(uStack_5e,local_60),local_48 + 1);
    }
  }
  return 0;
}

// =============================================================================
// Ghidra: FUN_14006f410 @ 0x14006f410
// =============================================================================
void FUN_14006f410(longlong param_1,int param_2)

{
  undefined8 uVar1;
  bool bVar2;
  QMimeData *this;
  QByteArray local_res8 [8];
  QByteArray local_res18 [16];
  QVariant local_40 [16];
  QImage local_30 [40];
  
  if (((*(byte *)(param_1 + 0x10) & 0x10) != 0) && (param_2 == 0)) {
    this = (QMimeData *)thunk_FUN_140076860(param_1 + 0x18);
    if (this != (QMimeData *)0x0) {
      bVar2 = QMimeData::hasUrls(this);
      if (bVar2) {
        uVar1 = QMimeData::urls(this);
        thunk_FUN_14006fe20(param_1,uVar1);
      }
      else {
        bVar2 = QMimeData::hasHtml(this);
        if (bVar2) {
          uVar1 = QMimeData::html(this);
          thunk_FUN_14006fce0(param_1,uVar1);
        }
        else {
          bVar2 = QMimeData::hasText(this);
          if (bVar2) {
            uVar1 = QMimeData::text(this);
            thunk_FUN_14006fdc0(param_1,uVar1);
          }
          else {
            bVar2 = QMimeData::hasImage(this);
            if (bVar2) {
              uVar1 = QMimeData::imageData(this);
              thunk_FUN_14006ec10(local_30,uVar1);
              QVariant::~QVariant(local_40);
              bVar2 = QImage::isNull(local_30);
              if (!bVar2) {
                QByteArray::QByteArray(local_res8);
                QBuffer::QBuffer((QBuffer *)local_40,local_res8,(QObject *)0x0);
                bVar2 = QBuffer::open((QBuffer *)local_40,2);
                if (!bVar2) {
                  qt_assert("!\"wtf?\"",
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave\\clipboard.cpp"
                            ,0xc3);
                    /* WARNING: Subroutine does not return */
                  abort();
                }
                bVar2 = QImage::save(local_30,(QIODevice *)local_40,"PNG",-1);
                if (!bVar2) {
                  qt_assert("!\"wtf?\"",
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave\\clipboard.cpp"
                            ,0xc9);
                    /* WARNING: Subroutine does not return */
                  abort();
                }
                QBuffer::close((QBuffer *)local_40);
                uVar1 = QByteArray::QByteArray(local_res18,local_res8);
                thunk_FUN_14006fd40(param_1,uVar1);
                QBuffer::~QBuffer((QBuffer *)local_40);
                QByteArray::~QByteArray(local_res8);
              }
              QImage::~QImage(local_30);
            }
            else {
              thunk_FUN_14006fda0(param_1);
            }
          }
        }
      }
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_14006f8f0 @ 0x14006f8f0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14006f8f0(QObject *param_1)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  undefined1 auStackY_198 [32];
  undefined1 *local_168;
  code *local_160 [2];
  undefined8 local_150;
  undefined *local_148;
  basic_streambuf<char,std::char_traits<char>_> local_140 [128];
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_150 = 0xfffffffffffffffe;
  local_38 = DAT_140763590 ^ (ulonglong)auStackY_198;
  local_168 = (undefined1 *)((ulonglong)local_168 & 0xffffffff00000000);
  if (((byte)param_1[0x10] & 1) != 0) {
    lVar2 = thunk_FUN_14006f2f0();
    if (lVar2 == 0) {
      uVar3 = thunk_FUN_140034660(local_160);
      cVar1 = thunk_FUN_140034220(uVar3,40000);
      thunk_FUN_140033080(local_160);
      if (cVar1 != '\0') {
        local_148 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_168 = (undefined1 *)CONCAT44(local_168._4_4_,1);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
        *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_140010dc0(local_140,2);
        thunk_FUN_140010590(&local_148,"memory fail");
        uVar3 = thunk_FUN_140016ea0(&local_148,local_58);
        uVar4 = thunk_FUN_140034660(local_160);
        thunk_FUN_140033850(uVar4,40000,uVar3,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave\\clipboard.cpp"
                           );
        thunk_FUN_140033080(local_160);
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
          FUN_1400025c2(uVar5);
        }
        thunk_FUN_140011330(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
    }
    else {
      local_168 = &LAB_1400040fc;
      local_160[0] = thunk_FUN_140076c30;
      QObject::disconnectImpl
                (param_1 + 0x18,local_160,param_1,&local_168,(QMetaObject *)&DAT_140763188);
      thunk_FUN_140076a00(param_1 + 0x18,lVar2,0);
      thunk_FUN_14006ead0(local_160,param_1 + 0x18,thunk_FUN_140076c30,param_1);
      QMetaObject::Connection::~Connection((Connection *)local_160);
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_140083580 @ 0x140083580
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140083580(longlong param_1)

{
  char cVar1;
  bool bVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  QByteArray *pQVar5;
  char *pcVar6;
  QJsonDocument *this;
  ulonglong uVar7;
  undefined1 auStack_1d8 [32];
  undefined4 local_1b8;
  QString local_1a8 [16];
  undefined4 local_198;
  QTypedArrayData<unsigned_short> *local_190;
  QTypedArrayData<unsigned_short> *local_188 [2];
  QString local_178 [12];
  int iStack_16c;
  undefined *local_168;
  basic_streambuf<char,std::char_traits<char>_> local_160 [128];
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  undefined8 local_78;
  ulonglong local_70 [3];
  ulonglong local_58;
  QString local_50 [8];
  QString local_48 [8];
  QJsonObject local_40 [24];
  ulonglong local_28;
  
  local_78 = 0xfffffffffffffffe;
  local_28 = DAT_140763590 ^ (ulonglong)auStack_1d8;
  local_198 = 0;
  if (*(int *)(*(longlong *)(param_1 + 0x68) + 4) == 0) {
    uVar3 = thunk_FUN_140082b70();
    local_188[0] = QString::fromAscii_helper("subAgent",8);
    cVar1 = thunk_FUN_1400b9fa0(uVar3,local_188);
    QString::~QString((QString *)local_188);
    if (cVar1 != '\0') {
      uVar3 = thunk_FUN_140034660(local_1a8);
      cVar1 = thunk_FUN_140034220(uVar3,10000);
      thunk_FUN_140033080(local_1a8);
      if (cVar1 != '\0') {
        local_168 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_e0);
        local_198 = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
        *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
             *(int *)(local_168 + 4) + -0x88;
        thunk_FUN_140010dc0(local_160,2);
        thunk_FUN_140010590(&local_168,"web token is empty await some");
        uVar3 = thunk_FUN_140016ea0(&local_168,local_70);
        uVar4 = thunk_FUN_140034660(local_1a8);
        local_1b8 = 0x9e;
        thunk_FUN_140033850(uVar4,10000,uVar3,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033080(local_1a8);
        if (0xf < local_58) {
          uVar7 = local_70[0];
          if (0xfff < local_58 + 1) {
            if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_70[0] - 8);
            if (local_70[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_70[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_70[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c2(uVar7);
        }
        thunk_FUN_140011330(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    thunk_FUN_14007fea0(param_1 + 0x98,local_50);
    QJsonObject::~QJsonObject(local_40);
    QString::~QString(local_48);
    QString::~QString(local_50);
    thunk_FUN_140082ea0(param_1);
    thunk_FUN_1400852b0(param_1,0);
  }
  else if (*(int *)(*(longlong *)(param_1 + 0x70) + 4) == 0) {
    uVar3 = thunk_FUN_140082b70();
    cVar1 = thunk_FUN_140084f40(uVar3);
    if (cVar1 != '\0') {
      uVar3 = thunk_FUN_140034660(local_1a8);
      cVar1 = thunk_FUN_140034220(uVar3,10000);
      thunk_FUN_140033080(local_1a8);
      if (cVar1 != '\0') {
        local_168 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_e0);
        local_198 = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
        *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
             *(int *)(local_168 + 4) + -0x88;
        thunk_FUN_140010dc0(local_160,2);
        thunk_FUN_140010590(&local_168,"web interface link is empty await some");
        uVar3 = thunk_FUN_140016ea0(&local_168,local_70);
        uVar4 = thunk_FUN_140034660(local_1a8);
        local_1b8 = 0xa6;
        thunk_FUN_140033850(uVar4,10000,uVar3,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033080(local_1a8);
        if (0xf < local_58) {
          uVar7 = local_70[0];
          if (0xfff < local_58 + 1) {
            if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_70[0] - 8);
            if (local_70[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_70[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_70[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c2(uVar7);
        }
        thunk_FUN_140011330(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    thunk_FUN_14007fea0(param_1 + 0x98,local_50);
    QJsonObject::~QJsonObject(local_40);
    QString::~QString(local_48);
    QString::~QString(local_50);
    thunk_FUN_140083200(param_1);
    thunk_FUN_1400852b0(param_1,0);
  }
  else {
    if (*(int *)(param_1 + 0xc0) == 1) {
      bVar2 = QListData::isEmpty((QListData *)(param_1 + 0x98));
      if (!bVar2) {
        uVar3 = thunk_FUN_14007fea0(param_1 + 0x98,local_50);
        thunk_FUN_14007e6f0(param_1 + 0xa0,uVar3);
        QJsonObject::~QJsonObject(local_40);
        QString::~QString(local_48);
        QString::~QString(local_50);
        bVar2 = QString::operator==((QString *)(param_1 + 0xa0),"get");
        if (bVar2) {
          uVar3 = thunk_FUN_140011a00(local_1a8,param_1 + 0x70,param_1 + 0xa8);
          uVar3 = thunk_FUN_140011a70(local_70,uVar3,"?web_token=");
          thunk_FUN_140011a00(local_178,uVar3);
          QString::~QString((QString *)local_70);
          QString::~QString(local_1a8);
          uVar3 = thunk_FUN_140082b70();
          cVar1 = thunk_FUN_140084f40(uVar3);
          if (cVar1 != '\0') {
            uVar3 = thunk_FUN_140034660(local_188);
            cVar1 = thunk_FUN_140034220(uVar3,10000);
            thunk_FUN_140033080(local_188);
            if (cVar1 != '\0') {
              local_168 = &DAT_1400dbe28;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_e0);
              local_198 = 0x10;
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
              *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
                   *(int *)(local_168 + 4) + -0x88;
              thunk_FUN_140010dc0(local_160,2);
              uVar3 = thunk_FUN_140010590(&local_168,"subAgent send http GET: ");
              pQVar5 = (QByteArray *)QString::toLocal8Bit(local_178);
              pcVar6 = QByteArray::data(pQVar5);
              thunk_FUN_140010590(uVar3,pcVar6);
              QByteArray::~QByteArray((QByteArray *)local_188);
              uVar3 = thunk_FUN_140016ea0(&local_168,local_50);
              uVar4 = thunk_FUN_140034660(local_188);
              local_1b8 = 0xbb;
              thunk_FUN_140033850(uVar4,10000,uVar3,
                                  "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                                 );
              thunk_FUN_140033080(local_188);
              thunk_FUN_1400113d0(local_50);
              thunk_FUN_140011330(local_e0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_e0);
            }
          }
          QUrl::QUrl((QUrl *)local_188,local_178,0);
          QNetworkRequest::QNetworkRequest((QNetworkRequest *)&local_190,(QUrl *)local_188);
          QUrl::~QUrl((QUrl *)local_188);
          QNetworkAccessManager::get
                    ((QNetworkAccessManager *)(param_1 + 0x10),(QNetworkRequest *)&local_190);
          QNetworkRequest::~QNetworkRequest((QNetworkRequest *)&local_190);
          QString::~QString(local_178);
          return;
        }
        bVar2 = QString::operator==((QString *)(param_1 + 0xa0),"post");
        if (bVar2) {
          QJsonValue::QJsonValue((QJsonValue *)local_70,(QString *)(param_1 + 0x90));
          local_190 = QString::fromAscii_helper("skp_version",0xb);
          QJsonObject::insert((QJsonObject *)(param_1 + 0xb0),local_1a8,(QJsonValue *)&local_190);
          QString::~QString((QString *)&local_190);
          QJsonValue::~QJsonValue((QJsonValue *)local_70);
          QJsonValue::QJsonValue((QJsonValue *)local_70,(QString *)(param_1 + 0x68));
          local_190 = QString::fromAscii_helper("web_token",9);
          QJsonObject::insert((QJsonObject *)(param_1 + 0xb0),local_1a8,(QJsonValue *)&local_190);
          QString::~QString((QString *)&local_190);
          QJsonValue::~QJsonValue((QJsonValue *)local_70);
          thunk_FUN_140011a00(local_188,param_1 + 0x70);
          uVar3 = thunk_FUN_140082b70();
          cVar1 = thunk_FUN_140084f40(uVar3);
          if (cVar1 != '\0') {
            uVar3 = thunk_FUN_140034660(local_1a8);
            cVar1 = thunk_FUN_140034220(uVar3,10000);
            thunk_FUN_140033080(local_1a8);
            if (cVar1 != '\0') {
              local_168 = &DAT_1400dbe28;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_e0);
              local_198 = 0x20;
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
              *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
                   *(int *)(local_168 + 4) + -0x88;
              thunk_FUN_140010dc0(local_160,2);
              QVariant::QVariant((QVariant *)local_70,(QJsonObject *)(param_1 + 0xb0));
              uVar3 = thunk_FUN_140010590(&local_168,"subAgent send http POST: ");
              pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)local_188);
              pcVar6 = QByteArray::data(pQVar5);
              thunk_FUN_140010590(uVar3,pcVar6);
              QByteArray::~QByteArray((QByteArray *)local_1a8);
              uVar3 = thunk_FUN_140010590(uVar3,", payload: ");
              thunk_FUN_14007edc0(uVar3,local_70);
              QVariant::~QVariant((QVariant *)local_70);
              uVar3 = thunk_FUN_140016ea0(&local_168,local_50);
              uVar4 = thunk_FUN_140034660(local_1a8);
              local_1b8 = 0xc4;
              thunk_FUN_140033850(uVar4,10000,uVar3,
                                  "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                                 );
              thunk_FUN_140033080(local_1a8);
              thunk_FUN_1400113d0(local_50);
              thunk_FUN_140011330(local_e0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_e0);
            }
          }
          QUrl::QUrl((QUrl *)&local_190,(QString *)local_188,0);
          QNetworkRequest::QNetworkRequest((QNetworkRequest *)local_178,(QUrl *)&local_190);
          QUrl::~QUrl((QUrl *)&local_190);
          QVariant::QVariant((QVariant *)local_1a8,"application/json");
          QNetworkRequest::setHeader((QNetworkRequest *)local_178,0,(QVariant *)local_1a8);
          QVariant::~QVariant((QVariant *)local_1a8);
          this = (QJsonDocument *)
                 QJsonDocument::QJsonDocument
                           ((QJsonDocument *)local_70,(QJsonObject *)(param_1 + 0xb0));
          pQVar5 = (QByteArray *)QJsonDocument::toJson(this,(JsonFormat)local_1a8);
          QNetworkAccessManager::post
                    ((QNetworkAccessManager *)(param_1 + 0x10),(QNetworkRequest *)local_178,pQVar5);
          QByteArray::~QByteArray((QByteArray *)local_1a8);
          QJsonDocument::~QJsonDocument((QJsonDocument *)local_70);
          QNetworkRequest::~QNetworkRequest((QNetworkRequest *)local_178);
          QString::~QString((QString *)local_188);
          return;
        }
        qt_assert("!\"never here\"",
                  "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                  ,0xca);
        return;
      }
      uVar3 = thunk_FUN_140082b70();
      cVar1 = thunk_FUN_140084f40(uVar3);
      if (cVar1 == '\0') {
        return;
      }
      uVar3 = thunk_FUN_140034660(local_1a8);
      cVar1 = thunk_FUN_140034220(uVar3,10000);
      thunk_FUN_140033080(local_1a8);
      if (cVar1 == '\0') {
        return;
      }
      local_168 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_e0);
      local_198 = 8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
      *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
           *(int *)(local_168 + 4) + -0x88;
      thunk_FUN_140010dc0(local_160,2);
      thunk_FUN_140010590(&local_168,"nothing to send");
      uVar3 = thunk_FUN_140016ea0(&local_168,local_70);
      uVar4 = thunk_FUN_140034660(local_1a8);
      local_1b8 = 0xb3;
      thunk_FUN_140033850(uVar4,10000,uVar3,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033080(local_1a8);
      thunk_FUN_1400113d0(local_70);
    }
    else {
      uVar3 = thunk_FUN_140082b70();
      cVar1 = thunk_FUN_140084f40(uVar3);
      if (cVar1 == '\0') {
        return;
      }
      uVar3 = thunk_FUN_140034660(local_1a8);
      cVar1 = thunk_FUN_140034220(uVar3,10000);
      thunk_FUN_140033080(local_1a8);
      if (cVar1 == '\0') {
        return;
      }
      local_168 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_e0);
      local_198 = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
      *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
           *(int *)(local_168 + 4) + -0x88;
      thunk_FUN_140010dc0(local_160,2);
      uVar3 = thunk_FUN_140010590(&local_168,"request in process method: ");
      pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0xa0));
      pcVar6 = QByteArray::data(pQVar5);
      thunk_FUN_140010590(uVar3,pcVar6);
      QByteArray::~QByteArray((QByteArray *)&local_190);
      uVar3 = thunk_FUN_140010590(uVar3," postfix: ");
      pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0xa8));
      pcVar6 = QByteArray::data(pQVar5);
      thunk_FUN_140010590(uVar3,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_188);
      uVar3 = thunk_FUN_140016ea0(&local_168,local_70);
      uVar4 = thunk_FUN_140034660(local_1a8);
      local_1b8 = 0xae;
      thunk_FUN_140033850(uVar4,10000,uVar3,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033080(local_1a8);
      if (0xf < local_58) {
        uVar7 = local_70[0];
        if (0xfff < local_58 + 1) {
          if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_70[0] - 8);
          if (local_70[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_70[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_70[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c2(uVar7);
      }
    }
    thunk_FUN_140011330(local_e0);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_e0)
    ;
  }
  return;
}

// =============================================================================
// Ghidra: FUN_1400875d0 @ 0x1400875d0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QObject * FUN_1400875d0(QObject *param_1)

{
  longlong *plVar1;
  QObject *pQVar2;
  char cVar3;
  int iVar4;
  Language LVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  longlong *plVar8;
  QLocale *this;
  QString *pQVar9;
  QIcon *this_00;
  ulonglong uVar10;
  undefined4 uVar11;
  undefined1 auStack_1c8 [32];
  QIcon **local_1a8;
  undefined4 local_1a0;
  QIcon *local_198;
  undefined4 uStack_190;
  undefined8 local_188;
  QTypedArrayData<unsigned_short> *local_180;
  undefined ***local_178;
  QObject *local_170;
  QObject *local_168;
  int iStack_15c;
  undefined *local_158;
  undefined **local_150;
  basic_ostream<char,std::char_traits<char>_> local_148 [96];
  undefined8 local_e8;
  undefined4 local_e0;
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  undefined8 local_68;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_68 = 0xfffffffffffffffe;
  local_38 = DAT_140763590 ^ (ulonglong)auStack_1c8;
  uVar11 = 0;
  local_188 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_188 & 0xffffffff00000000);
  local_170 = param_1;
  local_168 = param_1;
  QObject::QObject(param_1,(QObject *)0x0);
  *(undefined ***)param_1 = grabberSubAgent::ui::IManager::vftable;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined2 *)(param_1 + 0x20) = 0;
  thunk_FUN_140094540(param_1 + 0x28);
  *(undefined ***)param_1 = grabberSubAgent::ui::Manager::vftable;
  *(undefined ***)(param_1 + 0x28) = grabberSubAgent::ui::Manager::vftable;
  QSystemTrayIcon::QSystemTrayIcon((QSystemTrayIcon *)(param_1 + 0xa0),param_1);
  QMenu::QMenu((QMenu *)(param_1 + 0xb0),(QWidget *)0x0);
  *(code **)(param_1 + 0xe0) = shared_null_exref;
  QIcon::QIcon((QIcon *)(param_1 + 0xe8));
  *(undefined8 *)(param_1 + 0xf0) = 0;
  *(undefined8 *)(param_1 + 0xf8) = 0;
  *(undefined8 *)(param_1 + 0x100) = 0;
  *(QObject **)(param_1 + 0x108) = param_1 + 0x68;
  local_180 = QString::fromAscii_helper("gsa",3);
  iVar4 = thunk_FUN_140094dc0(param_1 + 0x28,&local_180);
  QString::~QString((QString *)&local_180);
  param_1[0x20] = (QObject)(iVar4 == 0);
  if (param_1[0x20] == (QObject)0x0) {
    uVar6 = thunk_FUN_140034660(&local_198);
    cVar3 = thunk_FUN_140034220(uVar6,40000);
    thunk_FUN_140033080(&local_198);
    if (cVar3 == '\0') {
      return param_1;
    }
    local_158 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_d0);
    local_188 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_188._4_4_,0x10);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
    *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    local_198 = (QIcon *)&local_150;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
    local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_e8 = 0;
    local_e0 = 4;
    thunk_FUN_140010590(&local_158,"agent ui cannot be initialized");
    uVar6 = thunk_FUN_140016ea0(&local_158,local_58);
    uVar7 = thunk_FUN_140034660(&local_198);
    local_1a8 = (QIcon **)CONCAT44(local_1a8._4_4_,99);
    thunk_FUN_140033850(uVar7,40000,uVar6,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\manager.cpp"
                       );
    thunk_FUN_140033080(&local_198);
    if (0xf < local_40) {
      uVar10 = local_58[0];
      if (0xfff < local_40 + 1) {
        if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_58[0] - 8);
        if (local_58[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_58[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_58[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c2(uVar10);
    }
    *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012800(&local_150);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_148);
  }
  else {
    uVar6 = thunk_FUN_140034660(&local_198);
    cVar3 = thunk_FUN_140034220(uVar6,20000);
    thunk_FUN_140033080(&local_198);
    if (cVar3 != '\0') {
      local_158 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      uVar11 = 4;
      local_188 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_188._4_4_,4);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_178 = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      thunk_FUN_140010590(&local_158,"agent ui initialized");
      uVar6 = thunk_FUN_140016ea0(&local_158,local_58);
      uVar7 = thunk_FUN_140034660(&local_198);
      local_1a8 = (QIcon **)CONCAT44(local_1a8._4_4_,0x47);
      thunk_FUN_140033850(uVar7,20000,uVar6,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\manager.cpp"
                         );
      thunk_FUN_140033080(&local_198);
      if (0xf < local_40) {
        uVar10 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c2(uVar10);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012800(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
    local_178 = operator_new(0x88);
    plVar8 = (longlong *)0x0;
    if (local_178 != (undefined ***)0x0) {
      plVar8 = (longlong *)thunk_FUN_140089c10(local_178);
    }
    plVar1 = *(longlong **)(param_1 + 0xf0);
    if ((plVar1 != plVar8) && (*(longlong **)(param_1 + 0xf0) = plVar8, plVar1 != (longlong *)0x0))
    {
      (**(code **)(*plVar1 + 0x18))(plVar1,1);
    }
    if (*(longlong *)(param_1 + 0xf0) == 0) {
      qt_assert("d","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qscopedpointer.h",
                0x70);
    }
    cVar3 = thunk_FUN_14008b030(*(undefined8 *)(param_1 + 0xf0));
    if (cVar3 != '\0') {
      plVar8 = (longlong *)(*(longlong *)(param_1 + 0xf0) + 0x10);
      if (*(longlong *)(param_1 + 0xf0) == 0) {
        plVar8 = (longlong *)0x0;
      }
      thunk_FUN_1400ab310(plVar8);
      if (*(longlong *)(param_1 + 0xf0) == 0) {
        qt_assert("d",
                  "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qscopedpointer.h",
                  0x70);
      }
      thunk_FUN_14008b040(*(undefined8 *)(param_1 + 0xf0),&local_180);
      if (*(int *)(local_180 + 4) == 0) {
        this = (QLocale *)QLocale::system();
        LVar5 = QLocale::language(this);
        pQVar9 = (QString *)thunk_FUN_140092c90(param_1 + 0x68,&local_188,LVar5);
        QString::operator=((QString *)&local_180,pQVar9);
        QString::~QString((QString *)&local_188);
        QLocale::~QLocale((QLocale *)&local_178);
      }
      thunk_FUN_140092fd0(param_1 + 0x68,&local_180);
      local_188 = QString::fromAscii_helper(":/images/selfcontrol_icon.ico",0x1d);
      this_00 = (QIcon *)QIcon::QIcon((QIcon *)&local_178,(QString *)&local_188);
      pQVar2 = local_168;
      local_198 = this_00;
      QIcon::operator=((QIcon *)(local_168 + 0xe8),this_00);
      QApplication::setWindowIcon((QIcon *)(pQVar2 + 0xe8));
      QIcon::~QIcon(this_00);
      QString::~QString((QString *)&local_188);
      thunk_FUN_140088910(param_1);
      QSystemTrayIcon::setIcon((QSystemTrayIcon *)(param_1 + 0xa0),(QIcon *)(param_1 + 0xe8));
      QSystemTrayIcon::setContextMenu((QSystemTrayIcon *)(param_1 + 0xa0),(QMenu *)(param_1 + 0xb0))
      ;
      local_198 = (QIcon *)thunk_FUN_140089190;
      uStack_190 = 0;
      local_1a0 = 0;
      local_1a8 = &local_198;
      thunk_FUN_140087280(&local_168,param_1 + 0x68,thunk_FUN_140093d70,param_1);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      QString::~QString((QString *)&local_180);
      return param_1;
    }
    param_1[0x20] = (QObject)0x0;
    uVar6 = thunk_FUN_140034660(&local_198);
    cVar3 = thunk_FUN_140034220(uVar6,40000);
    thunk_FUN_140033080(&local_198);
    if (cVar3 == '\0') {
      return param_1;
    }
    local_158 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_d0);
    local_188 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_188._4_4_,uVar11) | 8);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
    *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    thunk_FUN_140010dc0(&local_150,2);
    thunk_FUN_140010590(&local_158,"unable to open settings database");
    uVar6 = thunk_FUN_140016ea0(&local_158,local_58);
    uVar7 = thunk_FUN_140034660(&local_198);
    local_1a8 = (QIcon **)CONCAT44(local_1a8._4_4_,0x4d);
    thunk_FUN_140033850(uVar7,40000,uVar6,
                        "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\manager.cpp"
                       );
    thunk_FUN_140033080(&local_198);
    if (0xf < local_40) {
      uVar10 = local_58[0];
      if (0xfff < local_40 + 1) {
        if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_58[0] - 8);
        if (local_58[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_58[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_58[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c2(uVar10);
    }
    thunk_FUN_140011330(local_d0);
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_d0);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_140091870 @ 0x140091870
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_140091870(QAbstractItemModel *param_1,int param_2,undefined8 param_3,QModelIndex *param_4)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulonglong uVar4;
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
  local_28 = DAT_140763590 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (param_2 < 0) {
    uVar2 = thunk_FUN_140034660(local_150);
    cVar1 = thunk_FUN_140034220(uVar2,40000);
    thunk_FUN_140033080(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_1400dbe28;
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
      thunk_FUN_140010590(&local_138,"notifications model: invalid row requested");
      uVar2 = thunk_FUN_140016ea0(&local_138,local_48);
      uVar3 = thunk_FUN_140034660(local_150);
      local_168 = 0x66;
      thunk_FUN_140033850(uVar3,40000,uVar2,
                          "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-notifications\\model.cpp"
                         );
      thunk_FUN_140033080(local_150);
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
        FUN_1400025c2(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012800(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    uVar2 = 0;
  }
  else {
    QAbstractItemModel::beginInsertRows(param_1,param_4,param_2,param_2);
    thunk_FUN_140091670(param_1 + 0x38,param_2,param_1 + 0x18);
    QAbstractItemModel::endInsertRows(param_1);
    thunk_FUN_14008fae0(param_1);
    uVar2 = 1;
  }
  return uVar2;
}

// =============================================================================
// Ghidra: FUN_1400a1e40 @ 0x1400a1e40
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400a1e40(undefined8 param_1,QJsonObject *param_2)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  QMapDataBase *pQVar5;
  ulonglong *puVar6;
  QVariant *pQVar7;
  QMapDataBase *pQVar8;
  QMapDataBase *pQVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  ulonglong uVar12;
  basic_ios<char,std::char_traits<char>_> *this;
  QMapDataBase *pQVar13;
  undefined1 auStack_368 [32];
  undefined4 local_348;
  QMapDataBase *local_338;
  QMapDataBase *local_330;
  QTypedArrayData<unsigned_short> *local_328;
  QTypedArrayData<unsigned_short> *local_320;
  undefined4 local_318;
  QTypedArrayData<unsigned_short> *local_310;
  QTypedArrayData<unsigned_short> *local_308;
  longlong local_300;
  QJsonObject *local_2f8;
  QString local_2f0 [8];
  undefined8 local_2e8;
  undefined8 local_2e0;
  QJsonObject *local_2d8;
  QVariant local_2d0 [16];
  QVariant local_2c0 [16];
  undefined1 local_2b0 [16];
  undefined1 local_2a0 [16];
  undefined1 local_290 [16];
  undefined1 local_280 [20];
  int iStack_26c;
  undefined *local_268;
  basic_streambuf<char,std::char_traits<char>_> local_260 [128];
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
  
  local_2e0 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStack_368;
  pQVar9 = (QMapDataBase *)0x0;
  local_318 = 0;
  local_2f8 = param_2;
  local_2e8 = param_1;
  local_2d8 = param_2;
  QJsonObject::toVariantMap(param_2);
  local_310 = QString::fromAscii_helper("data",4);
  pQVar5 = *(QMapDataBase **)(local_338 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a1f0d:
    pQVar8 = pQVar9;
  }
  else {
    do {
      bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_310);
      if (bVar2) {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 0x10);
      }
      else {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 8);
        pQVar8 = pQVar5;
      }
      pQVar5 = pQVar13;
    } while (pQVar13 != (QMapDataBase *)0x0);
    if ((pQVar8 == (QMapDataBase *)0x0) ||
       (bVar2 = operator<((QString *)&local_310,(QString *)(pQVar8 + 0x18)), bVar2))
    goto LAB_1400a1f0d;
  }
  QString::~QString((QString *)&local_310);
  if (pQVar8 == (QMapDataBase *)0x0) {
    if (*(uint *)local_338 != 0) {
      if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a27d2;
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 != 1) goto LAB_1400a27d2;
    }
    thunk_FUN_140013430(local_338);
    goto LAB_1400a27d2;
  }
  local_328 = QString::fromAscii_helper("data",4);
  if (1 < *(uint *)local_338) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_338 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_338 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_338 == 0) {
LAB_1400a1fc1:
      thunk_FUN_140013430(local_338);
    }
    else if (*(uint *)local_338 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a1fc1;
    }
    local_338 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = *(QMapDataBase **)(local_338 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a2031:
    pQVar7 = (QVariant *)QVariant::QVariant(local_2d0);
    if (1 < *(uint *)local_338) {
      pQVar5 = QMapDataBase::createData();
      if (*(longlong *)(local_338 + 0x10) != 0) {
        puVar6 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_338 + 0x10),pQVar5);
        *(ulonglong **)(pQVar5 + 0x10) = puVar6;
        *puVar6 = *puVar6 & 3;
        *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
      }
      if (*(uint *)local_338 == 0) {
LAB_1400a2099:
        thunk_FUN_140013430(local_338);
      }
      else if (*(uint *)local_338 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_338;
        *(uint *)local_338 = *(uint *)local_338 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400a2099;
      }
      local_338 = pQVar5;
      QMapDataBase::recalcMostLeftNode(pQVar5);
    }
    pQVar5 = local_338 + 8;
    bVar2 = true;
    pQVar13 = *(QMapDataBase **)(local_338 + 0x10);
    pQVar8 = pQVar9;
    if (*(QMapDataBase **)(local_338 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400a211e:
      local_348 = CONCAT31(local_348._1_3_,bVar2);
      pQVar8 = (QMapDataBase *)thunk_FUN_140013070(local_338,&local_328,pQVar7,pQVar5);
    }
    else {
      do {
        pQVar5 = pQVar13;
        bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_328);
        if (bVar2) {
          pQVar13 = *(QMapDataBase **)(pQVar5 + 0x10);
        }
        else {
          pQVar13 = *(QMapDataBase **)(pQVar5 + 8);
          pQVar8 = pQVar5;
        }
        bVar2 = !bVar2;
      } while (pQVar13 != (QMapDataBase *)0x0);
      if ((pQVar8 == (QMapDataBase *)0x0) ||
         (bVar3 = operator<((QString *)&local_328,(QString *)(pQVar8 + 0x18)), bVar3))
      goto LAB_1400a211e;
      QVariant::operator=((QVariant *)(pQVar8 + 0x20),pQVar7);
    }
    QVariant::~QVariant(local_2d0);
    param_2 = local_2f8;
  }
  else {
    do {
      bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_328);
      if (bVar2) {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 0x10);
      }
      else {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 8);
        pQVar8 = pQVar5;
      }
      pQVar5 = pQVar13;
    } while (pQVar13 != (QMapDataBase *)0x0);
    if ((pQVar8 == (QMapDataBase *)0x0) ||
       (bVar2 = operator<((QString *)&local_328,(QString *)(pQVar8 + 0x18)), bVar2))
    goto LAB_1400a2031;
  }
  QVariant::toMap((QVariant *)(pQVar8 + 0x20));
  QString::~QString((QString *)&local_328);
  local_308 = QString::fromAscii_helper("logo",4);
  pQVar5 = *(QMapDataBase **)(local_330 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a21cd:
    pQVar8 = pQVar9;
  }
  else {
    do {
      bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_308);
      if (bVar2) {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 0x10);
      }
      else {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 8);
        pQVar8 = pQVar5;
      }
      pQVar5 = pQVar13;
    } while (pQVar13 != (QMapDataBase *)0x0);
    if ((pQVar8 == (QMapDataBase *)0x0) ||
       (bVar2 = operator<((QString *)&local_308,(QString *)(pQVar8 + 0x18)), bVar2))
    goto LAB_1400a21cd;
  }
  QString::~QString((QString *)&local_308);
  if (pQVar8 == (QMapDataBase *)0x0) {
    if (*(uint *)local_330 == 0) {
LAB_1400a2200:
      thunk_FUN_140013430(local_330);
    }
    else if (*(uint *)local_330 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_330;
      *(uint *)local_330 = *(uint *)local_330 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2200;
    }
    if (*(uint *)local_338 != 0) {
      if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a27d2;
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 != 1) goto LAB_1400a27d2;
    }
    thunk_FUN_140013430(local_338);
    goto LAB_1400a27d2;
  }
  local_320 = QString::fromAscii_helper("logo",4);
  if (1 < *(uint *)local_330) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_330 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_330 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_330 == 0) {
LAB_1400a22a1:
      thunk_FUN_140013430(local_330);
    }
    else if (*(uint *)local_330 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_330;
      *(uint *)local_330 = *(uint *)local_330 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a22a1;
    }
    local_330 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = *(QMapDataBase **)(local_330 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a2311:
    pQVar7 = (QVariant *)QVariant::QVariant(local_2c0);
    if (1 < *(uint *)local_330) {
      pQVar5 = QMapDataBase::createData();
      if (*(longlong *)(local_330 + 0x10) != 0) {
        puVar6 = (ulonglong *)thunk_FUN_140012e80(*(longlong *)(local_330 + 0x10),pQVar5);
        *(ulonglong **)(pQVar5 + 0x10) = puVar6;
        *puVar6 = *puVar6 & 3;
        *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
      }
      if (*(uint *)local_330 == 0) {
LAB_1400a2379:
        thunk_FUN_140013430(local_330);
      }
      else if (*(uint *)local_330 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_330;
        *(uint *)local_330 = *(uint *)local_330 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400a2379;
      }
      local_330 = pQVar5;
      QMapDataBase::recalcMostLeftNode(pQVar5);
    }
    pQVar5 = local_330 + 8;
    bVar2 = true;
    pQVar8 = *(QMapDataBase **)(local_330 + 0x10);
    if (*(QMapDataBase **)(local_330 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400a23fb:
      local_348 = CONCAT31(local_348._1_3_,bVar2);
      pQVar9 = (QMapDataBase *)thunk_FUN_140013070(local_330,&local_320,pQVar7,pQVar5);
    }
    else {
      do {
        pQVar5 = pQVar8;
        bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_320);
        if (bVar2) {
          pQVar8 = *(QMapDataBase **)(pQVar5 + 0x10);
        }
        else {
          pQVar8 = *(QMapDataBase **)(pQVar5 + 8);
          pQVar9 = pQVar5;
        }
        bVar2 = !bVar2;
      } while (pQVar8 != (QMapDataBase *)0x0);
      if ((pQVar9 == (QMapDataBase *)0x0) ||
         (bVar3 = operator<((QString *)&local_320,(QString *)(pQVar9 + 0x18)), bVar3))
      goto LAB_1400a23fb;
      QVariant::operator=((QVariant *)(pQVar9 + 0x20),pQVar7);
    }
    QVariant::~QVariant(local_2c0);
    pQVar8 = pQVar9;
  }
  else {
    do {
      bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_320);
      if (bVar2) {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 0x10);
      }
      else {
        pQVar13 = *(QMapDataBase **)(pQVar5 + 8);
        pQVar8 = pQVar5;
      }
      pQVar5 = pQVar13;
    } while (pQVar13 != (QMapDataBase *)0x0);
    if ((pQVar8 == (QMapDataBase *)0x0) ||
       (bVar2 = operator<((QString *)&local_320,(QString *)(pQVar8 + 0x18)), bVar2))
    goto LAB_1400a2311;
  }
  QVariant::toString((QVariant *)(pQVar8 + 0x20));
  QString::~QString((QString *)&local_320);
  if (*(int *)(local_300 + 4) == 0) {
    uVar10 = thunk_FUN_140082b70();
    cVar4 = thunk_FUN_140084f40(uVar10);
    if (cVar4 != '\0') {
      uVar10 = thunk_FUN_140034660(local_2b0);
      cVar4 = thunk_FUN_140034220(uVar10,10000);
      thunk_FUN_140033080(local_2b0);
      if (cVar4 != '\0') {
        local_268 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_1e0);
        local_318 = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_268,local_260,false);
        *(undefined ***)
         ((basic_ostream<char,std::char_traits<char>_> *)&local_268 + *(int *)(local_268 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
             *(int *)(local_268 + 4) + -0x88;
        thunk_FUN_140010dc0(local_260,2);
        thunk_FUN_140010590((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                            "Manager::onBrandingLogoChanged: branding logo has not been loaded, image is empty"
                           );
        uVar10 = thunk_FUN_140016ea0((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                                     local_88);
        uVar11 = thunk_FUN_140034660(local_2a0);
        local_348 = 0x5f;
        thunk_FUN_140033850(uVar11,10000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-selfControl\\model.cpp"
                           );
        thunk_FUN_140033080(local_2a0);
        if (0xf < local_70) {
          uVar12 = local_88[0];
          if (0xfff < local_70 + 1) {
            if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar12 = *(ulonglong *)(local_88[0] - 8);
            if (local_88[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_88[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_88[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c2(uVar12);
        }
        thunk_FUN_140011330(local_1e0);
        this = local_1e0;
LAB_1400a2775:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this)
        ;
      }
    }
  }
  else {
    uVar10 = QString::QString(local_2f0,(QString *)&local_300);
    thunk_FUN_1400a3c60(local_2e8,uVar10);
    uVar10 = thunk_FUN_140082b70();
    cVar4 = thunk_FUN_140084f40(uVar10);
    if (cVar4 != '\0') {
      uVar10 = thunk_FUN_140034660(local_290);
      cVar4 = thunk_FUN_140034220(uVar10,10000);
      thunk_FUN_140033080(local_290);
      if (cVar4 != '\0') {
        local_178 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        local_318 = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,local_170,false);
        *(undefined ***)(local_170 + (longlong)*(int *)(local_178 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        thunk_FUN_140010dc0(local_170,2);
        thunk_FUN_140010590(&local_178,
                            "Manager::onBrandingLogoChanged: branding logo has been loaded");
        uVar10 = thunk_FUN_140016ea0(&local_178,local_68);
        uVar11 = thunk_FUN_140034660(local_280);
        local_348 = 100;
        thunk_FUN_140033850(uVar11,10000,uVar10,
                            "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-selfControl\\model.cpp"
                           );
        thunk_FUN_140033080(local_280);
        if (0xf < local_50) {
          uVar12 = local_68[0];
          if (0xfff < local_50 + 1) {
            if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar12 = *(ulonglong *)(local_68[0] - 8);
            if (local_68[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_68[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_68[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c2(uVar12);
        }
        thunk_FUN_140011330(local_f0);
        this = local_f0;
        goto LAB_1400a2775;
      }
    }
  }
  QString::~QString((QString *)&local_300);
  if (*(uint *)local_330 == 0) {
LAB_1400a27a8:
    thunk_FUN_140013430(local_330);
  }
  else if (*(uint *)local_330 != 0xffffffff) {
    LOCK();
    uVar1 = *(uint *)local_330;
    *(uint *)local_330 = *(uint *)local_330 - 1;
    UNLOCK();
    if (uVar1 == 1) goto LAB_1400a27a8;
  }
  if (*(uint *)local_338 != 0) {
    if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a27d2;
    LOCK();
    uVar1 = *(uint *)local_338;
    *(uint *)local_338 = *(uint *)local_338 - 1;
    UNLOCK();
    if (uVar1 != 1) goto LAB_1400a27d2;
  }
  thunk_FUN_140013430(local_338);
LAB_1400a27d2:
  QJsonObject::~QJsonObject(param_2);
  return;
}

// =============================================================================
// Ghidra: FUN_1400a6b50 @ 0x1400a6b50
// =============================================================================
undefined8 FUN_1400a6b50(longlong *param_1,QString *param_2)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  longlong *plVar5;
  longlong lVar6;
  QString *pQVar7;
  QVariant *pQVar8;
  void **ppvVar9;
  QListData *this;
  undefined1 local_res8 [8];
  undefined8 uVar10;
  QVariant local_20 [24];
  
  uVar10 = 0xfffffffffffffffe;
  uVar2 = 0;
  if (*(int *)(*param_1 + 0x20) != 0) {
    uVar2 = qHash(param_2,*(uint *)(*param_1 + 0x24));
  }
  plVar5 = (longlong *)thunk_FUN_1400a7070(param_1,param_2,uVar2);
  if (*plVar5 == *param_1) {
    thunk_FUN_1400a7520(param_1,local_res8,param_2,param_2,uVar10);
    iVar3 = QListData::size((QListData *)(param_1 + 1));
    lVar6 = thunk_FUN_1400a6350(param_1,param_2);
    *(int *)(lVar6 + 0x24) = iVar3;
    uVar10 = thunk_FUN_1400a8d40(param_2,local_20);
    thunk_FUN_140012a70(param_1 + 1,uVar10);
  }
  else {
    pQVar7 = (QString *)thunk_FUN_1400a6350(param_1,param_2);
    bVar1 = operator==(pQVar7,param_2);
    if ((((bVar1) && (bVar1 = operator==(pQVar7 + 8,param_2 + 8), bVar1)) &&
        (*(longlong *)(pQVar7 + 0x18) == *(longlong *)(param_2 + 0x18))) &&
       (pQVar7[0x20] == param_2[0x20])) {
      return 0;
    }
    QString::operator=(pQVar7 + 8,param_2 + 8);
    *(undefined8 *)(pQVar7 + 0x18) = *(undefined8 *)(param_2 + 0x18);
    pQVar7[0x20] = param_2[0x20];
    this = (QListData *)(param_1 + 1);
    iVar3 = QListData::size(this);
    if (iVar3 <= *(int *)(pQVar7 + 0x24)) {
      qt_assert("candidate._viewIndex < _tasksView.size() && \"TasksModel::consume: \\\"index out of range\\\"\""
                ,
                "R:\\bbuild-8\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-taskTracker/model.hpp"
                ,0x77);
    }
    pQVar8 = (QVariant *)thunk_FUN_1400a8d40(pQVar7,local_20);
    iVar3 = *(int *)(pQVar7 + 0x24);
    if ((iVar3 < 0) || (iVar4 = QListData::size(this), iVar4 <= iVar3)) {
      qt_assert_x("QList<T>::operator[]","index out of range",
                  "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
    }
    if (1 < **(uint **)this) {
      thunk_FUN_14001bbe0(this,(*(uint **)this)[1]);
    }
    ppvVar9 = QListData::at(this,iVar3);
    QVariant::operator=(*ppvVar9,pQVar8);
  }
  QVariant::~QVariant(local_20);
  return 1;
}
