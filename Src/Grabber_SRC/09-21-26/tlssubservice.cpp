/*
 * tlssubservice_FULL CODE.cpp
 * tlssubservice.exe / grabberSubAgent - pseudocod extras cu Ghidra MCP
 * Data analizei: 2026-09-21
 *
 * IMPORTANT:
 * - Acesta este pseudocod C/C++ produs de Ghidra, nu codul-sursa original.
 * - Nu este garantat compilabil. Tipurile undefined*, numele FUN_/DAT_/LAB_,
 *   prototipurile si conventiile de apel sunt inferentele decompilatorului.
 * - Adresele sunt pastrate pentru verificare directa in proiectul Ghidra.
 * - Binarul are 7.953 functii; Qt/CRT/STL, QML si instantiarile repetitive
 *   nu sunt reproduse integral.
 */


// =============================================================================
// STARTUP SI MODURI
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140002f04
// -----------------------------------------------------------------------------
void entry(void)

{
  __security_init_cookie();
  FUN_1400b85a0();
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400b85a0
// -----------------------------------------------------------------------------
int FUN_1400b85a0(void)

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
  if (DAT_1407671f0 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_1407671f0 == 0) {
    DAT_1407671f0 = 1;
    iVar6 = _initterm_e(&DAT_1400dace0,&DAT_1400db120);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_1400da000,&DAT_1400dabd0);
    DAT_1407671f0 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_1400b9b24();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_1400b9b30();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1400b9ec0(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
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
// Ghidra @ 0x1400170f0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_1400170f0(int param_1,char **param_2)

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
  uVar9 = thunk_FUN_1400145d0();
  thunk_FUN_140029d80(uVar9,1,uVar8);
  uVar8 = thunk_FUN_140016910(local_5a0,0);
  thunk_FUN_14002ce40(uVar8);
  QString::~QString(local_5a0);
  QString::QString(local_660);
  QString::QString(local_678);
  if (local_658[0] == 2) {
    local_628 = QString::fromAscii_helper("-get-network-proxies",0x14);
    bVar3 = true;
    bVar2 = QString::operator==((QString *)&local_628,param_2[1]);
    if (!bVar2) goto LAB_14001722d;
    bVar2 = true;
  }
  else {
LAB_14001722d:
    bVar2 = false;
  }
  if (bVar3) {
    QString::~QString((QString *)&local_628);
  }
  if (bVar2) {
    FUN_140014710();
    QString::~QString(local_678);
    QString::~QString(local_660);
    return 0;
  }
  QApplication::QApplication(local_5e0,local_658,param_2,0x50602);
  QGuiApplication::setQuitOnLastWindowClosed(false);
  uVar8 = QCoreApplication::arguments();
  FUN_1400143e0(&local_680,uVar8);
  if (*(int *)local_620 == 0) {
LAB_1400172e2:
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
    if (*(int *)local_620 == 0) goto LAB_1400172e2;
  }
  local_640 = QString::fromAscii_helper("-logging-postfix",0x10);
  if (1 < *(uint *)(local_680 + 0x10)) {
    pQVar10 = QHashData::detach_helper
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140005fb0,
                         (_func_void_Node_ptr *)&LAB_1400020ef,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_140017392:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ef);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar11 = local_680 + 0x10;
      iVar7 = *(int *)pQVar11;
      *(int *)pQVar11 = *(int *)pQVar11 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_140017392;
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
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140005fb0,
                         (_func_void_Node_ptr *)&LAB_1400020ef,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_1400174b9:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ef);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar14 = local_680 + 0x10;
      iVar7 = *(int *)pQVar14;
      *(int *)pQVar14 = *(int *)pQVar14 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_1400174b9;
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
  pQVar13 = (QString *)thunk_FUN_1400118e0(local_5c8,&local_618,&DAT_1400dbf2c);
  QChar::QChar(local_648,0x20);
  pQVar13 = (QString *)QString::arg(pQVar13,local_5d0,local_668[0],0);
  QString::operator=(local_678,pQVar13);
  QString::~QString(local_5d0);
  QString::~QString(local_5c8);
  QString::~QString((QString *)&local_618);
  bVar3 = *(int *)(local_670 + 4) != 0;
  if (bVar3) {
    pQVar13 = (QString *)thunk_FUN_140011970(local_5c0,&DAT_1400dbf30,&local_670);
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
  uVar8 = thunk_FUN_140016910(local_5b0,8);
  uVar8 = thunk_FUN_1400118e0(local_5b8,uVar8,&DAT_1400dbf38);
  thunk_FUN_140011870(local_650,uVar8,local_678);
  QString::~QString(local_5b8);
  QString::~QString(local_5b0);
  uVar8 = QString::QString((QString *)&local_5e8,local_650);
  thunk_FUN_140035230(uVar8,0xa00000);
  qInstallMessageHandler((_func_void_QtMsgType_QMessageLogContext_ptr_QString_ptr *)&LAB_1400043bd);
  local_638 = QString::fromAscii_helper("-ipc-name",9);
  if (1 < *(uint *)(local_680 + 0x10)) {
    pQVar10 = QHashData::detach_helper
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140005fb0,
                         (_func_void_Node_ptr *)&LAB_1400020ef,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_14001782e:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ef);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar11 = local_680 + 0x10;
      iVar7 = *(int *)pQVar11;
      *(int *)pQVar11 = *(int *)pQVar11 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_14001782e;
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
                        (local_680,(_func_void_Node_ptr_void_ptr *)&LAB_140005fb0,
                         (_func_void_Node_ptr *)&LAB_1400020ef,0x20,8);
    if (*(int *)(local_680 + 0x10) == 0) {
LAB_140017953:
      QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ef);
    }
    else if (*(int *)(local_680 + 0x10) != -1) {
      LOCK();
      pQVar14 = local_680 + 0x10;
      iVar7 = *(int *)pQVar14;
      *(int *)pQVar14 = *(int *)pQVar14 + -1;
      UNLOCK();
      if (iVar7 == 1) goto LAB_140017953;
    }
  }
  local_680 = pQVar11;
  if (pQVar10 != local_680) {
    QString::operator=(local_660,(QString *)(pQVar10 + 0x18));
    uVar8 = thunk_FUN_140034c40(local_560);
    cVar4 = thunk_FUN_140034800(uVar8,20000);
    thunk_FUN_140033660(local_560);
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
      thunk_FUN_140010c30(local_310,2);
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010400(&local_318,"start agent for ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,local_668[0]);
      uVar8 = thunk_FUN_140016d10(&local_318,local_98);
      uVar9 = thunk_FUN_140034c40(local_550);
      thunk_FUN_140033e30(uVar9,20000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                         );
      thunk_FUN_140033660(local_550);
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
        FUN_1400025c7(uVar12);
      }
      thunk_FUN_1400111a0(local_290);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_290);
    }
    thunk_FUN_140019ce0(local_138);
    thunk_FUN_14001c130(local_138,local_660);
    iVar7 = QApplication::exec();
    thunk_FUN_14001a390(local_138);
    QString::~QString(local_650);
    QString::~QString((QString *)&local_670);
    if (*(int *)(local_680 + 0x10) != 0) {
      if (*(int *)(local_680 + 0x10) == -1) goto LAB_140017da5;
      LOCK();
      pQVar10 = local_680 + 0x10;
      iVar1 = *(int *)pQVar10;
      *(int *)pQVar10 = *(int *)pQVar10 + -1;
      UNLOCK();
      if (iVar1 != 1) goto LAB_140017da5;
    }
    QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ef);
LAB_140017da5:
    QApplication::~QApplication(local_5e0);
    QString::~QString(local_678);
    QString::~QString(local_660);
    return iVar7;
  }
  uVar8 = thunk_FUN_140034c40(local_580);
  cVar4 = thunk_FUN_140034800(uVar8,40000);
  thunk_FUN_140033660(local_580);
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
    thunk_FUN_140010c30(local_400,2);
    thunk_FUN_140010400(&local_408,"IPC name not specified");
    uVar8 = thunk_FUN_140016d10(&local_408,local_b8);
    uVar9 = thunk_FUN_140034c40(local_570);
    thunk_FUN_140033e30(uVar9,40000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-grabberSubAgent\\main.cpp"
                       );
    thunk_FUN_140033660(local_570);
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
      FUN_1400025c7(uVar12);
    }
    thunk_FUN_1400111a0(local_380);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_380);
  }
  QString::~QString(local_650);
  QString::~QString((QString *)&local_670);
  if (*(int *)(local_680 + 0x10) != 0) {
    if (*(int *)(local_680 + 0x10) == -1) goto LAB_140017b5e;
    LOCK();
    pQVar10 = local_680 + 0x10;
    iVar7 = *(int *)pQVar10;
    *(int *)pQVar10 = *(int *)pQVar10 + -1;
    UNLOCK();
    if (iVar7 != 1) goto LAB_140017b5e;
  }
  QHashData::free_helper(local_680,(_func_void_Node_ptr *)&LAB_1400020ef);
LAB_140017b5e:
  QApplication::~QApplication(local_5e0);
  QString::~QString(local_678);
  QString::~QString(local_660);
  return 1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140014710
// -----------------------------------------------------------------------------
void FUN_140014710(void)

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
  thunk_FUN_1400108e0(local_168,&local_170);
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
        puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140014883:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_140013330();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_140013330();
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
        if (iVar2 == 1) goto LAB_140014883;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014945:
      pQVar10 = (QVariant *)QVariant::QVariant(local_a0);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_1400149bc:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_140013330();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_140013330();
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
          if (iVar2 == 1) goto LAB_1400149bc;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014aa0:
        pQVar12 = (QMapDataBase *)thunk_FUN_140012ee0(local_res8,&local_res10,pQVar10,pQVar8,bVar4);
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
        goto LAB_140014aa0;
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
      goto LAB_140014945;
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
        puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140014b99:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_140013330();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_140013330();
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
        if (iVar2 == 1) goto LAB_140014b99;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014c55:
      pQVar10 = (QVariant *)QVariant::QVariant(local_90);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_140014ccc:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_140013330();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_140013330();
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
          if (iVar2 == 1) goto LAB_140014ccc;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014db0:
        pQVar12 = (QMapDataBase *)thunk_FUN_140012ee0(local_res8,&local_res18,pQVar10,pQVar8,bVar4);
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
        goto LAB_140014db0;
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
      goto LAB_140014c55;
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
        puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_140014eb3:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_140013330();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_140013330();
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
        if (iVar2 == 1) goto LAB_140014eb3;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140014f75:
      pQVar10 = (QVariant *)QVariant::QVariant(local_80);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_140014fec:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_140013330();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_140013330();
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
          if (iVar2 == 1) goto LAB_140014fec;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400150d0:
        pQVar12 = (QMapDataBase *)thunk_FUN_140012ee0(local_res8,&local_res20,pQVar10,pQVar8,bVar4);
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
        goto LAB_1400150d0;
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
      goto LAB_140014f75;
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
        puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_1400151d1:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_140013330();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_140013330();
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
        if (iVar2 == 1) goto LAB_1400151d1;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_140015291:
      pQVar10 = (QVariant *)QVariant::QVariant(local_70);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        pQVar12 = local_res8;
        if (*(int *)local_res8 == 0) {
LAB_140015308:
          lVar16 = *(longlong *)(local_res8 + 0x10);
          if (lVar16 != 0) {
            QString::~QString((QString *)(lVar16 + 0x18));
            QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
            if (*(longlong *)(lVar16 + 8) != 0) {
              thunk_FUN_140013330();
            }
            if (*(longlong *)(lVar16 + 0x10) != 0) {
              thunk_FUN_140013330();
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
          if (iVar2 == 1) goto LAB_140015308;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400153ec:
        pQVar12 = (QMapDataBase *)thunk_FUN_140012ee0(local_res8,&local_188,pQVar10,pQVar8,bVar4);
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
        goto LAB_1400153ec;
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
      goto LAB_140015291;
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
        puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
        *(ulonglong **)(pQVar8 + 0x10) = puVar9;
        *puVar9 = *puVar9 & 3;
        *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
      }
      pQVar12 = local_res8;
      if (*(int *)local_res8 == 0) {
LAB_1400154e2:
        lVar16 = *(longlong *)(local_res8 + 0x10);
        if (lVar16 != 0) {
          QString::~QString((QString *)(lVar16 + 0x18));
          QVariant::~QVariant((QVariant *)(lVar16 + 0x20));
          if (*(longlong *)(lVar16 + 8) != 0) {
            thunk_FUN_140013330();
          }
          if (*(longlong *)(lVar16 + 0x10) != 0) {
            thunk_FUN_140013330();
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
        if (iVar2 == 1) goto LAB_1400154e2;
      }
      local_res8 = pQVar8;
      QMapDataBase::recalcMostLeftNode(pQVar8);
    }
    if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400155a1:
      pQVar10 = (QVariant *)QVariant::QVariant(local_60);
      if (1 < *(uint *)local_res8) {
        pQVar8 = QMapDataBase::createData();
        if (*(longlong *)(local_res8 + 0x10) != 0) {
          puVar9 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_res8 + 0x10),pQVar8);
          *(ulonglong **)(pQVar8 + 0x10) = puVar9;
          *puVar9 = *puVar9 & 3;
          *puVar9 = *puVar9 | (ulonglong)(pQVar8 + 8);
        }
        if (*(uint *)local_res8 == 0) {
LAB_140015617:
          thunk_FUN_1400132a0(local_res8);
        }
        else if (*(uint *)local_res8 != 0xffffffff) {
          LOCK();
          uVar3 = *(uint *)local_res8;
          *(uint *)local_res8 = *(uint *)local_res8 - 1;
          UNLOCK();
          if (uVar3 == 1) goto LAB_140015617;
        }
        local_res8 = pQVar8;
        QMapDataBase::recalcMostLeftNode(pQVar8);
      }
      pQVar8 = local_res8 + 8;
      pQVar12 = (QMapDataBase *)0x0;
      bVar4 = true;
      pQVar17 = *(QMapDataBase **)(local_res8 + 0x10);
      if (*(QMapDataBase **)(local_res8 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400156ac:
        pQVar12 = (QMapDataBase *)thunk_FUN_140012ee0(local_res8,&local_180,pQVar10,pQVar8,bVar4);
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
        goto LAB_1400156ac;
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
      goto LAB_1400155a1;
    }
    QVariant::operator=((QVariant *)(pQVar12 + 0x20),local_d8);
    QString::~QString((QString *)&local_180);
    QVariant::~QVariant(local_d8);
    QString::~QString(local_130);
    QVariant::QVariant(local_c8,(QMap<QString,QVariant> *)&local_res8);
    thunk_FUN_1400128e0(&local_178,local_c8);
    QVariant::~QVariant(local_c8);
    if (*(uint *)local_res8 == 0) {
LAB_140015757:
      thunk_FUN_1400132a0();
    }
    else if (*(uint *)local_res8 != 0xffffffff) {
      LOCK();
      uVar3 = *(uint *)local_res8;
      *(uint *)local_res8 = *(uint *)local_res8 - 1;
      UNLOCK();
      if (uVar3 == 1) goto LAB_140015757;
    }
  }
  local_150 = 1;
  thunk_FUN_140010f40(local_168);
  QVariant::QVariant(local_b8,(QList<QVariant> *)&local_178);
  this = (QJsonDocument *)QJsonDocument::fromVariant(local_120);
  uVar13 = QJsonDocument::toJson(this);
  this_00 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_1400114c0(cout_exref,uVar13);
  std::basic_ostream<char,std::char_traits<char>_>::operator<<
            (this_00,(_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                      *)&LAB_1400025d1);
  QByteArray::~QByteArray(local_128);
  QJsonDocument::~QJsonDocument((QJsonDocument *)local_120);
  QVariant::~QVariant(local_b8);
  if (*(int *)local_178 != 0) {
    if (*(int *)local_178 == -1) goto LAB_140015836;
    LOCK();
    iVar2 = *(int *)local_178;
    *(int *)local_178 = *(int *)local_178 + -1;
    UNLOCK();
    if (iVar2 != 1) goto LAB_140015836;
  }
  thunk_FUN_140013090(&local_178,local_178);
LAB_140015836:
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400143e0
// -----------------------------------------------------------------------------
undefined8 * FUN_1400143e0(undefined8 *param_1,QListData *param_2)

{
  int iVar1;
  QString *pQVar2;
  void **ppvVar3;
  QString *this;
  int iVar4;
  int iVar5;
  
  *param_1 = shared_null_exref;
  iVar4 = 1;
  iVar1 = QListData::size(param_2);
  if (1 < iVar1) {
    do {
      iVar5 = iVar4 + 1;
      iVar1 = QListData::size(param_2);
      if (iVar5 < iVar1) {
        if ((iVar5 < 0) || (iVar1 = QListData::size(param_2), iVar1 <= iVar5)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        pQVar2 = (QString *)QListData::at(param_2,iVar5);
        if ((iVar4 < 0) || (iVar1 = QListData::size(param_2), iVar1 <= iVar4)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar3 = QListData::at(param_2,iVar4);
        this = (QString *)thunk_FUN_1400115d0(param_1,ppvVar3);
        QString::operator=(this,pQVar2);
      }
      iVar4 = iVar4 + 2;
      iVar1 = QListData::size(param_2);
    } while (iVar4 < iVar1);
  }
  return param_1;
}

// =============================================================================
// SERVICIU IPC SUBAGENT
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140019ce0
// -----------------------------------------------------------------------------
QObject * FUN_140019ce0(QObject *param_1)

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
    if (iVar1 != 0) goto LAB_140019db6;
  }
  else {
LAB_140019db6:
    qt_assert_x("qRegisterNormalizedMetaType",
                "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
                ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
  }
  QByteArray::~QByteArray(local_res18);
  QMetaType::registerNormalizedType(&local_res10,&LAB_140007bf8,&LAB_140002e23,4,0x17,0);
  QByteArray::~QByteArray((QByteArray *)&local_res10);
  QMetaObject::normalizedType((char *)&local_res10);
  QByteArray::constData((QByteArray *)&local_res10);
  pQVar2 = (QByteArray *)QMetaObject::normalizedType((char *)local_res18);
  iVar1 = *(int *)(local_res10 + 4);
  if (iVar1 == *(int *)(*(longlong *)pQVar2 + 4)) {
    pcVar3 = QByteArray::constData(pQVar2);
    pcVar4 = QByteArray::constData((QByteArray *)&local_res10);
    iVar1 = memcmp(pcVar4,pcVar3,(longlong)iVar1);
    if (iVar1 == 0) goto LAB_140019ea7;
  }
  qt_assert_x("qRegisterNormalizedMetaType",
              "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
              ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
LAB_140019ea7:
  QByteArray::~QByteArray(local_res18);
  QMetaType::registerNormalizedType(&local_res10,&LAB_1400079b9,&LAB_140007c5c,4,0x17,0);
  QByteArray::~QByteArray((QByteArray *)&local_res10);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14001a650
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001a650(undefined8 param_1,int param_2)

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
  uVar2 = thunk_FUN_140034c40(local_150);
  cVar1 = thunk_FUN_140034800(uVar2,10000);
  thunk_FUN_140033660(local_150);
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
           thunk_FUN_140010400(&local_138,"IPC Socket got error: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
    uVar2 = thunk_FUN_140016d10(&local_138,local_48);
    uVar3 = thunk_FUN_140034c40(local_150);
    local_168 = 0xa5;
    thunk_FUN_140033e30(uVar3,10000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033660(local_150);
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
      FUN_1400025c7(uVar4);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_130);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x14001a910
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001a910(undefined8 param_1,int param_2)

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
  uVar2 = thunk_FUN_140034c40(local_150);
  cVar1 = thunk_FUN_140034800(uVar2,10000);
  thunk_FUN_140033660(local_150);
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
           thunk_FUN_140010400(&local_138,"IPC Socket state: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
    uVar2 = thunk_FUN_140016d10(&local_138,local_48);
    uVar3 = thunk_FUN_140034c40(local_150);
    local_168 = 0xa7;
    thunk_FUN_140033e30(uVar3,10000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033660(local_150);
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
      FUN_1400025c7(uVar4);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_130);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x14001c130
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001c130(char *param_1,QString *param_2)

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
  puVar5 = (undefined8 *)thunk_FUN_140020d20();
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
  uVar8 = thunk_FUN_140034c40(local_2b0);
  cVar3 = thunk_FUN_140034800(uVar8,20000);
  thunk_FUN_140033660(local_2b0);
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
    uVar8 = thunk_FUN_140010400(&local_268,"Starting to listen: ");
    this_00 = (QByteArray *)QString::toLocal8Bit(param_2);
    pcVar6 = QByteArray::data(this_00);
    thunk_FUN_140010400(uVar8,pcVar6);
    QByteArray::~QByteArray(local_2f0);
    uVar8 = thunk_FUN_140016d10(&local_268,local_88);
    uVar9 = thunk_FUN_140034c40(local_2a0);
    thunk_FUN_140033e30(uVar9,20000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033660(local_2a0);
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
      FUN_1400025c7(uVar13);
    }
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_260);
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
    thunk_FUN_140018e40(local_2e8,param_1 + 0x60,timeout_exref,param_1);
    QMetaObject::Connection::~Connection(local_2e8);
    local_318 = operator_new(0x118);
    plVar10 = plVar11;
    if (local_318 != (void *)0x0) {
      plVar10 = (longlong *)thunk_FUN_140086990(local_318);
    }
    plVar2 = *(longlong **)(param_1 + 0x48);
    if ((plVar2 != plVar10) && (*(longlong **)(param_1 + 0x48) = plVar10, plVar2 != (longlong *)0x0)
       ) {
      (**(code **)(*plVar2 + 0x18))(plVar2,1);
    }
    local_318 = operator_new(0xd0);
    if (local_318 != (void *)0x0) {
      plVar11 = (longlong *)thunk_FUN_14007e280(local_318);
    }
    thunk_FUN_14001ffb0(param_1 + 0x50,plVar11);
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
  uVar8 = thunk_FUN_140034c40(local_290);
  cVar3 = thunk_FUN_140034800(uVar8,30000);
  thunk_FUN_140033660(local_290);
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
    thunk_FUN_140010400(&local_178,"local server does not listen");
    uVar8 = thunk_FUN_140016d10(&local_178,local_68);
    uVar9 = thunk_FUN_140034c40(local_280);
    thunk_FUN_140033e30(uVar9,30000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033660(local_280);
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
      FUN_1400025c7(uVar13);
    }
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_170);
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
  _CxxThrowException(&local_2e0,(ThrowInfo *)&DAT_140762348);
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14001ca00
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001ca00(longlong param_1)

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
    thunk_FUN_140020070(param_1 + 0x20);
    QObject::setParent(*(QObject **)(param_1 + 0x20),(QObject *)0x0);
    plVar1 = *(longlong **)(param_1 + 0x30);
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x18))(plVar1,1);
    }
    local_168 = operator_new(0x30);
    if (local_168 != (QTypedArrayData<unsigned_short> *)0x0) {
      uVar5 = thunk_FUN_140077bd0(local_168,param_1 + 0x20);
    }
    *(undefined8 *)(param_1 + 0x30) = uVar5;
    cVar4 = thunk_FUN_14007d0e0(uVar5);
    if (cVar4 == '\0') {
      uVar5 = thunk_FUN_140034c40(local_158);
      cVar4 = thunk_FUN_140034800(uVar5,40000);
      thunk_FUN_140033660(local_158);
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
        thunk_FUN_140010c30(&local_140,2);
        thunk_FUN_140010400(&local_148,"Failed to create RPC object");
        uVar5 = thunk_FUN_140016d10(&local_148,local_50);
        uVar6 = thunk_FUN_140034c40(local_158);
        thunk_FUN_140033e30(uVar6,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                           );
        thunk_FUN_140033660(local_158);
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
          FUN_1400025c7(uVar9);
        }
        thunk_FUN_1400111a0(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
      QLocalSocket::abort(*(QLocalSocket **)(param_1 + 0x20));
    }
    else {
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"getUserInfo",param_1,"onGetUserInfo");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"getSystemProxies",param_1,
                          "onGetSystemProxies");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"resetClipboard",param_1,
                          "onResetClipboard");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setClipboardText",param_1,
                          "onSetClipboardText");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setClipboardHtml",param_1,
                          "onSetClipboardHtml");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setClipboardImage",param_1,
                          "onSetClipboardImage");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setClipboardMimeData",param_1,
                          "onSetClipboardMimeData");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"clearClipboard",param_1,
                          "onClearClipboard");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"getClipboardUrls",param_1,
                          "onGetClipboardUrls");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setSelfControlState",param_1,
                          "onSelfControlState");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setSelfControlDayActivity",param_1,
                          "onSelfControlDayActivity");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setSelfControlDayRating",param_1,
                          "onSelfControlDayRating");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setSelfControlWebToken",param_1,
                          "onSelfControlWebToken");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"notifySelfControlTasksChanged",param_1,
                          "onSelfControlTasksChanged");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setNotificationAlert",param_1,
                          "onNotificationAlert");
      thunk_FUN_1400783a0(*(undefined8 *)(param_1 + 0x30),"setWebinterfaceLink",param_1,
                          "onWebinterfaceLink");
      pcVar2 = *(char **)self_exref;
      pcVar3 = *(char **)(param_1 + 0x20);
      pcVar7 = qFlagLocation("1quit()");
      pQVar8 = (QObject *)qFlagLocation("2disconnected()");
      QObject::connect((QObject *)&local_168,pcVar3,pQVar8,pcVar2,(ConnectionType)pcVar7);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      if ((*(longlong *)(param_1 + 0x48) != 0) &&
         (cVar4 = (**(code **)(**(longlong **)(param_1 + 0x48) + 0x58))(), cVar4 != '\0')) {
        thunk_FUN_140018920(&local_168,*(undefined8 *)(param_1 + 0x48),thunk_FUN_1400867f0,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_168);
      }
      if (*(longlong *)(param_1 + 0x50) != 0) {
        thunk_FUN_140018a90(&local_168,*(longlong *)(param_1 + 0x50),thunk_FUN_140086580,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_168);
        thunk_FUN_140018a90(&local_168,*(undefined8 *)(param_1 + 0x50),thunk_FUN_1400865c0,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_168);
      }
      thunk_FUN_140018ce0(&local_168,*(undefined8 *)(param_1 + 0x20),error_exref,0);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      thunk_FUN_140018e00(&local_168,*(undefined8 *)(param_1 + 0x20),stateChanged_exref,0);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      local_160 = QString::fromAscii_helper("test ping",9);
      QVariant::QVariant(local_158,(QString *)&local_160);
      QString::~QString((QString *)&local_160);
      local_168 = QString::fromAscii_helper("testPing",8);
      thunk_FUN_14007cd30(*(undefined8 *)(param_1 + 0x30),&local_168,local_158);
      QString::~QString((QString *)&local_168);
      QVariant::~QVariant(local_158);
    }
  }
  else {
    uVar5 = thunk_FUN_140034c40(local_158);
    cVar4 = thunk_FUN_140034800(uVar5,40000);
    thunk_FUN_140033660(local_158);
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
      thunk_FUN_140010400(&local_148,"Attempted to connect to already connected IPC (!)");
      uVar5 = thunk_FUN_140016d10(&local_148,local_50);
      uVar6 = thunk_FUN_140034c40(local_158);
      thunk_FUN_140033e30(uVar6,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                         );
      thunk_FUN_140033660(local_158);
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
        FUN_1400025c7(uVar9);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_140);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x14001d7a0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14001d7a0(void)

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
  uVar2 = thunk_FUN_140034c40(local_140);
  cVar1 = thunk_FUN_140034800(uVar2,20000);
  thunk_FUN_140033660(local_140);
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
    thunk_FUN_140010400(&local_128,"await conection timeout. aborting.");
    uVar2 = thunk_FUN_140016d10(&local_128,local_38);
    uVar3 = thunk_FUN_140034c40(local_140);
    local_158 = 0x16a;
    thunk_FUN_140033e30(uVar3,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-io\\service.cpp"
                       );
    thunk_FUN_140033660(local_140);
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
      FUN_1400025c7(uVar4);
    }
    *(undefined ***)((longlong)&local_128 + (longlong)*(int *)(local_128 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_130 + (longlong)*(int *)(local_128 + 4) + 4) =
         *(int *)(local_128 + 4) + -0x88;
    local_120 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_120);
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
// INFORMATII UTILIZATOR SI ADSI
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140020f30
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 * FUN_140020f30(undefined8 *param_1)

{
  char cVar1;
  DWORD DVar2;
  HMODULE hModule;
  undefined8 uVar3;
  basic_ostream<char,std::char_traits<char>_> *pbVar4;
  undefined8 uVar5;
  FARPROC pFVar6;
  ulonglong uVar7;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  undefined4 local_158;
  undefined ***local_150 [2];
  int iStack_13c;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  undefined8 local_48;
  ulonglong local_40 [3];
  ulonglong local_28;
  ulonglong local_20;
  
  local_48 = 0xfffffffffffffffe;
  local_20 = DAT_140763590 ^ (ulonglong)auStack_188;
  local_158 = 0;
  *param_1 = grabberSubAgent::UserInfoImpl::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  hModule = LoadLibraryA("activeds.dll");
  param_1[4] = hModule;
  if (hModule == (HMODULE)0x0) {
    uVar3 = thunk_FUN_140034c40(local_150);
    cVar1 = thunk_FUN_140034800(uVar3,40000);
    thunk_FUN_140033660(local_150);
    if (cVar1 == '\0') {
      return param_1;
    }
    local_138 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_b0);
    local_158 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
         *(int *)(local_138 + 4) + -0x88;
    local_150[0] = &local_130;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_c8 = 0;
    local_c0 = 4;
    DVar2 = GetLastError();
    pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010400(&local_138,"Failed to load activeds.dll: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar4,DVar2);
    uVar3 = thunk_FUN_140016d10(&local_138,local_40);
    uVar5 = thunk_FUN_140034c40(local_150);
    local_168 = 0x3a;
    thunk_FUN_140033e30(uVar5,40000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                       );
    thunk_FUN_140033660(local_150);
    if (0xf < local_28) {
      uVar7 = local_40[0];
      if (0xfff < local_28 + 1) {
        if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar7 = *(ulonglong *)(local_40[0] - 8);
        if (local_40[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_40[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_40[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c7(uVar7);
    }
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
         *(int *)(local_138 + 4) + -0x88;
    local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_130);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_128);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
    return param_1;
  }
  pFVar6 = GetProcAddress(hModule,"ADsGetObject");
  param_1[2] = pFVar6;
  if (pFVar6 == (FARPROC)0x0) {
    uVar3 = thunk_FUN_140034c40(local_150);
    cVar1 = thunk_FUN_140034800(uVar3,40000);
    thunk_FUN_140033660(local_150);
    if (cVar1 == '\0') {
      return param_1;
    }
    local_138 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_b0);
    local_158 = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
    *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
         *(int *)(local_138 + 4) + -0x88;
    thunk_FUN_140010c30(&local_130,2);
    DVar2 = GetLastError();
    pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010400(&local_138,"Failed to find ADsGetObject in activeds.dll: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar4,DVar2);
    uVar3 = thunk_FUN_140016d10(&local_138,local_40);
    uVar5 = thunk_FUN_140034c40(local_150);
    local_168 = 0x41;
    thunk_FUN_140033e30(uVar5,40000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                       );
    thunk_FUN_140033660(local_150);
    if (0xf < local_28) {
      uVar7 = local_40[0];
      if (0xfff < local_28 + 1) {
        if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar7 = *(ulonglong *)(local_40[0] - 8);
        if (local_40[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_40[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_40[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c7(uVar7);
    }
  }
  else {
    pFVar6 = GetProcAddress((HMODULE)param_1[4],"ADsOpenObject");
    param_1[3] = pFVar6;
    if (pFVar6 == (FARPROC)0x0) {
      uVar3 = thunk_FUN_140034c40(local_150);
      cVar1 = thunk_FUN_140034800(uVar3,40000);
      thunk_FUN_140033660(local_150);
      if (cVar1 == '\0') {
        return param_1;
      }
      local_138 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
           *(int *)(local_138 + 4) + -0x88;
      thunk_FUN_140010c30(&local_130,2);
      DVar2 = GetLastError();
      pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_138,"Failed to find ADsOpenObject in activeds.dll: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar4,DVar2);
      uVar3 = thunk_FUN_140016d10(&local_138,local_40);
      uVar5 = thunk_FUN_140034c40(local_150);
      local_168 = 0x48;
      thunk_FUN_140033e30(uVar5,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_150);
      if (0xf < local_28) {
        uVar7 = local_40[0];
        if (0xfff < local_28 + 1) {
          if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_40[0] - 8);
          if (local_40[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_40[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_40[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar7);
      }
    }
    else {
      pFVar6 = GetProcAddress((HMODULE)param_1[4],"FreeADsMem");
      param_1[1] = pFVar6;
      if (pFVar6 != (FARPROC)0x0) {
        CoInitialize((LPVOID)0x0);
        return param_1;
      }
      uVar3 = thunk_FUN_140034c40(local_150);
      cVar1 = thunk_FUN_140034800(uVar3,40000);
      thunk_FUN_140033660(local_150);
      if (cVar1 == '\0') {
        return param_1;
      }
      local_138 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = 8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
           *(int *)(local_138 + 4) + -0x88;
      thunk_FUN_140010c30(&local_130,2);
      DVar2 = GetLastError();
      pbVar4 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_138,"Failed to find FreeADsMem in activeds.dll: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar4,DVar2);
      uVar3 = thunk_FUN_140016d10(&local_138,local_40);
      uVar5 = thunk_FUN_140034c40(local_150);
      local_168 = 0x4f;
      thunk_FUN_140033e30(uVar5,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_150);
      if (0xf < local_28) {
        uVar7 = local_40[0];
        if (0xfff < local_28 + 1) {
          if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_40[0] - 8);
          if (local_40[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_40[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_40[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar7);
      }
    }
  }
  thunk_FUN_1400111a0(local_b0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140022990
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140022990(longlong param_1,QString *param_2,undefined8 param_3)

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
    uVar3 = thunk_FUN_140034c40(local_a0);
    cVar1 = thunk_FUN_140034800(uVar3,40000);
    thunk_FUN_140033660(local_a0);
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
               thunk_FUN_140010400(&local_198,"precondition check failed: ");
      pbVar8 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                         (pbVar7,*(void **)(param_1 + 0x10));
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(pbVar8,&DAT_1400de684);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,iVar2 == 0);
      uVar3 = thunk_FUN_140016d10(&local_198,local_80);
      uVar6 = thunk_FUN_140034c40(local_a0);
      local_1c8 = 0x98;
      thunk_FUN_140033e30(uVar6,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_a0);
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
        FUN_1400025c7(uVar10);
      }
      *(undefined ***)((longlong)&local_198 + (longlong)*(int *)(local_198 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_1a0 + (longlong)*(int *)(local_198 + 4) + 4) =
           *(int *)(local_198 + 4) + -0x88;
      local_190 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_190);
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
    thunk_FUN_140022520(&local_60,L"LDAP://",7);
    uVar3 = QString::toStdWString(param_2);
    thunk_FUN_140022010(&local_60,uVar3,0,0xffffffffffffffff);
    if (7 < local_88) {
      thunk_FUN_1400227c0(local_a0,local_a0[0],local_88 + 1);
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
        uVar3 = thunk_FUN_140034c40(local_a0);
        cVar1 = thunk_FUN_140034800(uVar3,40000);
        thunk_FUN_140033660(local_a0);
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
          thunk_FUN_140010c30(&local_190,2);
          thunk_FUN_140010400(&local_198,"get_FirstName failed");
          uVar3 = thunk_FUN_140016d10(&local_198,local_80);
          uVar6 = thunk_FUN_140034c40(local_a0);
          local_1c8 = 0x79;
          thunk_FUN_140033e30(uVar6,40000,uVar3,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033660(local_a0);
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
            FUN_1400025c7(uVar10);
          }
          thunk_FUN_1400111a0(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        pQVar4 = (QString *)FUN_1400226d0(&local_1a8);
        QVariant::QVariant((QVariant *)local_a0,pQVar4);
        local_1b8 = QString::fromAscii_helper("firstName",9);
        pQVar5 = (QVariant *)thunk_FUN_140011760(param_3,&local_1b8);
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
        uVar3 = thunk_FUN_140034c40(local_a0);
        cVar1 = thunk_FUN_140034800(uVar3,40000);
        thunk_FUN_140033660(local_a0);
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
          thunk_FUN_140010c30(&local_190,2);
          thunk_FUN_140010400(&local_198,"get_LastName failed");
          uVar3 = thunk_FUN_140016d10(&local_198,local_80);
          uVar6 = thunk_FUN_140034c40(local_a0);
          local_1c8 = 0x82;
          thunk_FUN_140033e30(uVar6,40000,uVar3,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033660(local_a0);
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
            FUN_1400025c7(uVar10);
          }
          thunk_FUN_1400111a0(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        pQVar4 = (QString *)FUN_1400226d0(&local_1a8);
        QVariant::QVariant((QVariant *)local_a0,pQVar4);
        local_1b8 = QString::fromAscii_helper("lastName",8);
        pQVar5 = (QVariant *)thunk_FUN_140011760(param_3,&local_1b8);
        QVariant::operator=(pQVar5,(QVariant *)local_a0);
        QString::~QString((QString *)&local_1b8);
        QVariant::~QVariant((QVariant *)local_a0);
        QString::~QString((QString *)&local_1a8);
        SysFreeString(local_1b0);
        local_1b0 = (BSTR)0x0;
      }
      iVar2 = (**(code **)(*local_1a0 + 0x110))(local_1a0,&local_1b0);
      if ((iVar2 < 0) || (local_1b0 == (BSTR)0x0)) {
        uVar3 = thunk_FUN_140034c40(local_a0);
        cVar1 = thunk_FUN_140034800(uVar3,40000);
        thunk_FUN_140033660(local_a0);
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
          thunk_FUN_140010c30(&local_190,2);
          thunk_FUN_140010400(&local_198,"get_FullName failed");
          uVar3 = thunk_FUN_140016d10(&local_198,local_80);
          uVar6 = thunk_FUN_140034c40(local_a0);
          local_1c8 = 0x8b;
          thunk_FUN_140033e30(uVar6,40000,uVar3,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033660(local_a0);
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
            FUN_1400025c7(uVar10);
          }
          thunk_FUN_1400111a0(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        pQVar4 = (QString *)FUN_1400226d0(&local_1a8);
        QVariant::QVariant((QVariant *)local_a0,pQVar4);
        local_1b8 = QString::fromAscii_helper("fullName",8);
        pQVar5 = (QVariant *)thunk_FUN_140011760(param_3,&local_1b8);
        QVariant::operator=(pQVar5,(QVariant *)local_a0);
        QString::~QString((QString *)&local_1b8);
        QVariant::~QVariant((QVariant *)local_a0);
        QString::~QString((QString *)&local_1a8);
        SysFreeString(local_1b0);
        local_1b0 = (BSTR)0x0;
      }
      (**(code **)(*local_1a0 + 0x10))();
      if (7 < local_48) {
        thunk_FUN_1400227c0(&local_60,CONCAT62(uStack_5e,local_60),local_48 + 1);
      }
      return 1;
    }
    uVar3 = thunk_FUN_140034c40(local_a0);
    cVar1 = thunk_FUN_140034800(uVar3,40000);
    thunk_FUN_140033660(local_a0);
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
      thunk_FUN_140010c30(&local_190,2);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_198,"ADsGetObject failed: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,iVar2);
      uVar3 = thunk_FUN_140016d10(&local_198,local_80);
      uVar6 = thunk_FUN_140034c40(local_a0);
      local_1c8 = 0x92;
      thunk_FUN_140033e30(uVar6,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_a0);
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
        FUN_1400025c7(uVar10);
      }
      thunk_FUN_1400111a0(local_110);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_110);
    }
    if (7 < local_48) {
      thunk_FUN_1400227c0(&local_60,CONCAT62(uStack_5e,local_60),local_48 + 1);
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140023760
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QString * FUN_140023760(undefined8 param_1,QString *param_2)

{
  char cVar1;
  BOOL BVar2;
  HANDLE ProcessHandle;
  undefined8 *TokenInformation;
  QString *pQVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *this;
  ulonglong uVar6;
  undefined1 auStackY_198 [32];
  DWORD local_168 [2];
  undefined ***local_160 [2];
  QString local_150 [8];
  HANDLE local_148;
  undefined8 local_140;
  undefined *local_138;
  undefined **local_130;
  basic_ostream<char,std::char_traits<char>_> local_128 [96];
  undefined8 local_c8;
  undefined4 local_c0;
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  undefined8 local_48;
  ulonglong local_40 [3];
  ulonglong local_28;
  ulonglong local_20;
  
  local_48 = 0xfffffffffffffffe;
  local_20 = DAT_140763590 ^ (ulonglong)auStackY_198;
  local_168[1] = 0;
  QString::QString(local_150);
  local_148 = (HANDLE)0x0;
  ProcessHandle = GetCurrentProcess();
  BVar2 = OpenProcessToken(ProcessHandle,8,&local_148);
  if (BVar2 == 0) {
    uVar4 = thunk_FUN_140034c40(local_160);
    cVar1 = thunk_FUN_140034800(uVar4,40000);
    thunk_FUN_140033660(local_160);
    if (cVar1 != '\0') {
      local_138 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_168[1] = 8;
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
      thunk_FUN_140010400(&local_138,"OpenProcessToken failed");
      uVar4 = thunk_FUN_140016d10(&local_138,local_40);
      uVar5 = thunk_FUN_140034c40(local_160);
      thunk_FUN_140033e30(uVar5,40000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_160);
      if (0xf < local_28) {
        uVar6 = local_40[0];
        if (0xfff < local_28 + 1) {
          if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar6 = *(ulonglong *)(local_40[0] - 8);
          if (local_40[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_40[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_40[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar6);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
LAB_140023d1f:
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  else {
    local_168[0] = 0;
    GetTokenInformation(local_148,TokenUser,(LPVOID)0x0,0,local_168);
    if (local_168[0] == 0) {
      uVar4 = thunk_FUN_140034c40(local_160);
      cVar1 = thunk_FUN_140034800(uVar4,40000);
      thunk_FUN_140033660(local_160);
      if (cVar1 != '\0') {
        local_138 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_168[1] = 4;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
        *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_140010c30(&local_130,2);
        this = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_138,"GetTokenInformation failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,local_168[0]);
        uVar4 = thunk_FUN_140016d10(&local_138,local_40);
        uVar5 = thunk_FUN_140034c40(local_160);
        thunk_FUN_140033e30(uVar5,40000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_160);
        if (0xf < local_28) {
          uVar6 = local_40[0];
          if (0xfff < local_28 + 1) {
            if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_40[0] - 8);
            if (local_40[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_40[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_40[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar6);
        }
        thunk_FUN_1400111a0(local_b0);
        goto LAB_140023d1f;
      }
    }
    else {
      TokenInformation = LocalAlloc(0x40,(ulonglong)local_168[0]);
      BVar2 = GetTokenInformation(local_148,TokenUser,TokenInformation,local_168[0],local_168);
      if (BVar2 != 0) {
        local_140 = (HLOCAL)0x0;
        ConvertSidToStringSidA(*TokenInformation,&local_140);
        pQVar3 = (QString *)QString::fromLocal8Bit((char *)local_160,(int)local_140);
        QString::operator=(local_150,pQVar3);
        QString::~QString((QString *)local_160);
        LocalFree(local_140);
        LocalFree(TokenInformation);
        CloseHandle(local_148);
        QString::QString(param_2,local_150);
        goto LAB_140023d33;
      }
      uVar4 = thunk_FUN_140034c40(local_160);
      cVar1 = thunk_FUN_140034800(uVar4,40000);
      thunk_FUN_140033660(local_160);
      if (cVar1 != '\0') {
        local_138 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_168[1] = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_130,false);
        *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_140010c30(&local_130,2);
        thunk_FUN_140010400(&local_138,"GetTokenInformation failed 2");
        uVar4 = thunk_FUN_140016d10(&local_138,local_40);
        uVar5 = thunk_FUN_140034c40(local_160);
        thunk_FUN_140033e30(uVar5,40000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_160);
        if (0xf < local_28) {
          uVar6 = local_40[0];
          if (0xfff < local_28 + 1) {
            if ((local_40[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_40[0] - 8);
            if (local_40[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_40[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_40[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar6);
        }
        thunk_FUN_1400111a0(local_b0);
        goto LAB_140023d1f;
      }
    }
  }
  QString::QString(param_2);
LAB_140023d33:
  QString::~QString(local_150);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140023f80
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 * FUN_140023f80(undefined ***param_1,undefined8 *param_2)

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
  thunk_FUN_140023760(param_1,local_88);
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
    thunk_FUN_1400025c7(0);
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
      uVar10 = thunk_FUN_140034c40(local_1c0);
      cVar3 = thunk_FUN_140034800(uVar10,40000);
      thunk_FUN_140033660(local_1c0);
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
        thunk_FUN_140010c30(&local_170,2);
        pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140010400(&local_178,
                                      "GetUserNameEx failed for NameFullyQualifiedDN feature with error code: "
                                     );
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,DVar4);
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_1c0);
        local_1d8 = 0xb6;
        thunk_FUN_140033e30(uVar11,40000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_1c0);
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
          FUN_1400025c7(uVar16);
        }
        thunk_FUN_1400111a0(local_f0);
LAB_1400243de:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
    else {
      pQVar9 = (QString *)QString::fromWCharArray((wchar_t *)&local_188,(int)pppuVar13);
      QString::operator=((QString *)&local_1a0,pQVar9);
      QString::~QString((QString *)&local_188);
      uVar10 = thunk_FUN_140034c40(local_1c0);
      cVar3 = thunk_FUN_140034800(uVar10,20000);
      thunk_FUN_140033660(local_1c0);
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
        thunk_FUN_140010400(&local_178,"AD info successfully fetched by GetUserNameEx");
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_1c0);
        local_1d8 = 0xbc;
        thunk_FUN_140033e30(uVar11,20000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_1c0);
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
          FUN_1400025c7(uVar16);
        }
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_140012670(&local_170);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_168);
        goto LAB_1400243de;
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
    thunk_FUN_1400025c7(0);
    pppuVar7 = pppuVar8;
  }
  cVar3 = GetUserNameExW(6,pppuVar7);
  if (cVar3 == '\0') {
    DVar4 = GetLastError();
    uVar10 = thunk_FUN_140034c40(local_1c0);
    cVar3 = thunk_FUN_140034800(uVar10,40000);
    thunk_FUN_140033660(local_1c0);
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
                thunk_FUN_140010400(&local_178,
                                    "GetUserNameEx failed for NameUniqueId feature with error code: "
                                   );
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,DVar4);
      uVar10 = thunk_FUN_140016d10(&local_178,local_68);
      uVar11 = thunk_FUN_140034c40(local_1c0);
      local_1d8 = 0xcb;
      thunk_FUN_140033e30(uVar11,40000,uVar10,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_1c0);
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
        FUN_1400025c7(uVar16);
      }
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
           *(int *)(local_178 + 4) + -0x88;
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_170);
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
  thunk_FUN_1400025c7(pppuVar7);
  thunk_FUN_1400025c7(pppuVar13);
  bVar2 = local_1b0;
  if (local_1b0 == false) {
    if ((*(int *)(local_198 + 4) == 0) || (*(int *)(local_1a0 + 4) == 0)) {
      uVar10 = thunk_FUN_140034c40(local_1c0);
      cVar3 = thunk_FUN_140034800(uVar10,30000);
      thunk_FUN_140033660(local_1c0);
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
        thunk_FUN_140010400(&local_178,"failed to query AD info from AD, trying to get from LDAP");
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_1c0);
        local_1d8 = 0xd2;
        thunk_FUN_140033e30(uVar11,30000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_1c0);
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
          FUN_1400025c7(uVar16);
        }
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_140012670(&local_170);
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
      cVar3 = thunk_FUN_140025830(param_1,local_88,&local_1a8,&local_1c8);
      if (cVar3 != '\0') {
        if (*(int *)(local_1a0 + 4) == 0) {
          QString::operator=((QString *)&local_1a0,(QString *)&local_1a8);
        }
        if (*(int *)(local_198 + 4) == 0) {
          QString::operator=((QString *)&local_198,(QString *)&local_1c8);
        }
        uVar10 = thunk_FUN_140034c40(local_1c0);
        cVar3 = thunk_FUN_140034800(uVar10,20000);
        thunk_FUN_140033660(local_1c0);
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
          thunk_FUN_140010c30(&local_170,2);
          thunk_FUN_140010400(&local_178,"AD info successfully fetched from LDAP");
          uVar10 = thunk_FUN_140016d10(&local_178,local_68);
          uVar11 = thunk_FUN_140034c40(local_1c0);
          local_1d8 = 0xe2;
          thunk_FUN_140033e30(uVar11,20000,uVar10,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033660(local_1c0);
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
            FUN_1400025c7(uVar16);
          }
          thunk_FUN_1400111a0(local_f0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_f0);
        }
      }
      QString::~QString((QString *)&local_1c8);
      QString::~QString((QString *)&local_1a8);
    }
    if ((*(int *)(local_198 + 4) == 0) || (*(int *)(local_1a0 + 4) == 0)) {
      uVar10 = thunk_FUN_140034c40(local_1c0);
      cVar3 = thunk_FUN_140034800(uVar10,30000);
      thunk_FUN_140033660(local_1c0);
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
        thunk_FUN_140010400(&local_178,
                            "failed to query AD info from LDAP, trying to get from registry");
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_1c0);
        local_1d8 = 0xea;
        thunk_FUN_140033e30(uVar11,30000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_1c0);
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
          FUN_1400025c7(uVar16);
        }
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_140012670(&local_170);
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
      cVar3 = thunk_FUN_140026ee0(param_1,local_88,&local_1a8,&local_1c8);
      if (cVar3 != '\0') {
        if (*(int *)(local_1a0 + 4) == 0) {
          QString::operator=((QString *)&local_1a0,(QString *)&local_1a8);
        }
        if (*(int *)(local_198 + 4) == 0) {
          QString::operator=((QString *)&local_198,(QString *)&local_1c8);
        }
        uVar10 = thunk_FUN_140034c40(local_1c0);
        cVar3 = thunk_FUN_140034800(uVar10,20000);
        thunk_FUN_140033660(local_1c0);
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
          thunk_FUN_140010c30(&local_170,2);
          thunk_FUN_140010400(&local_178,"AD info successfully fetched from registry");
          uVar10 = thunk_FUN_140016d10(&local_178,local_68);
          uVar11 = thunk_FUN_140034c40(local_1c0);
          local_1d8 = 0xfa;
          thunk_FUN_140033e30(uVar11,20000,uVar10,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033660(local_1c0);
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
            FUN_1400025c7(uVar16);
          }
          thunk_FUN_1400111a0(local_f0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_f0);
        }
      }
      QString::~QString((QString *)&local_1c8);
      QString::~QString((QString *)&local_1a8);
    }
    if ((*(int *)(local_198 + 4) == 0) || (*(int *)(local_1a0 + 4) == 0)) {
      uVar10 = thunk_FUN_140034c40(local_1c0);
      cVar3 = thunk_FUN_140034800(uVar10,40000);
      thunk_FUN_140033660(local_1c0);
      if (cVar3 != '\0') {
        thunk_FUN_140010ab0(&local_178,2,1);
        thunk_FUN_140010400(&local_178,"Failed to obtain AD GUID and DN in anyway");
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_1c0);
        local_1d8 = 0x102;
        thunk_FUN_140033e30(uVar11,40000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_1c0);
        thunk_FUN_140011240(local_68);
        thunk_FUN_1400111a0(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
      local_1af = true;
    }
    else {
      thunk_FUN_140022990(param_1,&local_1a0,param_2);
    }
    local_1c8 = QString::fromAscii_helper("fullName",8);
    uVar17 = uVar17 | 2;
    local_1ac = uVar17;
    lVar12 = thunk_FUN_140013a80(*param_2,&local_1c8);
    if (lVar12 != 0) {
      bVar1 = false;
      goto LAB_140024f0f;
    }
  }
  bVar1 = true;
LAB_140024f0f:
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
      thunk_FUN_1400025c7(0);
      lpBuffer = pppuVar13;
    }
    BVar5 = GetUserNameW((LPWSTR)lpBuffer,(LPDWORD)&local_1c8);
    if (BVar5 == 0) {
      uVar10 = thunk_FUN_140034c40(local_1c0);
      cVar3 = thunk_FUN_140034800(uVar10,40000);
      thunk_FUN_140033660(local_1c0);
      if (cVar3 != '\0') {
        thunk_FUN_140010ab0(&local_178,2,1);
        DVar4 = GetLastError();
        pbVar15 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_140010400(&local_178,"GetUserNameW failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar15,DVar4);
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_1c0);
        local_1d8 = 0x119;
        thunk_FUN_140033e30(uVar11,40000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_1c0);
        thunk_FUN_140011240(local_68);
        thunk_FUN_1400111a0(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
    else {
      pQVar9 = (QString *)QString::fromWCharArray((wchar_t *)&local_188,(int)lpBuffer);
      QVariant::QVariant((QVariant *)local_1c0,pQVar9);
      local_1a8 = QString::fromAscii_helper("fullName",8);
      pQVar14 = (QVariant *)thunk_FUN_140011760(param_2,&local_1a8);
      QVariant::operator=(pQVar14,(QVariant *)local_1c0);
      QString::~QString((QString *)&local_1a8);
      QVariant::~QVariant((QVariant *)local_1c0);
      QString::~QString((QString *)&local_188);
    }
    thunk_FUN_1400025c7(lpBuffer);
  }
  if (*(int *)(local_1a0 + 4) != 0) {
    QVariant::QVariant((QVariant *)local_1c0,(QString *)&local_1a0);
    local_1c8 = QString::fromAscii_helper("distinguishedName",0x11);
    pQVar14 = (QVariant *)thunk_FUN_140011760(param_2,&local_1c8);
    QVariant::operator=(pQVar14,(QVariant *)local_1c0);
    QString::~QString((QString *)&local_1c8);
    QVariant::~QVariant((QVariant *)local_1c0);
  }
  if (*(int *)(local_198 + 4) != 0) {
    pQVar9 = (QString *)QString::toLower((QString *)&local_198);
    QVariant::QVariant((QVariant *)local_1c0,pQVar9);
    local_1c8 = QString::fromAscii_helper("GUID",4);
    pQVar14 = (QVariant *)thunk_FUN_140011760(param_2,&local_1c8);
    QVariant::operator=(pQVar14,(QVariant *)local_1c0);
    QString::~QString((QString *)&local_1c8);
    QVariant::~QVariant((QVariant *)local_1c0);
    QString::~QString((QString *)&local_190);
  }
  QVariant::QVariant((QVariant *)local_1c0,bVar2);
  local_1c8 = QString::fromAscii_helper("local",5);
  pQVar14 = (QVariant *)thunk_FUN_140011760(param_2,&local_1c8);
  QVariant::operator=(pQVar14,(QVariant *)local_1c0);
  QString::~QString((QString *)&local_1c8);
  QVariant::~QVariant((QVariant *)local_1c0);
  QVariant::QVariant((QVariant *)local_1c0,local_1af);
  local_1c8 = QString::fromAscii_helper("fail",4);
  pQVar14 = (QVariant *)thunk_FUN_140011760(param_2,&local_1c8);
  QVariant::operator=(pQVar14,(QVariant *)local_1c0);
  QString::~QString((QString *)&local_1c8);
  QVariant::~QVariant((QVariant *)local_1c0);
  pQVar9 = (QString *)QStandardPaths::writableLocation((StandardLocation)&local_190);
  QVariant::QVariant((QVariant *)local_1c0,pQVar9);
  local_1c8 = QString::fromAscii_helper("home",4);
  pQVar14 = (QVariant *)thunk_FUN_140011760(param_2,&local_1c8);
  QVariant::operator=(pQVar14,(QVariant *)local_1c0);
  QString::~QString((QString *)&local_1c8);
  QVariant::~QVariant((QVariant *)local_1c0);
  QString::~QString((QString *)&local_190);
  QString::~QString(local_88);
  QString::~QString((QString *)&local_198);
  QString::~QString((QString *)&local_1a0);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140025830
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_140025830(longlong param_1,QString *param_2,QString *param_3,QString *param_4)

{
  GUID *rguid;
  longlong lVar1;
  ulonglong uVar2;
  char cVar3;
  int iVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined8 uVar7;
  BSTR bstrString;
  LPOLESTR lpsz;
  QString *pQVar8;
  basic_ostream<char,std::char_traits<char>_> *pbVar9;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar10;
  ulonglong extraout_RAX;
  ushort *puVar11;
  undefined2 *puVar12;
  uint uVar13;
  longlong lVar14;
  undefined1 auStack_2c8 [32];
  longlong *local_2a8;
  longlong **local_2a0;
  uint local_298;
  longlong *local_290;
  longlong local_288;
  longlong local_280;
  longlong *local_278;
  int iStack_26c;
  undefined *local_268;
  undefined **local_260;
  basic_ostream<char,std::char_traits<char>_> local_258 [96];
  undefined8 local_1f8;
  undefined4 local_1f0;
  basic_ios<char,std::char_traits<char>_> local_1e0 [104];
  wchar_t *local_178;
  undefined8 local_170;
  longlong local_168;
  undefined8 local_160;
  undefined8 local_158;
  undefined ***local_150;
  wchar_t *local_148;
  wchar_t *local_140;
  _union_2707 local_138;
  QString local_120 [8];
  undefined8 local_118;
  undefined1 local_110 [16];
  undefined4 local_100 [2];
  undefined4 local_f8;
  undefined4 local_f0;
  undefined8 local_d8 [3];
  ulonglong local_c0;
  ushort local_b8;
  undefined6 uStack_b6;
  undefined8 local_a8;
  ulonglong local_a0;
  undefined2 local_98;
  undefined6 uStack_96;
  undefined8 local_88;
  ulonglong local_80;
  undefined1 local_78 [32];
  ulonglong local_58;
  
  local_118 = 0xfffffffffffffffe;
  local_58 = DAT_140763590 ^ (ulonglong)auStack_2c8;
  local_298 = 0;
  if (*(int *)(*(longlong *)param_2 + 4) == 0) {
    uVar5 = thunk_FUN_140034c40(&local_148);
    cVar3 = thunk_FUN_140034800(uVar5,40000);
    uVar6 = thunk_FUN_140033660(&local_148);
    if (cVar3 == '\0') goto LAB_140026a2c;
    local_268 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1e0)
    ;
    local_298 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_150 = &local_260;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1f8 = 0;
    local_1f0 = 4;
    thunk_FUN_140010400(&local_268,"User Sid is empty");
    uVar5 = thunk_FUN_140016d10(&local_268,&local_b8);
    uVar7 = thunk_FUN_140034c40(&local_148);
    local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x15c);
    thunk_FUN_140033e30(uVar7,40000,uVar5,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                       );
    thunk_FUN_140033660(&local_148);
    if (0xf < local_a0) {
      uVar2 = CONCAT62(uStack_b6,local_b8);
      uVar6 = uVar2;
      if (0xfff < local_a0 + 1) {
        if ((local_b8 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(uVar2 - 8);
        if (uVar2 <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (uVar2 - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < uVar2 - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c7(uVar6);
    }
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
  }
  else {
    if ((*(longlong *)(param_1 + 0x18) != 0) && (*(longlong *)(param_1 + 8) != 0)) {
      QString::clear(param_3);
      QString::clear(param_4);
      local_278 = (longlong *)0x0;
      local_290 = (longlong *)0x0;
      VariantInit((VARIANTARG *)&local_138.n2);
      local_2a0 = &local_278;
      local_2a8 = (longlong *)&DAT_1400de268;
      iVar4 = (**(code **)(param_1 + 0x18))(L"LDAP://rootDSE",0,0,1);
      if ((iVar4 < 0) || (local_278 == (longlong *)0x0)) {
        uVar5 = thunk_FUN_140034c40(local_d8);
        cVar3 = thunk_FUN_140034800(uVar5,40000);
        thunk_FUN_140033660(local_d8);
        if (cVar3 == '\0') goto LAB_140026802;
        local_268 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_1e0);
        local_298 = 0x100;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
        *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
             *(int *)(local_268 + 4) + -0x88;
        thunk_FUN_140010c30(&local_260,2);
        pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140010400(&local_268,
                                     "Cannot execute query. Cannot bind to LDAP://rootDSE. ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,iVar4);
        uVar5 = thunk_FUN_140016d10(&local_268,&local_b8);
        uVar7 = thunk_FUN_140034c40(local_d8);
        local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x21b);
        thunk_FUN_140033e30(uVar7,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_d8);
        if (0xf < local_a0) {
          uVar2 = CONCAT62(uStack_b6,local_b8);
          uVar6 = uVar2;
          if (0xfff < local_a0 + 1) {
            if ((local_b8 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(uVar2 - 8);
            if (uVar2 <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (uVar2 - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < uVar2 - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar6);
        }
      }
      else {
        bstrString = SysAllocString(L"defaultNamingContext");
        iVar4 = (**(code **)(*local_278 + 0x78))(local_278,bstrString);
        SysFreeString(bstrString);
        if (((-1 < iVar4) && (local_138.n2.vt == 8)) && (local_138.decVal.u2.Lo64 != 0)) {
          local_a0 = 7;
          local_a8 = 0;
          local_b8 = 0;
          thunk_FUN_140022520(&local_b8,L"LDAP://",7);
          SysStringLen((BSTR)local_138.decVal.u2);
          thunk_FUN_1400221b0(&local_b8,local_138.decVal.u2.Lo64);
          puVar11 = &local_b8;
          if (7 < local_a0) {
            puVar11 = (ushort *)CONCAT62(uStack_b6,local_b8);
          }
          local_2a0 = &local_290;
          local_2a8 = (longlong *)&DAT_1400de278;
          iVar4 = (**(code **)(param_1 + 0x18))(puVar11,0,0,1);
          if ((iVar4 < 0) || (local_290 == (longlong *)0x0)) {
            uVar5 = thunk_FUN_140034c40(local_d8);
            cVar3 = thunk_FUN_140034800(uVar5,40000);
            thunk_FUN_140033660(local_d8);
            if (cVar3 != '\0') {
              local_268 = &DAT_1400dbe28;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_1e0);
              local_298 = 0x40;
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
              *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
                   *(int *)(local_268 + 4) + -0x88;
              thunk_FUN_140010c30(&local_260,2);
              pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                       thunk_FUN_140010400(&local_268,
                                           "Cannot execute query. Cannot bind to the container ");
              pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,iVar4);
              std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        ((basic_ostream<char,std::char_traits<char>_> *)pbVar10,local_290);
              uVar5 = thunk_FUN_140016d10(&local_268,local_78);
              uVar7 = thunk_FUN_140034c40(local_d8);
              local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x211);
              thunk_FUN_140033e30(uVar7,40000,uVar5,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                                 );
              thunk_FUN_140033660(local_d8);
              thunk_FUN_140011240(local_78);
              thunk_FUN_1400111a0(local_1e0);
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_1e0);
            }
          }
          else {
            local_80 = 7;
            local_88 = 0;
            local_98 = 0;
            thunk_FUN_140022520(&local_98,L"(&(objectClass=user)(objectCategory=person)(objectSid=",
                                0x36);
            uVar5 = QString::toStdWString(param_2);
            thunk_FUN_140022010(&local_98,uVar5,0,0xffffffffffffffff);
            if (7 < local_c0) {
              thunk_FUN_1400227c0(local_d8,local_d8[0],local_c0 + 1);
            }
            thunk_FUN_1400221b0(&local_98,&DAT_1400deac4,2);
            local_148 = L"distinguishedName";
            local_140 = L"objectGUID";
            local_100[0] = 5;
            local_f8 = 7;
            local_f0 = 2;
            local_288 = 0;
            local_178 = (wchar_t *)0x0;
            local_170 = 0;
            local_168 = 0;
            local_160 = 0;
            local_158 = 0;
            local_280 = 0;
            iVar4 = (**(code **)(*local_290 + 0x18))(local_290,local_100);
            if (iVar4 < 0) {
              uVar5 = thunk_FUN_140034c40(local_d8);
              cVar3 = thunk_FUN_140034800(uVar5,40000);
              thunk_FUN_140033660(local_d8);
              if (cVar3 != '\0') {
                local_268 = &DAT_1400dbe28;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_1e0);
                local_298 = 0x20;
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                           (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
                *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
                     *(int *)(local_268 + 4) + -0x88;
                thunk_FUN_140010c30(&local_260,2);
                thunk_FUN_140010400(&local_268,"Failed to set search preference");
                uVar5 = thunk_FUN_140016d10(&local_268,local_78);
                uVar7 = thunk_FUN_140034c40(local_d8);
                local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x20b);
                thunk_FUN_140033e30(uVar7,40000,uVar5,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                                   );
                thunk_FUN_140033660(local_d8);
                thunk_FUN_140011240(local_78);
                thunk_FUN_1400111a0(local_1e0);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_1e0);
              }
            }
            else {
              lpsz = operator_new(0x4e);
              puVar12 = &local_98;
              if (7 < local_80) {
                puVar12 = (undefined2 *)CONCAT62(uStack_96,local_98);
              }
              local_2a8 = &local_280;
              iVar4 = (**(code **)(*local_290 + 0x20))(local_290,puVar12,&local_148,2);
              if ((iVar4 < 0) || (local_280 == 0)) {
                uVar5 = thunk_FUN_140034c40(local_d8);
                cVar3 = thunk_FUN_140034800(uVar5,40000);
                thunk_FUN_140033660(local_d8);
                if (cVar3 != '\0') {
                  local_268 = &DAT_1400dbe28;
                  std::basic_ios<char,std::char_traits<char>_>::
                  basic_ios<char,std::char_traits<char>_>(local_1e0);
                  local_298 = 0x10;
                  std::basic_ostream<char,std::char_traits<char>_>::
                  basic_ostream<char,std::char_traits<char>_>
                            ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                             (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
                  *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
                       std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                       vftable;
                  *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
                       *(int *)(local_268 + 4) + -0x88;
                  thunk_FUN_140010c30(&local_260,2);
                  thunk_FUN_140010400(&local_268,"Failed to execute search");
                  uVar5 = thunk_FUN_140016d10(&local_268,local_78);
                  uVar7 = thunk_FUN_140034c40(local_d8);
                  local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x205);
                  thunk_FUN_140033e30(uVar7,40000,uVar5,
                                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                                     );
                  thunk_FUN_140033660(local_d8);
                  thunk_FUN_140011240(local_78);
                  thunk_FUN_1400111a0(local_1e0);
                  std::basic_ios<char,std::char_traits<char>_>::
                  ~basic_ios<char,std::char_traits<char>_>(local_1e0);
                }
              }
              else {
                iVar4 = (**(code **)(*local_290 + 0x30))();
                while (iVar4 == 0) {
                  local_288 = 0;
                  iVar4 = (**(code **)(*local_290 + 0x48))(local_290,local_280);
                  while (iVar4 != 0x5013) {
                    uVar6 = 0;
                    if (iVar4 < 0) {
                      if (local_288 != 0) {
                        (**(code **)(param_1 + 8))();
                        local_288 = 0;
                      }
LAB_140025f15:
                      uVar5 = thunk_FUN_140034c40(local_110);
                      cVar3 = thunk_FUN_140034800(uVar5,40000);
                      thunk_FUN_140033660(local_110);
                      if (cVar3 != '\0') {
                        local_268 = &DAT_1400dbe28;
                        std::basic_ios<char,std::char_traits<char>_>::
                        basic_ios<char,std::char_traits<char>_>(local_1e0);
                        local_298 = local_298 | 4;
                        std::basic_ostream<char,std::char_traits<char>_>::
                        basic_ostream<char,std::char_traits<char>_>
                                  ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                                   (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false
                                  );
                        *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
                             std::
                             basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>
                             ::vftable;
                        *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
                             *(int *)(local_268 + 4) + -0x88;
                        thunk_FUN_140010c30(&local_260,2);
                        pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                                 thunk_FUN_140010400(&local_268,"Failed to get next search column: "
                                                    );
                        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,iVar4);
                        uVar5 = thunk_FUN_140016d10(&local_268,local_78);
                        uVar7 = thunk_FUN_140034c40(local_d8);
                        local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x1c1);
                        thunk_FUN_140033e30(uVar7,40000,uVar5,
                                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                                           );
                        thunk_FUN_140033660(local_d8);
                        thunk_FUN_140011240(local_78);
                        thunk_FUN_1400111a0(local_1e0);
                        std::basic_ios<char,std::char_traits<char>_>::
                        ~basic_ios<char,std::char_traits<char>_>(local_1e0);
                      }
                      break;
                    }
                    if (local_288 == 0) goto LAB_140025f15;
                    local_178 = (wchar_t *)0x0;
                    local_170 = 0;
                    local_168 = 0;
                    local_160 = 0;
                    local_158 = 0;
                    iVar4 = (**(code **)(*local_290 + 0x50))
                                      (local_290,local_280,local_288,&local_178);
                    if (-1 < iVar4) {
                      if (local_168 != 0) {
                        if ((int)local_170 == 1) {
                          uVar5 = local_160;
                          lVar14 = local_168;
                          if ((int)local_160 != 0) {
                            do {
                              lVar1 = *(longlong *)(lVar14 + 8 + uVar6 * 0x18);
                              if (lVar1 != 0) {
                                pQVar8 = (QString *)
                                         QString::fromWCharArray((wchar_t *)&local_150,(int)lVar1);
                                QString::operator=(param_3,pQVar8);
                                QString::~QString((QString *)&local_150);
                                uVar5 = local_160;
                                lVar14 = local_168;
                                if (*(int *)(*(longlong *)param_3 + 4) != 0) break;
                              }
                              uVar13 = (int)uVar6 + 1;
                              uVar6 = (ulonglong)uVar13;
                            } while (uVar13 < (uint)uVar5);
                          }
                        }
                        else if (((((int)local_170 == 8) && (local_178 != (wchar_t *)0x0)) &&
                                 (iVar4 = _wcsicmp(local_178,L"objectGUID"), iVar4 == 0)) &&
                                (uVar5 = local_160, (int)local_160 != 0)) {
                          do {
                            rguid = *(GUID **)(local_168 + 0x10 + uVar6 * 0x18);
                            if ((rguid != (GUID *)0x0) &&
                               (0xf < *(uint *)(local_168 + 8 + uVar6 * 0x18))) {
                              StringFromGUID2(rguid,lpsz,0x27);
                              pQVar8 = (QString *)
                                       QString::fromWCharArray((wchar_t *)local_120,(int)lpsz);
                              QString::operator=(param_4,pQVar8);
                              QString::~QString(local_120);
                              uVar5 = local_160;
                              if (*(int *)(*(longlong *)param_4 + 4) != 0) break;
                            }
                            uVar13 = (int)uVar6 + 1;
                            uVar6 = (ulonglong)uVar13;
                          } while (uVar13 < (uint)uVar5);
                        }
                      }
                      (**(code **)(*local_290 + 0x58))(local_290,&local_178);
                    }
                    (**(code **)(param_1 + 8))(local_288);
                    local_288 = 0;
                    iVar4 = (**(code **)(*local_290 + 0x48))(local_290,local_280);
                  }
                  iVar4 = (**(code **)(*local_290 + 0x38))(local_290,local_280);
                }
                if (iVar4 != 0x5012) {
                  uVar5 = thunk_FUN_140034c40(local_d8);
                  cVar3 = thunk_FUN_140034800(uVar5,40000);
                  thunk_FUN_140033660(local_d8);
                  if (cVar3 != '\0') {
                    local_268 = &DAT_1400dbe28;
                    std::basic_ios<char,std::char_traits<char>_>::
                    basic_ios<char,std::char_traits<char>_>(local_1e0);
                    local_298 = local_298 | 8;
                    std::basic_ostream<char,std::char_traits<char>_>::
                    basic_ostream<char,std::char_traits<char>_>
                              ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                               (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
                    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
                         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>
                         ::vftable;
                    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
                         *(int *)(local_268 + 4) + -0x88;
                    thunk_FUN_140010c30(&local_260,2);
                    pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                             thunk_FUN_140010400(&local_268,"Failed to enumerate search rows: ");
                    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,iVar4);
                    uVar5 = thunk_FUN_140016d10(&local_268,local_78);
                    uVar7 = thunk_FUN_140034c40(local_d8);
                    local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x1ff);
                    thunk_FUN_140033e30(uVar7,40000,uVar5,
                                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                                       );
                    thunk_FUN_140033660(local_d8);
                    thunk_FUN_140011240(local_78);
                    thunk_FUN_1400111a0(local_1e0);
                    std::basic_ios<char,std::char_traits<char>_>::
                    ~basic_ios<char,std::char_traits<char>_>(local_1e0);
                  }
                }
                (**(code **)(*local_290 + 0x60))(local_290,local_280);
              }
              thunk_FUN_1400025c7(lpsz);
            }
            (**(code **)(*local_290 + 0x10))();
            if (7 < local_80) {
              thunk_FUN_1400227c0(&local_98,CONCAT62(uStack_96,local_98),local_80 + 1);
            }
          }
          if (7 < local_a0) {
            thunk_FUN_1400227c0(&local_b8,CONCAT62(uStack_b6,local_b8),local_a0 + 1);
          }
          goto LAB_140026802;
        }
        uVar5 = thunk_FUN_140034c40(local_d8);
        cVar3 = thunk_FUN_140034800(uVar5,40000);
        thunk_FUN_140033660(local_d8);
        if (cVar3 == '\0') goto LAB_140026802;
        local_268 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_1e0);
        local_298 = 0x80;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
        *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
             *(int *)(local_268 + 4) + -0x88;
        thunk_FUN_140010c30(&local_260,2);
        thunk_FUN_140010400(&local_268,"Failed to get object");
        uVar5 = thunk_FUN_140016d10(&local_268,local_78);
        uVar7 = thunk_FUN_140034c40(local_d8);
        local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x216);
        thunk_FUN_140033e30(uVar7,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_d8);
        thunk_FUN_140011240(local_78);
      }
      thunk_FUN_1400111a0(local_1e0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1e0);
LAB_140026802:
      VariantClear((VARIANTARG *)&local_138.n2);
      if (local_278 != (longlong *)0x0) {
        (**(code **)(*local_278 + 0x10))();
      }
      if ((*(int *)(*(longlong *)param_3 + 4) != 0) && (*(int *)(*(longlong *)param_4 + 4) != 0)) {
        return 1;
      }
      return 0;
    }
    uVar5 = thunk_FUN_140034c40(local_d8);
    cVar3 = thunk_FUN_140034800(uVar5,40000);
    uVar6 = thunk_FUN_140033660(local_d8);
    if (cVar3 == '\0') goto LAB_140026a2c;
    local_268 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1e0)
    ;
    local_298 = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_150 = &local_260;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1f8 = 0;
    local_1f0 = 4;
    pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010400(&local_268,"ADsOpenObject or FreeADsMem failed: ");
    pbVar10 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar9,*(void **)(param_1 + 0x18));
    std::basic_ostream<char,std::char_traits<char>_>::operator<<
              ((basic_ostream<char,std::char_traits<char>_> *)pbVar10,*(void **)(param_1 + 8));
    uVar5 = thunk_FUN_140016d10(&local_268,&local_b8);
    uVar7 = thunk_FUN_140034c40(local_d8);
    local_2a8 = (longlong *)CONCAT44(local_2a8._4_4_,0x162);
    thunk_FUN_140033e30(uVar7,40000,uVar5,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                       );
    thunk_FUN_140033660(local_d8);
    if (0xf < local_a0) {
      uVar2 = CONCAT62(uStack_b6,local_b8);
      uVar6 = uVar2;
      if (0xfff < local_a0 + 1) {
        if ((local_b8 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(uVar2 - 8);
        if (uVar2 <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (uVar2 - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < uVar2 - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c7(uVar6);
    }
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
  }
  local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  thunk_FUN_140012670(&local_260);
  std::basic_streambuf<char,std::char_traits<char>_>::~basic_streambuf<char,std::char_traits<char>_>
            ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
  std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
            (local_258);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_1e0);
  uVar6 = extraout_RAX;
LAB_140026a2c:
  return uVar6 & 0xffffffffffffff00;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140026ee0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

ulonglong FUN_140026ee0(undefined8 param_1,longlong *param_2,QString *param_3,QString *param_4)

{
  char cVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  undefined8 uVar4;
  ulonglong extraout_RAX;
  QString *pQVar5;
  ulonglong extraout_RAX_00;
  longlong lVar6;
  undefined1 auStack_198 [32];
  char *local_178;
  undefined4 local_168 [2];
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
  local_30 = DAT_140763590 ^ (ulonglong)auStack_198;
  local_168[0] = 0;
  if (*(int *)(*param_2 + 4) == 0) {
    uVar2 = thunk_FUN_140034c40(local_160);
    cVar1 = thunk_FUN_140034800(uVar2,40000);
    uVar3 = thunk_FUN_140033660(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
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
      thunk_FUN_140010400(&local_148,"User Sid is empty");
      uVar2 = thunk_FUN_140016d10(&local_148,local_50);
      uVar4 = thunk_FUN_140034c40(local_160);
      local_178 = (char *)CONCAT44(local_178._4_4_,0x229);
      thunk_FUN_140033e30(uVar4,40000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_160);
      if (0xf < local_38) {
        uVar3 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar3 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar3) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar3 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar3) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar3);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
      uVar3 = extraout_RAX;
    }
  }
  else {
    uVar2 = thunk_FUN_140011970(local_160,
                                "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\State\\"
                                ,param_2);
    local_178 = "Distinguished-Name";
    pQVar5 = (QString *)thunk_FUN_140027460(param_1,local_168,&DAT_ffffffff80000002,uVar2);
    QString::operator=(param_3,pQVar5);
    QString::~QString((QString *)local_168);
    QString::~QString((QString *)local_160);
    uVar2 = thunk_FUN_140011970(local_168,
                                "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\ProfileList\\",
                                param_2);
    local_178 = "Guid";
    pQVar5 = (QString *)thunk_FUN_140027460(param_1,local_160,&DAT_ffffffff80000002,uVar2);
    QString::operator=(param_4,pQVar5);
    QString::~QString((QString *)local_160);
    QString::~QString((QString *)local_168);
    lVar6 = *(longlong *)param_3;
    if ((*(int *)(lVar6 + 4) != 0) || (lVar6 = *(longlong *)param_4, *(int *)(lVar6 + 4) != 0)) {
      return CONCAT71((int7)((ulonglong)lVar6 >> 8),1);
    }
    uVar2 = thunk_FUN_140034c40(local_160);
    cVar1 = thunk_FUN_140034800(uVar2,40000);
    uVar3 = thunk_FUN_140033660(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168[0] = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_140010c30(&local_140,2);
      thunk_FUN_140010400(&local_148,"Failed to get DN and GUID from registry");
      uVar2 = thunk_FUN_140016d10(&local_148,local_50);
      uVar4 = thunk_FUN_140034c40(local_160);
      local_178 = (char *)CONCAT44(local_178._4_4_,0x236);
      thunk_FUN_140033e30(uVar4,40000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_160);
      if (0xf < local_38) {
        uVar3 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar3 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar3) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar3 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar3) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar3);
      }
      thunk_FUN_1400111a0(local_c0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
      return extraout_RAX_00 & 0xffffffffffffff00;
    }
  }
  return uVar3 & 0xffffffffffffff00;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140027460
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QString * FUN_140027460(undefined8 param_1,QString *param_2,HKEY param_3,QString *param_4,
                       LPCSTR param_5)

{
  char cVar1;
  LSTATUS LVar2;
  QByteArray *this;
  char *lpSubKey;
  LPBYTE lpData;
  QString *pQVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  undefined1 auStackY_1c8 [32];
  DWORD local_194;
  undefined ***local_190 [2];
  DWORD local_180 [2];
  HKEY local_178;
  int iStack_16c;
  undefined *local_168;
  undefined **local_160;
  basic_ostream<char,std::char_traits<char>_> local_158 [96];
  undefined8 local_f8;
  undefined4 local_f0;
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  undefined8 local_78;
  QString *local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_78 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStackY_1c8;
  local_70 = param_2;
  QString::QString(param_2);
  this = (QByteArray *)QString::toLocal8Bit(param_4);
  lpSubKey = QByteArray::data(this);
  LVar2 = RegOpenKeyExA(param_3,lpSubKey,0,1,&local_178);
  QByteArray::~QByteArray((QByteArray *)local_180);
  if (LVar2 == 0) {
    local_194 = 0;
    LVar2 = RegQueryValueExA(local_178,param_5,(LPDWORD)0x0,local_180,(LPBYTE)0x0,&local_194);
    if (LVar2 == 0) {
      lpData = LocalAlloc(0x40,(ulonglong)local_194);
      LVar2 = RegQueryValueExA(local_178,param_5,(LPDWORD)0x0,local_180,lpData,&local_194);
      if (LVar2 == 0) {
        if (local_180[0] == 1) {
          pQVar3 = (QString *)QString::fromLocal8Bit((char *)local_190,(int)lpData);
          QString::operator=(param_2,pQVar3);
          QString::~QString((QString *)local_190);
        }
      }
      else {
        uVar4 = thunk_FUN_140034c40(local_190);
        cVar1 = thunk_FUN_140034800(uVar4,40000);
        thunk_FUN_140033660(local_190);
        if (cVar1 != '\0') {
          local_168 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_e0);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_168,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_160,false);
          *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
               *(int *)(local_168 + 4) + -0x88;
          thunk_FUN_140010c30(&local_160,2);
          pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_140010400(&local_168,"RegQueryValueExA failed 2: ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,LVar2);
          uVar4 = thunk_FUN_140016d10(&local_168,local_68);
          uVar6 = thunk_FUN_140034c40(local_190);
          thunk_FUN_140033e30(uVar6,40000,uVar4,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                             );
          thunk_FUN_140033660(local_190);
          if (0xf < local_50) {
            uVar7 = local_68[0];
            if (0xfff < local_50 + 1) {
              if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar7 = *(ulonglong *)(local_68[0] - 8);
              if (local_68[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_68[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_68[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c7(uVar7);
          }
          thunk_FUN_1400111a0(local_e0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_e0);
        }
      }
      LocalFree(lpData);
    }
    else {
      uVar4 = thunk_FUN_140034c40(local_190);
      cVar1 = thunk_FUN_140034800(uVar4,40000);
      thunk_FUN_140033660(local_190);
      if (cVar1 != '\0') {
        local_168 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_e0);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_168,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_160,false);
        *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
             *(int *)(local_168 + 4) + -0x88;
        thunk_FUN_140010c30(&local_160,2);
        pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_140010400(&local_168,"RegQueryValueExA failed: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,LVar2);
        uVar4 = thunk_FUN_140016d10(&local_168,local_68);
        uVar6 = thunk_FUN_140034c40(local_190);
        thunk_FUN_140033e30(uVar6,40000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                           );
        thunk_FUN_140033660(local_190);
        if (0xf < local_50) {
          uVar7 = local_68[0];
          if (0xfff < local_50 + 1) {
            if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_68[0] - 8);
            if (local_68[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_68[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_68[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar7);
        }
        thunk_FUN_1400111a0(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    RegCloseKey(local_178);
  }
  else {
    uVar4 = thunk_FUN_140034c40(local_190);
    cVar1 = thunk_FUN_140034800(uVar4,40000);
    thunk_FUN_140033660(local_190);
    if (cVar1 != '\0') {
      local_168 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_e0);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_168,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_160,false);
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
           *(int *)(local_168 + 4) + -0x88;
      local_190[0] = &local_160;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_f8 = 0;
      local_f0 = 4;
      pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_168,"RegOpenKeyExA failed: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,LVar2);
      uVar4 = thunk_FUN_140016d10(&local_168,local_68);
      uVar6 = thunk_FUN_140034c40(local_190);
      thunk_FUN_140033e30(uVar6,40000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-userInfo\\adsiQuery.cpp"
                         );
      thunk_FUN_140033660(local_190);
      if (0xf < local_50) {
        uVar7 = local_68[0];
        if (0xfff < local_50 + 1) {
          if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_68[0] - 8);
          if (local_68[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_68[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_68[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar7);
      }
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
           *(int *)(local_168 + 4) + -0x88;
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_160);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_158);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_e0);
    }
  }
  return param_2;
}

// =============================================================================
// CLIPBOARD SI LOCAL RPC
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14006f9f0
// -----------------------------------------------------------------------------
void FUN_14006f9f0(longlong param_1,int param_2)

{
  undefined8 uVar1;
  bool bVar2;
  QMimeData *this;
  QByteArray local_res8 [8];
  QByteArray local_res18 [16];
  QVariant local_40 [16];
  QImage local_30 [40];
  
  if (((*(byte *)(param_1 + 0x10) & 0x10) != 0) && (param_2 == 0)) {
    this = (QMimeData *)thunk_FUN_140076e40(param_1 + 0x18);
    if (this != (QMimeData *)0x0) {
      bVar2 = QMimeData::hasUrls(this);
      if (bVar2) {
        uVar1 = QMimeData::urls(this);
        thunk_FUN_140070400(param_1,uVar1);
      }
      else {
        bVar2 = QMimeData::hasHtml(this);
        if (bVar2) {
          uVar1 = QMimeData::html(this);
          thunk_FUN_1400702c0(param_1,uVar1);
        }
        else {
          bVar2 = QMimeData::hasText(this);
          if (bVar2) {
            uVar1 = QMimeData::text(this);
            thunk_FUN_1400703a0(param_1,uVar1);
          }
          else {
            bVar2 = QMimeData::hasImage(this);
            if (bVar2) {
              uVar1 = QMimeData::imageData(this);
              thunk_FUN_14006f1f0(local_30,uVar1);
              QVariant::~QVariant(local_40);
              bVar2 = QImage::isNull(local_30);
              if (!bVar2) {
                QByteArray::QByteArray(local_res8);
                QBuffer::QBuffer((QBuffer *)local_40,local_res8,(QObject *)0x0);
                bVar2 = QBuffer::open((QBuffer *)local_40,2);
                if (!bVar2) {
                  qt_assert("!\"wtf?\"",
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave\\clipboard.cpp"
                            ,0xc3);
                    /* WARNING: Subroutine does not return */
                  abort();
                }
                bVar2 = QImage::save(local_30,(QIODevice *)local_40,"PNG",-1);
                if (!bVar2) {
                  qt_assert("!\"wtf?\"",
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave\\clipboard.cpp"
                            ,0xc9);
                    /* WARNING: Subroutine does not return */
                  abort();
                }
                QBuffer::close((QBuffer *)local_40);
                uVar1 = QByteArray::QByteArray(local_res18,local_res8);
                thunk_FUN_140070320(param_1,uVar1);
                QBuffer::~QBuffer((QBuffer *)local_40);
                QByteArray::~QByteArray(local_res8);
              }
              QImage::~QImage(local_30);
            }
            else {
              thunk_FUN_140070380(param_1);
            }
          }
        }
      }
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14006fed0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14006fed0(QObject *param_1)

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
    lVar2 = thunk_FUN_14006f8d0();
    if (lVar2 == 0) {
      uVar3 = thunk_FUN_140034c40(local_160);
      cVar1 = thunk_FUN_140034800(uVar3,40000);
      thunk_FUN_140033660(local_160);
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
        thunk_FUN_140010c30(local_140,2);
        thunk_FUN_140010400(&local_148,"memory fail");
        uVar3 = thunk_FUN_140016d10(&local_148,local_58);
        uVar4 = thunk_FUN_140034c40(local_160);
        thunk_FUN_140033e30(uVar4,40000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberAgent-slave\\slave\\clipboard.cpp"
                           );
        thunk_FUN_140033660(local_160);
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
          FUN_1400025c7(uVar5);
        }
        thunk_FUN_1400111a0(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
    }
    else {
      local_168 = &LAB_140004089;
      local_160[0] = thunk_FUN_140077210;
      QObject::disconnectImpl
                (param_1 + 0x18,local_160,param_1,&local_168,(QMetaObject *)&DAT_140763188);
      thunk_FUN_140076fe0(param_1 + 0x18,lVar2,0);
      thunk_FUN_14006f0b0(local_160,param_1 + 0x18,thunk_FUN_140077210,param_1);
      QMetaObject::Connection::~Connection((Connection *)local_160);
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14007b810
// -----------------------------------------------------------------------------
undefined8 *
FUN_14007b810(longlong *param_1,undefined4 param_2,QString *param_3,undefined8 *param_4,
             undefined8 *param_5)

{
  undefined8 *puVar1;
  
  puVar1 = QHashData::allocateNode((QHashData *)*param_1,8);
  if (puVar1 == (undefined8 *)0x0) {
    puVar1 = (undefined8 *)0x0;
  }
  else {
    *puVar1 = *param_5;
    *(undefined4 *)(puVar1 + 1) = param_2;
    QString::QString((QString *)(puVar1 + 2),param_3);
    puVar1[3] = *param_4;
    QString::QString((QString *)(puVar1 + 4),(QString *)(param_4 + 1));
  }
  *param_5 = puVar1;
  *(int *)(*param_1 + 0x14) = *(int *)(*param_1 + 0x14) + 1;
  return puVar1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14007bb00
// -----------------------------------------------------------------------------
void FUN_14007bb00(longlong param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  
  if (param_2 != (undefined8 *)0x0) {
    uVar1 = *(undefined4 *)(param_1 + 8);
    *param_2 = 0;
    *(undefined4 *)(param_2 + 1) = uVar1;
    QString::QString((QString *)(param_2 + 2),(QString *)(param_1 + 0x10));
    param_2[3] = *(undefined8 *)(param_1 + 0x18);
    QString::QString((QString *)(param_2 + 4),(QString *)(param_1 + 0x20));
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14007c890
// -----------------------------------------------------------------------------
void FUN_14007c890(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  Data *pDVar1;
  int iVar2;
  QVariant *this;
  Data *pDVar3;
  Data *pDVar4;
  Data *local_res18;
  
  local_res18 = (Data *)shared_null_exref;
  thunk_FUN_14007bb70(param_1,param_2,&local_res18,param_4,0xfffffffffffffffe);
  pDVar3 = local_res18;
  if (*(int *)local_res18 != 0) {
    if (*(int *)local_res18 == -1) {
      return;
    }
    LOCK();
    iVar2 = *(int *)local_res18;
    *(int *)local_res18 = *(int *)local_res18 + -1;
    UNLOCK();
    if (iVar2 != 1) {
      return;
    }
  }
  pDVar4 = local_res18 + ((longlong)*(int *)(local_res18 + 0xc) + 2) * 8;
  pDVar1 = local_res18 + ((longlong)*(int *)(local_res18 + 8) + 2) * 8;
  while (pDVar1 != pDVar4) {
    pDVar4 = pDVar4 + -8;
    this = *(QVariant **)pDVar4;
    if (this != (QVariant *)0x0) {
      QVariant::~QVariant(this);
      thunk_FUN_1400025c7(this,0x10);
    }
  }
  QListData::dispose(pDVar3);
  return;
}

// =============================================================================
// HTTP SI WEB TOKEN
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400814c0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400814c0(longlong param_1,QNetworkReply *param_2)

{
  char cVar1;
  Algorithm AVar2;
  int iVar3;
  QSslConfiguration *this;
  QSslKey *this_00;
  QByteArray *pQVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  undefined1 auStack_1b8 [32];
  undefined4 local_198;
  undefined4 local_188 [4];
  QString local_178 [8];
  QSslCertificate local_170 [8];
  QByteArray local_168 [8];
  QByteArray local_160 [8];
  QByteArray local_158 [8];
  QSslKey local_150 [8];
  QByteArray local_148 [8];
  undefined8 local_140;
  undefined *local_138;
  basic_streambuf<char,std::char_traits<char>_> local_130 [128];
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_140 = 0xfffffffffffffffe;
  local_28 = DAT_140763590 ^ (ulonglong)auStack_1b8;
  local_188[0] = 0;
  if (*(int *)(*(longlong *)(param_1 + 0x78) + 4) != 0) {
    this = (QSslConfiguration *)QNetworkReply::sslConfiguration(param_2);
    QSslConfiguration::peerCertificate(this);
    QSslConfiguration::~QSslConfiguration((QSslConfiguration *)local_188);
    QByteArray::QByteArray(local_148);
    this_00 = (QSslKey *)QSslCertificate::publicKey(local_170);
    AVar2 = QSslKey::toDer(this_00,local_158);
    pQVar4 = (QByteArray *)QCryptographicHash::hash(local_160,AVar2);
    pQVar4 = (QByteArray *)QByteArray::toBase64(pQVar4);
    QString::QString(local_178,pQVar4);
    QByteArray::~QByteArray(local_168);
    QByteArray::~QByteArray(local_160);
    QByteArray::~QByteArray(local_158);
    QSslKey::~QSslKey(local_150);
    QByteArray::~QByteArray(local_148);
    iVar3 = QString::compare((QString *)(param_1 + 0x78),local_178,1);
    if (iVar3 != 0) {
      uVar5 = thunk_FUN_140034c40(local_188);
      cVar1 = thunk_FUN_140034800(uVar5,40000);
      thunk_FUN_140033660(local_188);
      if (cVar1 != '\0') {
        local_138 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_188[0] = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
        *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_140010c30(local_130,2);
        thunk_FUN_140010400(&local_138,"SSL pinning failed. Invalid server hash");
        uVar5 = thunk_FUN_140016d10(&local_138,local_48);
        uVar6 = thunk_FUN_140034c40(local_188);
        local_198 = 0xd7;
        thunk_FUN_140033e30(uVar6,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033660(local_188);
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
          FUN_1400025c7(uVar7);
        }
        thunk_FUN_1400111a0(local_b0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_b0);
      }
      (**(code **)(*(longlong *)param_2 + 0xe8))(param_2);
    }
    QString::~QString(local_178);
    QSslCertificate::~QSslCertificate(local_170);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140081bd0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140081bd0(longlong param_1,QNetworkReply *param_2)

{
  char cVar1;
  bool bVar2;
  NetworkError NVar3;
  int iVar4;
  undefined8 uVar5;
  QString *pQVar6;
  basic_ostream<char,std::char_traits<char>_> *pbVar7;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar8;
  QByteArray *pQVar9;
  char *pcVar10;
  undefined8 uVar11;
  QVariant *this;
  QJsonValueRef *this_00;
  ulonglong uVar12;
  uint uVar13;
  uint uVar14;
  undefined1 auStack_1c8 [32];
  undefined4 local_1a8;
  undefined8 local_198;
  QVariant local_190 [16];
  QVariant local_180 [16];
  undefined ***local_170;
  QByteArray local_168 [8];
  undefined8 local_160;
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
  local_40 = DAT_140763590 ^ (ulonglong)auStack_1c8;
  uVar13 = 0;
  local_198 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_198 & 0xffffffff00000000);
  *(undefined4 *)(param_1 + 0xc0) = 1;
  NVar3 = QNetworkReply::error(param_2);
  if (NVar3 == 0) {
    this = (QVariant *)QNetworkReply::attribute(param_2,(Attribute)local_190);
    iVar4 = QVariant::toInt(this,(bool *)0x0);
    QVariant::~QVariant(local_190);
    if (iVar4 - 200U < 100) {
      uVar5 = thunk_FUN_140083150();
      cVar1 = thunk_FUN_140085520(uVar5);
      if (cVar1 != '\0') {
        uVar5 = thunk_FUN_140034c40(local_190);
        cVar1 = thunk_FUN_140034800(uVar5,10000);
        thunk_FUN_140033660(local_190);
        if (cVar1 != '\0') {
          local_158 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_d0);
          uVar13 = 4;
          local_198 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_198._4_4_,4);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
          *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
               *(int *)(local_158 + 4) + -0x88;
          thunk_FUN_140010c30(&local_150,2);
          thunk_FUN_140010400(&local_158,"got server reply");
          uVar5 = thunk_FUN_140016d10(&local_158,local_60);
          uVar11 = thunk_FUN_140034c40(local_190);
          local_1a8 = 0x102;
          thunk_FUN_140033e30(uVar11,10000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                             );
          thunk_FUN_140033660(local_190);
          if (0xf < local_48) {
            uVar12 = local_60[0];
            if (0xfff < local_48 + 1) {
              if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar12 = *(ulonglong *)(local_60[0] - 8);
              if (local_60[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_60[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_60[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_1400025c7(uVar12);
          }
          thunk_FUN_1400111a0(local_d0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_d0);
        }
      }
      QIODevice::readAll((QIODevice *)param_2);
      if (0 < *(int *)(local_160 + 4)) {
        uVar5 = thunk_FUN_140083150();
        cVar1 = thunk_FUN_140085520(uVar5);
        uVar14 = uVar13;
        if (cVar1 != '\0') {
          uVar5 = thunk_FUN_140034c40(local_190);
          cVar1 = thunk_FUN_140034800(uVar5,10000);
          thunk_FUN_140033660(local_190);
          if (cVar1 != '\0') {
            local_158 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_d0);
            uVar14 = uVar13 | 8;
            local_198 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_198._4_4_,uVar13) | 8);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
            *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
                 *(int *)(local_158 + 4) + -0x88;
            thunk_FUN_140010c30(&local_150,2);
            pQVar6 = (QString *)QString::QString((QString *)&local_170,(QByteArray *)&local_160);
            pQVar9 = (QByteArray *)QString::toLocal8Bit(pQVar6);
            pcVar10 = QByteArray::data(pQVar9);
            thunk_FUN_140010400(&local_158,pcVar10);
            QByteArray::~QByteArray((QByteArray *)&local_198);
            QString::~QString((QString *)&local_170);
            uVar5 = thunk_FUN_140016d10(&local_158,local_60);
            uVar11 = thunk_FUN_140034c40(local_190);
            local_1a8 = 0x106;
            thunk_FUN_140033e30(uVar11,10000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                               );
            thunk_FUN_140033660(local_190);
            thunk_FUN_140011240(local_60);
            thunk_FUN_1400111a0(local_d0);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_d0);
          }
        }
        QJsonDocument::fromJson(local_168,(QJsonParseError *)&local_160);
        bVar2 = QJsonDocument::isObject((QJsonDocument *)local_168);
        if (bVar2) {
          QJsonDocument::object((QJsonDocument *)local_168);
          local_198 = QString::fromAscii_helper("skp_version",0xb);
          bVar2 = QJsonObject::contains((QJsonObject *)local_190,(QString *)&local_198);
          QString::~QString((QString *)&local_198);
          if (bVar2) {
            local_198 = QString::fromAscii_helper("skp_version",0xb);
            this_00 = (QJsonValueRef *)
                      QJsonObject::operator[]((QJsonObject *)local_190,(QString *)local_60);
            uVar5 = QJsonValueRef::toVariant(this_00);
            uVar5 = thunk_FUN_14007e110(uVar5,&local_170);
            thunk_FUN_140085bb0(param_1,uVar5);
            QString::~QString((QString *)&local_170);
            QVariant::~QVariant(local_180);
            QString::~QString((QString *)&local_198);
          }
          else {
            uVar5 = thunk_FUN_140083150();
            cVar1 = thunk_FUN_140085520(uVar5);
            if (cVar1 != '\0') {
              uVar5 = thunk_FUN_140034c40(local_180);
              cVar1 = thunk_FUN_140034800(uVar5,10000);
              thunk_FUN_140033660(local_180);
              if (cVar1 != '\0') {
                local_158 = &DAT_1400dbe28;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_d0);
                local_198 = (QTypedArrayData<unsigned_short> *)
                            (CONCAT44(local_198._4_4_,uVar14) | 0x10);
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                           (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
                *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
                     *(int *)(local_158 + 4) + -0x88;
                thunk_FUN_140010c30(&local_150,2);
                thunk_FUN_140010400(&local_158,"server response does not contain skp version");
                uVar5 = thunk_FUN_140016d10(&local_158,local_60);
                uVar11 = thunk_FUN_140034c40(local_180);
                local_1a8 = 0x10e;
                thunk_FUN_140033e30(uVar11,10000,uVar5,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                                   );
                thunk_FUN_140033660(local_180);
                thunk_FUN_140011240(local_60);
                thunk_FUN_1400111a0(local_d0);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_d0);
              }
            }
          }
          cVar1 = FUN_140081000(local_190);
          if (cVar1 == '\0') {
            cVar1 = FUN_140080e90(local_190);
            if (cVar1 != '\0') {
              uVar5 = QJsonObject::QJsonObject((QJsonObject *)local_180,(QJsonObject *)local_190);
              thunk_FUN_140086030(param_1,uVar5);
            }
          }
          else {
            uVar5 = QJsonObject::QJsonObject((QJsonObject *)local_180,(QJsonObject *)local_190);
            thunk_FUN_140086090(param_1,uVar5);
          }
          QJsonObject::~QJsonObject((QJsonObject *)local_190);
        }
        else {
          uVar5 = thunk_FUN_140034c40(local_180);
          cVar1 = thunk_FUN_140034800(uVar5,30000);
          thunk_FUN_140033660(local_180);
          if (cVar1 != '\0') {
            local_158 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_d0);
            local_198 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_198._4_4_,uVar14) | 0x20)
            ;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
            *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
                 *(int *)(local_158 + 4) + -0x88;
            thunk_FUN_140010c30(&local_150,2);
            thunk_FUN_140010400(&local_158,"server response is not an object");
            uVar5 = thunk_FUN_140016d10(&local_158,local_60);
            uVar11 = thunk_FUN_140034c40(local_180);
            local_1a8 = 0x117;
            thunk_FUN_140033e30(uVar11,30000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                               );
            thunk_FUN_140033660(local_180);
            thunk_FUN_140011240(local_60);
            thunk_FUN_1400111a0(local_d0);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_d0);
          }
        }
        thunk_FUN_140085890(param_1,1);
        QJsonDocument::~QJsonDocument((QJsonDocument *)local_168);
      }
      QObject::deleteLater((QObject *)param_2);
      thunk_FUN_140083b60(param_1);
      QByteArray::~QByteArray((QByteArray *)&local_160);
      return;
    }
    thunk_FUN_140080270(param_1 + 0x98);
    uVar5 = thunk_FUN_140034c40(local_180);
    cVar1 = thunk_FUN_140034800(uVar5,40000);
    thunk_FUN_140033660(local_180);
    if (cVar1 != '\0') {
      local_158 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      local_198 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_198._4_4_,2);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
           *(int *)(local_158 + 4) + -0x88;
      local_170 = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_158,"server responded with bad status code: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,iVar4);
      uVar5 = thunk_FUN_140016d10(&local_158,local_60);
      uVar11 = thunk_FUN_140034c40(local_180);
      local_1a8 = 0xfc;
      thunk_FUN_140033e30(uVar11,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033660(local_180);
      if (0xf < local_48) {
        uVar12 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar12 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar12);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
    thunk_FUN_140085890(param_1,0);
  }
  else {
    thunk_FUN_140080270(param_1 + 0x98);
    QString::operator=((QString *)(param_1 + 0x90),"0.0");
    thunk_FUN_140085890(param_1,0);
    uVar5 = thunk_FUN_140034c40(local_190);
    cVar1 = thunk_FUN_140034800(uVar5,40000);
    thunk_FUN_140033660(local_190);
    if (cVar1 != '\0') {
      local_158 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      local_198 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_198._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
           *(int *)(local_158 + 4) + -0x88;
      local_170 = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      pQVar6 = (QString *)QIODevice::errorString((QIODevice *)param_2);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_158,"server responded with error: ");
      pbVar8 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,NVar3);
      uVar5 = thunk_FUN_140010400(pbVar8,&DAT_1400df4a0);
      pQVar9 = (QByteArray *)QString::toLocal8Bit(pQVar6);
      pcVar10 = QByteArray::data(pQVar9);
      thunk_FUN_140010400(uVar5,pcVar10);
      QByteArray::~QByteArray(local_168);
      QString::~QString((QString *)&local_198);
      uVar5 = thunk_FUN_140016d10(&local_158,local_60);
      uVar11 = thunk_FUN_140034c40(local_190);
      local_1a8 = 0xea;
      thunk_FUN_140033e30(uVar11,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033660(local_190);
      if (0xf < local_48) {
        uVar12 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar12 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar12 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar12) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar12);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_160 + (longlong)*(int *)(local_158 + 4) + 4) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
  }
  thunk_FUN_140083480(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140083b60
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140083b60(longlong param_1)

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
    uVar3 = thunk_FUN_140083150();
    local_188[0] = QString::fromAscii_helper("subAgent",8);
    cVar1 = thunk_FUN_1400ba1a0(uVar3,local_188);
    QString::~QString((QString *)local_188);
    if (cVar1 != '\0') {
      uVar3 = thunk_FUN_140034c40(local_1a8);
      cVar1 = thunk_FUN_140034800(uVar3,10000);
      thunk_FUN_140033660(local_1a8);
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
        thunk_FUN_140010c30(local_160,2);
        thunk_FUN_140010400(&local_168,"web token is empty await some");
        uVar3 = thunk_FUN_140016d10(&local_168,local_70);
        uVar4 = thunk_FUN_140034c40(local_1a8);
        local_1b8 = 0x9e;
        thunk_FUN_140033e30(uVar4,10000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033660(local_1a8);
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
          FUN_1400025c7(uVar7);
        }
        thunk_FUN_1400111a0(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    thunk_FUN_140080480(param_1 + 0x98,local_50);
    QJsonObject::~QJsonObject(local_40);
    QString::~QString(local_48);
    QString::~QString(local_50);
    thunk_FUN_140083480(param_1);
    thunk_FUN_140085890(param_1,0);
  }
  else if (*(int *)(*(longlong *)(param_1 + 0x70) + 4) == 0) {
    uVar3 = thunk_FUN_140083150();
    cVar1 = thunk_FUN_140085520(uVar3);
    if (cVar1 != '\0') {
      uVar3 = thunk_FUN_140034c40(local_1a8);
      cVar1 = thunk_FUN_140034800(uVar3,10000);
      thunk_FUN_140033660(local_1a8);
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
        thunk_FUN_140010c30(local_160,2);
        thunk_FUN_140010400(&local_168,"web interface link is empty await some");
        uVar3 = thunk_FUN_140016d10(&local_168,local_70);
        uVar4 = thunk_FUN_140034c40(local_1a8);
        local_1b8 = 0xa6;
        thunk_FUN_140033e30(uVar4,10000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033660(local_1a8);
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
          FUN_1400025c7(uVar7);
        }
        thunk_FUN_1400111a0(local_e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_e0);
      }
    }
    thunk_FUN_140080480(param_1 + 0x98,local_50);
    QJsonObject::~QJsonObject(local_40);
    QString::~QString(local_48);
    QString::~QString(local_50);
    thunk_FUN_1400837e0(param_1);
    thunk_FUN_140085890(param_1,0);
  }
  else {
    if (*(int *)(param_1 + 0xc0) == 1) {
      bVar2 = QListData::isEmpty((QListData *)(param_1 + 0x98));
      if (!bVar2) {
        uVar3 = thunk_FUN_140080480(param_1 + 0x98,local_50);
        thunk_FUN_14007ecd0(param_1 + 0xa0,uVar3);
        QJsonObject::~QJsonObject(local_40);
        QString::~QString(local_48);
        QString::~QString(local_50);
        bVar2 = QString::operator==((QString *)(param_1 + 0xa0),"get");
        if (bVar2) {
          uVar3 = thunk_FUN_140011870(local_1a8,param_1 + 0x70,param_1 + 0xa8);
          uVar3 = thunk_FUN_1400118e0(local_70,uVar3,"?web_token=");
          thunk_FUN_140011870(local_178,uVar3);
          QString::~QString((QString *)local_70);
          QString::~QString(local_1a8);
          uVar3 = thunk_FUN_140083150();
          cVar1 = thunk_FUN_140085520(uVar3);
          if (cVar1 != '\0') {
            uVar3 = thunk_FUN_140034c40(local_188);
            cVar1 = thunk_FUN_140034800(uVar3,10000);
            thunk_FUN_140033660(local_188);
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
              thunk_FUN_140010c30(local_160,2);
              uVar3 = thunk_FUN_140010400(&local_168,"subAgent send http GET: ");
              pQVar5 = (QByteArray *)QString::toLocal8Bit(local_178);
              pcVar6 = QByteArray::data(pQVar5);
              thunk_FUN_140010400(uVar3,pcVar6);
              QByteArray::~QByteArray((QByteArray *)local_188);
              uVar3 = thunk_FUN_140016d10(&local_168,local_50);
              uVar4 = thunk_FUN_140034c40(local_188);
              local_1b8 = 0xbb;
              thunk_FUN_140033e30(uVar4,10000,uVar3,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                                 );
              thunk_FUN_140033660(local_188);
              thunk_FUN_140011240(local_50);
              thunk_FUN_1400111a0(local_e0);
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
          thunk_FUN_140011870(local_188,param_1 + 0x70);
          uVar3 = thunk_FUN_140083150();
          cVar1 = thunk_FUN_140085520(uVar3);
          if (cVar1 != '\0') {
            uVar3 = thunk_FUN_140034c40(local_1a8);
            cVar1 = thunk_FUN_140034800(uVar3,10000);
            thunk_FUN_140033660(local_1a8);
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
              thunk_FUN_140010c30(local_160,2);
              QVariant::QVariant((QVariant *)local_70,(QJsonObject *)(param_1 + 0xb0));
              uVar3 = thunk_FUN_140010400(&local_168,"subAgent send http POST: ");
              pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)local_188);
              pcVar6 = QByteArray::data(pQVar5);
              thunk_FUN_140010400(uVar3,pcVar6);
              QByteArray::~QByteArray((QByteArray *)local_1a8);
              uVar3 = thunk_FUN_140010400(uVar3,", payload: ");
              thunk_FUN_14007f3a0(uVar3,local_70);
              QVariant::~QVariant((QVariant *)local_70);
              uVar3 = thunk_FUN_140016d10(&local_168,local_50);
              uVar4 = thunk_FUN_140034c40(local_1a8);
              local_1b8 = 0xc4;
              thunk_FUN_140033e30(uVar4,10000,uVar3,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                                 );
              thunk_FUN_140033660(local_1a8);
              thunk_FUN_140011240(local_50);
              thunk_FUN_1400111a0(local_e0);
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
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                  ,0xca);
        return;
      }
      uVar3 = thunk_FUN_140083150();
      cVar1 = thunk_FUN_140085520(uVar3);
      if (cVar1 == '\0') {
        return;
      }
      uVar3 = thunk_FUN_140034c40(local_1a8);
      cVar1 = thunk_FUN_140034800(uVar3,10000);
      thunk_FUN_140033660(local_1a8);
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
      thunk_FUN_140010c30(local_160,2);
      thunk_FUN_140010400(&local_168,"nothing to send");
      uVar3 = thunk_FUN_140016d10(&local_168,local_70);
      uVar4 = thunk_FUN_140034c40(local_1a8);
      local_1b8 = 0xb3;
      thunk_FUN_140033e30(uVar4,10000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033660(local_1a8);
      thunk_FUN_140011240(local_70);
    }
    else {
      uVar3 = thunk_FUN_140083150();
      cVar1 = thunk_FUN_140085520(uVar3);
      if (cVar1 == '\0') {
        return;
      }
      uVar3 = thunk_FUN_140034c40(local_1a8);
      cVar1 = thunk_FUN_140034800(uVar3,10000);
      thunk_FUN_140033660(local_1a8);
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
      thunk_FUN_140010c30(local_160,2);
      uVar3 = thunk_FUN_140010400(&local_168,"request in process method: ");
      pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0xa0));
      pcVar6 = QByteArray::data(pQVar5);
      thunk_FUN_140010400(uVar3,pcVar6);
      QByteArray::~QByteArray((QByteArray *)&local_190);
      uVar3 = thunk_FUN_140010400(uVar3," postfix: ");
      pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0xa8));
      pcVar6 = QByteArray::data(pQVar5);
      thunk_FUN_140010400(uVar3,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_188);
      uVar3 = thunk_FUN_140016d10(&local_168,local_70);
      uVar4 = thunk_FUN_140034c40(local_1a8);
      local_1b8 = 0xae;
      thunk_FUN_140033e30(uVar4,10000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033660(local_1a8);
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
        FUN_1400025c7(uVar7);
      }
    }
    thunk_FUN_1400111a0(local_e0);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_e0)
    ;
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140084c10
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140084c10(longlong param_1,QString *param_2)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  QByteArray *this;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_188 [32];
  undefined4 local_168;
  QTypedArrayData<unsigned_short> *local_158 [2];
  undefined8 local_148;
  int iStack_13c;
  undefined *local_138;
  basic_streambuf<char,std::char_traits<char>_> local_130 [128];
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_148 = 0xfffffffffffffffe;
  local_28 = DAT_140763590 ^ (ulonglong)auStack_188;
  local_158[0] = (QTypedArrayData<unsigned_short> *)((ulonglong)local_158[0] & 0xffffffff00000000);
  bVar1 = operator==((QString *)(param_1 + 0x68),param_2);
  if (!bVar1) {
    uVar3 = thunk_FUN_140083150();
    local_158[0] = QString::fromAscii_helper("subAgent",8);
    cVar2 = thunk_FUN_1400ba1a0(uVar3,local_158);
    QString::~QString((QString *)local_158);
    if (cVar2 != '\0') {
      uVar3 = thunk_FUN_140034c40(local_158);
      cVar2 = thunk_FUN_140034800(uVar3,20000);
      thunk_FUN_140033660(local_158);
      if (cVar2 != '\0') {
        local_138 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_158[0] = (QTypedArrayData<unsigned_short> *)CONCAT44(local_158[0]._4_4_,1);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
        *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_140010c30(local_130,2);
        uVar3 = thunk_FUN_140010400(&local_138,"web token acquired: ");
        this = (QByteArray *)QString::toLocal8Bit(param_2);
        pcVar4 = QByteArray::data(this);
        thunk_FUN_140010400(uVar3,pcVar4);
        QByteArray::~QByteArray((QByteArray *)local_158);
        uVar3 = thunk_FUN_140016d10(&local_138,local_48);
        uVar5 = thunk_FUN_140034c40(local_158);
        local_168 = 0x62;
        thunk_FUN_140033e30(uVar5,20000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033660(local_158);
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
          FUN_1400025c7(uVar6);
        }
        thunk_FUN_1400111a0(local_b0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_b0);
      }
    }
    QString::operator=((QString *)(param_1 + 0x68),param_2);
    thunk_FUN_140080c50(param_1);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140084f10
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140084f10(longlong param_1,QString *param_2,QString *param_3)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  QByteArray *this;
  ulonglong uVar5;
  char *pcVar6;
  uint uVar7;
  PeerVerifyMode PVar8;
  undefined1 auStack_1a8 [32];
  undefined4 local_188;
  QTypedArrayData<unsigned_short> *local_178;
  uint local_170;
  undefined ***local_168;
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
  local_40 = DAT_140763590 ^ (ulonglong)auStack_1a8;
  uVar7 = 0;
  local_170 = 0;
  bVar1 = operator==((QString *)(param_1 + 0x78),param_3);
  if (!bVar1) {
    QString::operator=((QString *)(param_1 + 0x78),param_3);
    PVar8 = 1;
    if (*(int *)(*(longlong *)(param_1 + 0x78) + 4) == 0) {
      PVar8 = 3;
    }
    uVar3 = thunk_FUN_140034c40(&local_168);
    cVar2 = thunk_FUN_140034800(uVar3,10000);
    thunk_FUN_140033660(&local_168);
    if (cVar2 != '\0') {
      local_158 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      uVar7 = 1;
      local_170 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_168 = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      pcVar6 = "query";
      if (*(int *)(*(longlong *)(param_1 + 0x78) + 4) == 0) {
        pcVar6 = "auto verify";
      }
      uVar3 = thunk_FUN_140010400(&local_158,"New pinning hash was received. Using ");
      uVar3 = thunk_FUN_140010400(uVar3,pcVar6);
      thunk_FUN_140010400(uVar3," peer mode.");
      uVar3 = thunk_FUN_140016d10(&local_158,local_60);
      uVar4 = thunk_FUN_140034c40(&local_168);
      local_188 = 0x50;
      thunk_FUN_140033e30(uVar4,10000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033660(&local_168);
      if (0xf < local_48) {
        uVar5 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar5);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
    QSslConfiguration::defaultConfiguration();
    QSslConfiguration::setPeerVerifyMode((QSslConfiguration *)&local_178,PVar8);
    QSslConfiguration::setDefaultConfiguration((QSslConfiguration *)&local_178);
    QSslConfiguration::~QSslConfiguration((QSslConfiguration *)&local_178);
  }
  bVar1 = operator==((QString *)(param_1 + 0x70),param_2);
  if (!bVar1) {
    uVar3 = thunk_FUN_140083150();
    local_178 = QString::fromAscii_helper("subAgent",8);
    cVar2 = thunk_FUN_1400ba1a0(uVar3,&local_178);
    QString::~QString((QString *)&local_178);
    if (cVar2 != '\0') {
      uVar3 = thunk_FUN_140034c40(&local_168);
      cVar2 = thunk_FUN_140034800(uVar3,20000);
      thunk_FUN_140033660(&local_168);
      if (cVar2 != '\0') {
        local_158 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        local_170 = uVar7 | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        thunk_FUN_140010c30(&local_150,2);
        uVar3 = thunk_FUN_140010400(&local_158,"web interface link acquired: ");
        this = (QByteArray *)QString::toLocal8Bit(param_2);
        pcVar6 = QByteArray::data(this);
        thunk_FUN_140010400(uVar3,pcVar6);
        QByteArray::~QByteArray((QByteArray *)&local_178);
        uVar3 = thunk_FUN_140016d10(&local_158,local_60);
        uVar4 = thunk_FUN_140034c40(&local_168);
        local_188 = 0x58;
        thunk_FUN_140033e30(uVar4,20000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033660(&local_168);
        if (0xf < local_48) {
          uVar5 = local_60[0];
          if (0xfff < local_48 + 1) {
            if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar5 = *(ulonglong *)(local_60[0] - 8);
            if (local_60[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_60[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_60[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar5);
        }
        thunk_FUN_1400111a0(local_d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
      }
    }
    QString::operator=((QString *)(param_1 + 0x70),param_2);
    thunk_FUN_140080c50(param_1);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140085890
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140085890(longlong param_1,bool param_2)

{
  char cVar1;
  QDateTime *pQVar2;
  undefined8 uVar3;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar4;
  ulonglong uVar5;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined4 local_168 [2];
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
  local_38 = DAT_140763590 ^ (ulonglong)auStack_198;
  local_168[0] = 0;
  if (param_2 != false) {
    pQVar2 = (QDateTime *)QDateTime::currentDateTimeUtc();
    QDateTime::operator=((QDateTime *)(param_1 + 0x88),pQVar2);
    QDateTime::~QDateTime((QDateTime *)local_168);
  }
  if (param_2 != (bool)*(char *)(param_1 + 0x80)) {
    uVar3 = thunk_FUN_140034c40(local_160);
    cVar1 = thunk_FUN_140034800(uVar3,20000);
    thunk_FUN_140033660(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168[0] = 1;
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
             thunk_FUN_140010400(&local_148,"http connected: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
      uVar3 = thunk_FUN_140016d10(&local_148,local_58);
      uVar4 = thunk_FUN_140034c40(local_160);
      local_178 = 0x12a;
      thunk_FUN_140033e30(uVar4,20000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                         );
      thunk_FUN_140033660(local_160);
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
        FUN_1400025c7(uVar5);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    *(bool *)(param_1 + 0x80) = param_2;
    thunk_FUN_140086530(param_1,param_2);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140085bb0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140085bb0(QTypedArrayData<unsigned_short> *param_1,QString *param_2)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  undefined8 uVar4;
  QByteArray *this;
  char *pcVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  undefined1 auStack_1a8 [32];
  undefined4 local_188;
  QTypedArrayData<unsigned_short> *local_178;
  undefined1 *local_170;
  QTypedArrayData<unsigned_short> **local_168;
  undefined1 local_160 [16];
  ulonglong *local_150;
  undefined8 local_148;
  int iStack_13c;
  undefined *local_138;
  basic_streambuf<char,std::char_traits<char>_> local_130 [128];
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_148 = 0xfffffffffffffffe;
  local_28 = DAT_140763590 ^ (ulonglong)auStack_1a8;
  local_178 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_178 & 0xffffffff00000000);
  bVar1 = operator==((QString *)(param_1 + 0x90),param_2);
  if (!bVar1) {
    QString::operator=((QString *)(param_1 + 0x90),param_2);
    uVar4 = thunk_FUN_140083150();
    local_178 = QString::fromAscii_helper("subAgent",8);
    cVar2 = thunk_FUN_1400ba1a0(uVar4,&local_178);
    QString::~QString((QString *)&local_178);
    if (cVar2 != '\0') {
      uVar4 = thunk_FUN_140034c40(&local_170);
      cVar2 = thunk_FUN_140034800(uVar4,20000);
      thunk_FUN_140033660(&local_170);
      if (cVar2 != '\0') {
        local_138 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_178 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_178._4_4_,1);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
        *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_140010c30(local_130,2);
        uVar4 = thunk_FUN_140010400(&local_138,"skp version updated to: ");
        this = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x90));
        pcVar5 = QByteArray::data(this);
        thunk_FUN_140010400(uVar4,pcVar5);
        QByteArray::~QByteArray((QByteArray *)&local_178);
        uVar4 = thunk_FUN_140016d10(&local_138,local_48);
        uVar6 = thunk_FUN_140034c40(&local_170);
        local_188 = 0x13c;
        thunk_FUN_140033e30(uVar6,20000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-http\\manager.cpp"
                           );
        thunk_FUN_140033660(&local_170);
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
          FUN_1400025c7(uVar7);
        }
        thunk_FUN_1400111a0(local_b0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_b0);
      }
    }
    thunk_FUN_140080a50(param_1);
    local_178 = param_1;
    __ExceptionPtrCreate(local_160);
    local_170 = local_160;
    local_168 = &local_178;
    iVar3 = std::_Execute_once((once_flag *)(param_1 + 200),
                               (_func_int_void_ptr_void_ptr_void_ptr_ptr *)&LAB_140003e9f,&local_170
                              );
    if (iVar3 == 0) {
      bVar1 = __ExceptionPtrToBool(local_160);
      if (bVar1) {
        __ExceptionPtrCopy(local_48,local_160);
        local_150 = local_48;
        __ExceptionPtrRethrow(local_48);
      }
      std::_XGetLastError();
    }
    else {
      __ExceptionPtrDestroy(local_160);
    }
  }
  return;
}

// =============================================================================
// UI SI SETTINGS
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140087bb0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QObject * FUN_140087bb0(QObject *param_1)

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
  thunk_FUN_1400948c0(param_1 + 0x28);
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
  iVar4 = thunk_FUN_140094cd0(param_1 + 0x28,&local_180);
  QString::~QString((QString *)&local_180);
  param_1[0x20] = (QObject)(iVar4 == 0);
  if (param_1[0x20] == (QObject)0x0) {
    uVar6 = thunk_FUN_140034c40(&local_198);
    cVar3 = thunk_FUN_140034800(uVar6,40000);
    thunk_FUN_140033660(&local_198);
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
    thunk_FUN_140010400(&local_158,"agent ui cannot be initialized");
    uVar6 = thunk_FUN_140016d10(&local_158,local_58);
    uVar7 = thunk_FUN_140034c40(&local_198);
    local_1a8 = (QIcon **)CONCAT44(local_1a8._4_4_,99);
    thunk_FUN_140033e30(uVar7,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\manager.cpp"
                       );
    thunk_FUN_140033660(&local_198);
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
      FUN_1400025c7(uVar10);
    }
    *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_150);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_148);
  }
  else {
    uVar6 = thunk_FUN_140034c40(&local_198);
    cVar3 = thunk_FUN_140034800(uVar6,20000);
    thunk_FUN_140033660(&local_198);
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
      thunk_FUN_140010400(&local_158,"agent ui initialized");
      uVar6 = thunk_FUN_140016d10(&local_158,local_58);
      uVar7 = thunk_FUN_140034c40(&local_198);
      local_1a8 = (QIcon **)CONCAT44(local_1a8._4_4_,0x47);
      thunk_FUN_140033e30(uVar7,20000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\manager.cpp"
                         );
      thunk_FUN_140033660(&local_198);
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
        FUN_1400025c7(uVar10);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_150);
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
      plVar8 = (longlong *)thunk_FUN_14008a1f0(local_178);
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
    cVar3 = thunk_FUN_14008b610(*(undefined8 *)(param_1 + 0xf0));
    if (cVar3 != '\0') {
      plVar8 = (longlong *)(*(longlong *)(param_1 + 0xf0) + 0x10);
      if (*(longlong *)(param_1 + 0xf0) == 0) {
        plVar8 = (longlong *)0x0;
      }
      thunk_FUN_1400ab5a0(plVar8);
      if (*(longlong *)(param_1 + 0xf0) == 0) {
        qt_assert("d",
                  "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qscopedpointer.h",
                  0x70);
      }
      thunk_FUN_14008b620(*(undefined8 *)(param_1 + 0xf0),&local_180);
      if (*(int *)(local_180 + 4) == 0) {
        this = (QLocale *)QLocale::system();
        LVar5 = QLocale::language(this);
        pQVar9 = (QString *)thunk_FUN_140093270(param_1 + 0x68,&local_188,LVar5);
        QString::operator=((QString *)&local_180,pQVar9);
        QString::~QString((QString *)&local_188);
        QLocale::~QLocale((QLocale *)&local_178);
      }
      thunk_FUN_1400935b0(param_1 + 0x68,&local_180);
      local_188 = QString::fromAscii_helper(":/images/selfcontrol_icon.ico",0x1d);
      this_00 = (QIcon *)QIcon::QIcon((QIcon *)&local_178,(QString *)&local_188);
      pQVar2 = local_168;
      local_198 = this_00;
      QIcon::operator=((QIcon *)(local_168 + 0xe8),this_00);
      QApplication::setWindowIcon((QIcon *)(pQVar2 + 0xe8));
      QIcon::~QIcon(this_00);
      QString::~QString((QString *)&local_188);
      thunk_FUN_140088ef0(param_1);
      QSystemTrayIcon::setIcon((QSystemTrayIcon *)(param_1 + 0xa0),(QIcon *)(param_1 + 0xe8));
      QSystemTrayIcon::setContextMenu((QSystemTrayIcon *)(param_1 + 0xa0),(QMenu *)(param_1 + 0xb0))
      ;
      local_198 = (QIcon *)thunk_FUN_140089770;
      uStack_190 = 0;
      local_1a0 = 0;
      local_1a8 = &local_198;
      thunk_FUN_140087860(&local_168,param_1 + 0x68,thunk_FUN_140094350,param_1);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      QString::~QString((QString *)&local_180);
      return param_1;
    }
    param_1[0x20] = (QObject)0x0;
    uVar6 = thunk_FUN_140034c40(&local_198);
    cVar3 = thunk_FUN_140034800(uVar6,40000);
    thunk_FUN_140033660(&local_198);
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
    thunk_FUN_140010c30(&local_150,2);
    thunk_FUN_140010400(&local_158,"unable to open settings database");
    uVar6 = thunk_FUN_140016d10(&local_158,local_58);
    uVar7 = thunk_FUN_140034c40(&local_198);
    local_1a8 = (QIcon **)CONCAT44(local_1a8._4_4_,0x4d);
    thunk_FUN_140033e30(uVar7,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\manager.cpp"
                       );
    thunk_FUN_140033660(&local_198);
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
      FUN_1400025c7(uVar10);
    }
    thunk_FUN_1400111a0(local_d0);
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_d0);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14008a1f0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QObject * FUN_14008a1f0(QObject *param_1)

{
  char cVar1;
  bool bVar2;
  Status SVar3;
  QTypedArrayData<unsigned_short> *pQVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  QByteArray *this;
  char *pcVar7;
  QVariant *pQVar8;
  ulonglong uVar9;
  undefined1 auStack_1e8 [32];
  undefined4 local_1c8;
  QTypedArrayData<unsigned_short> *local_1b8;
  undefined4 local_1b0 [2];
  QString local_1a8 [8];
  longlong local_1a0;
  QDir local_198 [8];
  QSettings local_190 [16];
  QObject *local_180;
  QString local_178 [8];
  undefined ***local_170;
  QFile local_168 [12];
  int iStack_15c;
  undefined *local_158;
  undefined **local_150;
  basic_ostream<char,std::char_traits<char>_> local_148 [96];
  undefined8 local_e8;
  undefined4 local_e0;
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  QString local_68 [8];
  undefined8 local_60;
  QVariant local_50 [32];
  ulonglong local_30 [3];
  ulonglong local_18;
  ulonglong local_10;
  
  local_60 = 0xfffffffffffffffe;
  local_10 = DAT_140763590 ^ (ulonglong)auStack_1e8;
  local_1b0[0] = 0;
  local_180 = param_1;
  QObject::QObject(param_1,(QObject *)0x0);
  thunk_FUN_1400ac140(param_1 + 0x10);
  *(undefined ***)param_1 = grabberSubAgent::ui::SettingsProvider::vftable;
  *(undefined ***)(param_1 + 0x10) = grabberSubAgent::ui::SettingsProvider::vftable;
  pQVar4 = QString::fromAscii_helper("Time tracking",0xd);
  *(QTypedArrayData<unsigned_short> **)(param_1 + 0x78) = pQVar4;
  pQVar4 = QString::fromAscii_helper("settings.sqlite",0xf);
  *(QTypedArrayData<unsigned_short> **)(param_1 + 0x80) = pQVar4;
  QDir::homePath();
  if (*(int *)(local_1a0 + 4) == 0) {
    uVar5 = thunk_FUN_140034c40(local_50);
    cVar1 = thunk_FUN_140034800(uVar5,40000);
    thunk_FUN_140033660(local_50);
    if (cVar1 != '\0') {
      local_158 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      local_1b0[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_170 = &local_150;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_e8 = 0;
      local_e0 = 4;
      thunk_FUN_140010400(&local_158,"unable to locate home path");
      uVar5 = thunk_FUN_140016d10(&local_158,local_30);
      uVar6 = thunk_FUN_140034c40(local_50);
      local_1c8 = 0x22;
      thunk_FUN_140033e30(uVar6,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\settingsProvider.cpp"
                         );
      thunk_FUN_140033660(local_50);
      if (0xf < local_18) {
        uVar9 = local_30[0];
        if (0xfff < local_18 + 1) {
          if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar9 = *(ulonglong *)(local_30[0] - 8);
          if (local_30[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_30[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_30[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar9);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
  }
  else {
    uVar5 = thunk_FUN_1400118e0(&local_1b8,&local_1a0,&DAT_1400dbf38);
    thunk_FUN_140011870(local_1a8,uVar5);
    QString::~QString((QString *)&local_1b8);
    QDir::QDir(local_198,local_1a8);
    bVar2 = QDir::exists(local_198);
    if ((bVar2) || (bVar2 = QDir::mkpath(local_198,local_1a8), bVar2)) {
      uVar5 = thunk_FUN_1400118e0(local_68,local_1a8,&DAT_1400dbf38);
      thunk_FUN_140011870(local_178,uVar5,param_1 + 0x80);
      QString::~QString(local_68);
      thunk_FUN_1400adee0(param_1 + 0x10,local_178);
      bVar2 = QSqlDatabase::isOpen((QSqlDatabase *)(param_1 + 0x18));
      if (bVar2) {
        QFile::QFile(local_168);
        uVar5 = thunk_FUN_1400118e0(&local_1b8,local_1a8,&DAT_1400dbf38);
        thunk_FUN_1400118e0(local_1b0,uVar5,"settings.ini");
        QString::~QString((QString *)&local_1b8);
        QFile::setFileName(local_168,(QString *)local_1b0);
        bVar2 = QFile::exists(local_168);
        if (bVar2) {
          QSettings::QSettings(local_190,(QString *)local_1b0,1,(QObject *)0x0);
          SVar3 = QSettings::status(local_190);
          if (SVar3 == 0) {
            QVariant::QVariant(local_50,"");
            local_1b8 = QString::fromAscii_helper("common/language",0xf);
            pQVar8 = (QVariant *)
                     QSettings::value(local_190,(QString *)local_30,(QVariant *)&local_1b8);
            QVariant::toString(pQVar8);
            thunk_FUN_14008bc40(param_1);
            QString::~QString((QString *)&local_170);
            QVariant::~QVariant((QVariant *)local_30);
            QString::~QString((QString *)&local_1b8);
            QVariant::~QVariant(local_50);
            QVariant::QVariant(local_50,false);
            local_1b8 = QString::fromAscii_helper("self_control/dayActivityEnabled",0x1f);
            pQVar8 = (QVariant *)
                     QSettings::value(local_190,(QString *)local_30,(QVariant *)&local_1b8);
            QVariant::toBool(pQVar8);
            thunk_FUN_14008b980(param_1);
            QVariant::~QVariant((QVariant *)local_30);
            QString::~QString((QString *)&local_1b8);
            QVariant::~QVariant(local_50);
            QVariant::QVariant(local_50,false);
            local_1b8 = QString::fromAscii_helper("self_control/dayActivityInPercentage",0x24);
            pQVar8 = (QVariant *)
                     QSettings::value(local_190,(QString *)local_30,(QVariant *)&local_1b8);
            QVariant::toBool(pQVar8);
            thunk_FUN_14008ba30(param_1);
            QVariant::~QVariant((QVariant *)local_30);
            QString::~QString((QString *)&local_1b8);
            QVariant::~QVariant(local_50);
            QVariant::QVariant(local_50,false);
            local_1b8 = QString::fromAscii_helper("self_control/dayRatingEnabled",0x1d);
            pQVar8 = (QVariant *)
                     QSettings::value(local_190,(QString *)local_30,(QVariant *)&local_1b8);
            QVariant::toBool(pQVar8);
            thunk_FUN_14008bae0(param_1);
            QVariant::~QVariant((QVariant *)local_30);
            QString::~QString((QString *)&local_1b8);
            QVariant::~QVariant(local_50);
            QVariant::QVariant(local_50,false);
            local_1b8 = QString::fromAscii_helper("self_control/dataManagementEnabled",0x22);
            pQVar8 = (QVariant *)
                     QSettings::value(local_190,(QString *)local_30,(QVariant *)&local_1b8);
            bVar2 = QVariant::toBool(pQVar8);
            thunk_FUN_14008b8d0(param_1,bVar2);
            QVariant::~QVariant((QVariant *)local_30);
            QString::~QString((QString *)&local_1b8);
            QVariant::~QVariant(local_50);
            QVariant::QVariant(local_50,true);
            local_1b8 = QString::fromAscii_helper("self_control/dataFetchingEnabled",0x20);
            pQVar8 = (QVariant *)
                     QSettings::value(local_190,(QString *)local_30,(QVariant *)&local_1b8);
            bVar2 = QVariant::toBool(pQVar8);
            thunk_FUN_14008b820(param_1,bVar2);
            QVariant::~QVariant((QVariant *)local_30);
            QString::~QString((QString *)&local_1b8);
            QVariant::~QVariant(local_50);
            QFile::remove(local_168);
          }
          QSettings::~QSettings(local_190);
        }
        QString::~QString((QString *)local_1b0);
        QFile::~QFile(local_168);
      }
      QString::~QString(local_178);
    }
    else {
      uVar5 = thunk_FUN_140034c40(local_50);
      cVar1 = thunk_FUN_140034800(uVar5,40000);
      thunk_FUN_140033660(local_50);
      if (cVar1 != '\0') {
        local_158 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        local_1b0[0] = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        thunk_FUN_140010c30(&local_150,2);
        uVar5 = thunk_FUN_140010400(&local_158,"unable to create settings directory: ");
        this = (QByteArray *)QString::toLocal8Bit(local_1a8);
        pcVar7 = QByteArray::data(this);
        thunk_FUN_140010400(uVar5,pcVar7);
        QByteArray::~QByteArray((QByteArray *)&local_1b8);
        uVar5 = thunk_FUN_140016d10(&local_158,local_30);
        uVar6 = thunk_FUN_140034c40(local_50);
        local_1c8 = 0x2c;
        thunk_FUN_140033e30(uVar6,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui\\settingsProvider.cpp"
                           );
        thunk_FUN_140033660(local_50);
        if (0xf < local_18) {
          uVar9 = local_30[0];
          if (0xfff < local_18 + 1) {
            if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar9 = *(ulonglong *)(local_30[0] - 8);
            if (local_30[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_30[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_30[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar9);
        }
        thunk_FUN_1400111a0(local_d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
      }
    }
    QDir::~QDir(local_198);
    QString::~QString(local_1a8);
  }
  QString::~QString((QString *)&local_1a0);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140090fb0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QVariant * FUN_140090fb0(longlong param_1,QVariant *param_2,QModelIndex *param_3,int param_4)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar5;
  longlong lVar6;
  ulonglong uVar7;
  QString *pQVar8;
  QListData *this_00;
  undefined1 auStack_1a8 [32];
  undefined4 local_188;
  QVariant local_178 [16];
  undefined4 local_168;
  QVariant local_160 [16];
  undefined8 local_150;
  undefined *local_148;
  basic_streambuf<char,std::char_traits<char>_> local_140 [128];
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_150 = 0xfffffffffffffffe;
  local_38 = DAT_140763590 ^ (ulonglong)auStack_1a8;
  local_168 = 0;
  QVariant::QVariant(local_160);
  iVar2 = QModelIndex::row(param_3);
  this_00 = (QListData *)(param_1 + 0x38);
  iVar3 = QListData::size(this_00);
  if ((iVar3 + -1 < iVar2) || (iVar2 < 0)) goto LAB_140091215;
  if (param_4 == 0x101) {
    lVar6 = thunk_FUN_140090cf0(this_00,iVar2);
    pQVar8 = (QString *)(lVar6 + 8);
LAB_1400911ef:
    QVariant::QVariant(local_178,pQVar8);
  }
  else {
    if (param_4 == 0x102) {
      lVar6 = thunk_FUN_140090cf0(this_00,iVar2);
      pQVar8 = (QString *)(lVar6 + 0x10);
      goto LAB_1400911ef;
    }
    if (param_4 != 0x103) {
      uVar4 = thunk_FUN_140034c40(local_178);
      cVar1 = thunk_FUN_140034800(uVar4,30000);
      thunk_FUN_140033660(local_178);
      if (cVar1 != '\0') {
        local_148 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_168 = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
        *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_140010c30(local_140,2);
        this = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_140010400(&local_148,
                                   "notification model: data requested for unsupported role ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_4);
        uVar4 = thunk_FUN_140016d10(&local_148,local_58);
        uVar5 = thunk_FUN_140034c40(local_178);
        local_188 = 0x46;
        thunk_FUN_140033e30(uVar5,30000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-notifications\\model.cpp"
                           );
        thunk_FUN_140033660(local_178);
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
          FUN_1400025c7(uVar7);
        }
        thunk_FUN_1400111a0(local_c0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
      goto LAB_140091215;
    }
    lVar6 = thunk_FUN_140090cf0(this_00,iVar2);
    QVariant::QVariant(local_178,(QDateTime *)(lVar6 + 0x18));
  }
  QVariant::operator=(local_160,local_178);
  QVariant::~QVariant(local_178);
LAB_140091215:
  QVariant::QVariant(param_2,local_160);
  QVariant::~QVariant(local_160);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140091e50
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_140091e50(QAbstractItemModel *param_1,int param_2,undefined8 param_3,QModelIndex *param_4)

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
    uVar2 = thunk_FUN_140034c40(local_150);
    cVar1 = thunk_FUN_140034800(uVar2,40000);
    thunk_FUN_140033660(local_150);
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
      thunk_FUN_140010400(&local_138,"notifications model: invalid row requested");
      uVar2 = thunk_FUN_140016d10(&local_138,local_48);
      uVar3 = thunk_FUN_140034c40(local_150);
      local_168 = 0x66;
      thunk_FUN_140033e30(uVar3,40000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-notifications\\model.cpp"
                         );
      thunk_FUN_140033660(local_150);
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
        FUN_1400025c7(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_130);
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
    thunk_FUN_140091c50(param_1 + 0x38,param_2,param_1 + 0x18);
    QAbstractItemModel::endInsertRows(param_1);
    thunk_FUN_1400900c0(param_1);
    uVar2 = 1;
  }
  return uVar2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140092470
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8
FUN_140092470(QAbstractItemModel *param_1,int param_2,undefined8 param_3,QModelIndex *param_4)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  longlong *plVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  undefined1 auStack_1b8 [32];
  undefined4 local_198;
  undefined4 local_188;
  undefined ***local_180 [2];
  QString local_170 [8];
  QString local_168 [8];
  QString local_160 [8];
  QDateTime local_158 [8];
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
  local_38 = DAT_140763590 ^ (ulonglong)auStack_1b8;
  uVar3 = 0;
  local_188 = 0;
  iVar2 = QListData::size((QListData *)(param_1 + 0x38));
  if ((iVar2 + -1 < param_2) || (param_2 < 0)) {
    uVar5 = thunk_FUN_140034c40(local_180);
    cVar1 = thunk_FUN_140034800(uVar5,40000);
    thunk_FUN_140033660(local_180);
    if (cVar1 != '\0') {
      local_148 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_188 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_180[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      thunk_FUN_140010400(&local_148,"notifications model: invalid row requested");
      uVar5 = thunk_FUN_140016d10(&local_148,local_58);
      uVar6 = thunk_FUN_140034c40(local_180);
      local_198 = 0x78;
      thunk_FUN_140033e30(uVar6,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-notifications\\model.cpp"
                         );
      thunk_FUN_140033660(local_180);
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
        FUN_1400025c7(uVar7);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    uVar5 = 0;
  }
  else {
    QAbstractItemModel::beginRemoveRows(param_1,param_4,param_2,param_2);
    thunk_FUN_140092af0(param_1 + 0x38,local_170,param_2);
    if (*(int *)(*(longlong *)(param_1 + 0x10) + 0x20) != 0) {
      uVar3 = qHash(local_170,*(uint *)(*(longlong *)(param_1 + 0x10) + 0x24));
    }
    plVar4 = (longlong *)thunk_FUN_1400918b0(param_1 + 0x10,local_170,uVar3);
    if (*plVar4 != *(longlong *)(param_1 + 0x10)) {
      thunk_FUN_140092310(param_1 + 0x10,local_170);
    }
    QAbstractItemModel::endRemoveRows(param_1);
    thunk_FUN_1400900c0(param_1);
    QDateTime::~QDateTime(local_158);
    QString::~QString(local_160);
    QString::~QString(local_168);
    QString::~QString(local_170);
    uVar5 = 1;
  }
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140094cd0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140094cd0(longlong param_1,longlong *param_2)

{
  ulonglong uVar1;
  char cVar2;
  bool bVar3;
  DWORD DVar4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *pbVar6;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  QString *pQVar10;
  QByteArray *pQVar11;
  QGLContext *pQVar12;
  QRect *pQVar13;
  QOpenGLFunctions *this;
  char *pcVar14;
  uchar *puVar15;
  ulonglong uVar16;
  longlong lVar17;
  undefined *puVar18;
  uint uVar19;
  undefined *puVar20;
  undefined1 auStackY_228 [32];
  QTypedArrayData<unsigned_short> *local_1f8;
  undefined ***local_1f0 [2];
  uint local_1e0 [2];
  QTypedArrayData<unsigned_short> *local_1d8;
  int iStack_1cc;
  undefined *local_1c8;
  undefined **local_1c0;
  basic_ostream<char,std::char_traits<char>_> local_1b8 [96];
  undefined8 local_158;
  undefined4 local_150;
  basic_ios<char,std::char_traits<char>_> local_140 [104];
  QTypedArrayData<unsigned_short> *local_d8;
  QFile local_d0 [16];
  QWindow local_c0 [16];
  QSurface local_b0 [24];
  undefined8 local_98;
  ulonglong local_90 [3];
  ulonglong local_78;
  uint local_68;
  undefined4 uStack_64;
  uchar *puStack_60;
  ulonglong local_50;
  ulonglong local_48;
  
  local_98 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStackY_228;
  uVar19 = 0;
  local_1e0[0] = 0;
  DVar4 = GetVersion();
  uVar5 = thunk_FUN_140034c40(local_1f0);
  cVar2 = thunk_FUN_140034800(uVar5,20000);
  thunk_FUN_140033660(local_1f0);
  uVar5 = 1;
  if (cVar2 != '\0') {
    local_1c8 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_140)
    ;
    local_1e0[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
    *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
         *(int *)(local_1c8 + 4) + -0x88;
    local_1f0[0] = &local_1c0;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_1c0);
    local_1c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_158 = 0;
    local_150 = 4;
    pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010400(&local_1c8,"OS version mj: ");
    pbVar7 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,DVar4 & 0xff);
    pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_140010400(pbVar7," mn: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,DVar4 >> 8 & 0xff);
    uVar8 = thunk_FUN_140016d10(&local_1c8,&local_68);
    uVar9 = thunk_FUN_140034c40(local_1f0);
    thunk_FUN_140033e30(uVar9,20000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                       );
    thunk_FUN_140033660(local_1f0);
    if (0xf < local_50) {
      uVar1 = CONCAT44(uStack_64,local_68);
      uVar16 = uVar1;
      if (0xfff < local_50 + 1) {
        if ((local_68 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar16 = *(ulonglong *)(uVar1 - 8);
        if (uVar1 <= uVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (uVar1 - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < uVar1 - uVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c7(uVar16);
    }
    *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
         *(int *)(local_1c8 + 4) + -0x88;
    local_1c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_1c0);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_1c0);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_1b8);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_140);
    uVar19 = 1;
  }
  if ((DVar4 & 0xff) < 6) {
    return 4;
  }
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Bold.ttf",0x19);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-BoldItalic.ttf",0x1f);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-CondBold.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-CondLight.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-CondLightItalic.ttf",0x24);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-ExtraBold.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-ExtraBoldItalic.ttf",0x24);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Italic.ttf",0x1b);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Light.ttf",0x1a);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-LightItalic.ttf",0x20);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Regular.ttf",0x1c);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Semibold.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-SemiboldItalic.ttf",0x23);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Black.ttf",0x18);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-BlackItalic.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Bold.ttf",0x17);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-BoldItalic.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Italic.ttf",0x19);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Light.ttf",0x18);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-LightItalic.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Medium.ttf",0x19);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-MediumItalic.ttf",0x1f);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Regular.ttf",0x1a);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Thin.ttf",0x17);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-ThinItalic.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Bold.ttf",0x20);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-BoldItalic.ttf",0x26);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Italic.ttf",0x22);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Light.ttf",0x21);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-LightItalic.ttf",0x27);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Regular.ttf",0x23);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper("Open Sans",9);
  QFont::QFont((QFont *)local_1f0,(QString *)&local_1f8,7,0x19,false);
  QString::~QString((QString *)&local_1f8);
  QApplication::setFont((QFont *)local_1f0,(char *)0x0);
  QFont::~QFont((QFont *)local_1f0);
  pQVar10 = (QString *)QCoreApplication::applicationDirPath();
  QDir::QDir((QDir *)&local_1d8,pQVar10);
  QString::~QString((QString *)&local_d8);
  QDir::cdUp((QDir *)&local_1d8);
  local_1f8 = QString::fromAscii_helper("fonts",5);
  QDir::cd((QDir *)&local_1d8,(QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  pQVar10 = (QString *)QDir::path((QDir *)&local_1d8);
  pQVar11 = (QByteArray *)QString::toLocal8Bit(pQVar10);
  qputenv("QT_QPA_FONTDIR",pQVar11);
  QByteArray::~QByteArray((QByteArray *)&local_1f8);
  QString::~QString((QString *)local_1e0);
  QDir::~QDir((QDir *)&local_1d8);
  bVar3 = QGLFormat::hasOpenGL();
  if (!bVar3) {
    uVar5 = thunk_FUN_140034c40(local_1f0);
    cVar2 = thunk_FUN_140034800(uVar5,50000);
    thunk_FUN_140033660(local_1f0);
    if (cVar2 != '\0') {
      local_1c8 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_140);
      local_1e0[0] = uVar19 | 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
      *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
           *(int *)(local_1c8 + 4) + -0x88;
      thunk_FUN_140010c30(&local_1c0,2);
      thunk_FUN_140010400(&local_1c8,"System does not support OpenGL");
      uVar5 = thunk_FUN_140016d10(&local_1c8,&local_68);
      uVar8 = thunk_FUN_140034c40(local_1f0);
      thunk_FUN_140033e30(uVar8,50000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                         );
      thunk_FUN_140033660(local_1f0);
      if (0xf < local_50) {
        uVar1 = CONCAT44(uStack_64,local_68);
        uVar16 = uVar1;
        if (0xfff < local_50 + 1) {
          if ((local_68 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar16 = *(ulonglong *)(uVar1 - 8);
          if (uVar1 <= uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (uVar1 - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < uVar1 - uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar16);
      }
      thunk_FUN_1400111a0(local_140);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_140);
    }
    return 1;
  }
  QString::QString((QString *)&local_1f8);
  pQVar12 = QGLContext::currentContext();
  QWindow::QWindow(local_c0,(QScreen *)0x0);
  QWindow::setSurfaceType(local_c0,1);
  uVar8 = 3;
  pQVar13 = (QRect *)QRect::QRect((QRect *)&local_68,0,0,3,3);
  QWindow::setGeometry(local_c0,pQVar13);
  QWindow::create(local_c0);
  QOpenGLContext::QOpenGLContext((QOpenGLContext *)local_1f0,(QObject *)0x0);
  bVar3 = QOpenGLContext::create((QOpenGLContext *)local_1f0);
  if (bVar3) {
    bVar3 = QOpenGLContext::makeCurrent((QOpenGLContext *)local_1f0,local_b0);
    if (bVar3) {
      this = QOpenGLContext::functions((QOpenGLContext *)local_1f0);
      if (this != (QOpenGLFunctions *)0x0) {
        puStack_60 = QOpenGLFunctions::glGetString(this,0x1f02);
        if (puStack_60 == (uchar *)0x0) {
          local_68 = 0;
        }
        else {
          lVar17 = -1;
          do {
            lVar17 = lVar17 + 1;
          } while (puStack_60[lVar17] != '\0');
          local_68 = (uint)lVar17;
        }
        QString::operator=((QString *)&local_1f8,&local_68);
        if (pQVar12 != (QGLContext *)0x0) {
          (**(code **)(*(longlong *)pQVar12 + 0x10))(pQVar12);
        }
        QOpenGLContext::~QOpenGLContext((QOpenGLContext *)local_1f0);
        QWindow::~QWindow(local_c0);
        uVar5 = thunk_FUN_140034c40(local_1f0);
        cVar2 = thunk_FUN_140034800(uVar5,20000);
        thunk_FUN_140033660(local_1f0);
        if (cVar2 != '\0') {
          local_1c8 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_140);
          uVar19 = uVar19 | 0x20;
          local_1e0[0] = uVar19;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
          *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
               *(int *)(local_1c8 + 4) + -0x88;
          thunk_FUN_140010c30(&local_1c0,2);
          uVar5 = thunk_FUN_140010400(&local_1c8,"glVersionString: ");
          pQVar11 = (QByteArray *)QString::toLocal8Bit((QString *)&local_1f8);
          pcVar14 = QByteArray::data(pQVar11);
          thunk_FUN_140010400(uVar5,pcVar14);
          QByteArray::~QByteArray((QByteArray *)&local_d8);
          uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
          uVar9 = thunk_FUN_140034c40(local_1f0);
          thunk_FUN_140033e30(uVar9,20000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                             );
          thunk_FUN_140033660(local_1f0);
          thunk_FUN_140011240(local_90);
          thunk_FUN_1400111a0(local_140);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_140);
        }
        QGLFormat::openGLVersionFlags();
        if ((((ulonglong)local_1d8 & 0x40) == 0) && (((ulonglong)local_1d8 & 0x800) == 0)) {
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          puVar20 = &DAT_1400f67a4;
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x40;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 1) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x80;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 2) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.2: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x100;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 4) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.3: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x200;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 8) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.4: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x400;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x10) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.5: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x800;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x20) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 2.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x1000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x40) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 2.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x2000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x1000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x4000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x2000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x8000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x4000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.2: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x10000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x8000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.3: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x20000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x10000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x40000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x20000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x80000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x40000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.2: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x100000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x80000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.3: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x200000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x80) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common 1.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x400000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x200) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common 1.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x800000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x100) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common Lite 1.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x1000000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x400) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common Lite 1.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x2000000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            if (((uint)local_1d8 & 0x800) != 0) {
              puVar20 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES 2.0: ");
            thunk_FUN_140010400(uVar5,puVar20);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,50000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            local_1e0[0] = uVar19 | 0x4000000;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            thunk_FUN_140010400(&local_1c8,"OpenGL version lower than 2.1");
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,50000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = 2;
          goto LAB_1400975ae;
        }
        QString::~QString((QString *)&local_1f8);
        local_1d8 = QString::fromAscii_helper(":/base_qml.rcc",0xe);
        QFile::QFile(local_d0,(QString *)&local_1d8);
        QString::~QString((QString *)&local_1d8);
        bVar3 = QFile::open(local_d0,1);
        if (bVar3) {
          pQVar11 = (QByteArray *)QIODevice::readAll((QIODevice *)local_d0);
          QByteArray::operator=((QByteArray *)(param_1 + 0x68),pQVar11);
          QByteArray::~QByteArray((QByteArray *)&local_d8);
          pQVar11 = (QByteArray *)thunk_FUN_140098690(&local_1f8,param_1 + 0x68);
          QByteArray::operator=((QByteArray *)(param_1 + 0x68),pQVar11);
          QByteArray::~QByteArray((QByteArray *)&local_1f8);
          QFileDevice::close((QFileDevice *)local_d0);
          pQVar10 = (QString *)QString::QString((QString *)local_1e0);
          puVar15 = (uchar *)QByteArray::constData((QByteArray *)(param_1 + 0x68));
          bVar3 = QResource::registerResource(puVar15,pQVar10);
          QString::~QString((QString *)local_1e0);
          if (bVar3) {
            if (*(int *)(*param_2 + 4) == 0) {
LAB_140097aaa:
              uVar8 = 0;
            }
            else {
              local_d8 = QString::fromAscii_helper(":/%1_qml.rcc",0xc);
              QChar::QChar((QChar *)local_1e0,0x20);
              QString::arg((QString *)&local_d8,&local_1f8,param_2,0);
              QString::~QString((QString *)&local_d8);
              QFile::setFileName(local_d0,(QString *)&local_1f8);
              bVar3 = QFile::open(local_d0,1);
              if (bVar3) {
                pQVar11 = (QByteArray *)QIODevice::readAll((QIODevice *)local_d0);
                QByteArray::operator=((QByteArray *)(param_1 + 0x70),pQVar11);
                QByteArray::~QByteArray((QByteArray *)&local_1d8);
                pQVar11 = (QByteArray *)thunk_FUN_140098690(&local_68,param_1 + 0x70);
                QByteArray::operator=((QByteArray *)(param_1 + 0x70),pQVar11);
                QByteArray::~QByteArray((QByteArray *)&local_68);
                QFileDevice::close((QFileDevice *)local_d0);
                pQVar10 = (QString *)QString::QString((QString *)local_1f0);
                puVar15 = (uchar *)QByteArray::constData((QByteArray *)(param_1 + 0x70));
                bVar3 = QResource::registerResource(puVar15,pQVar10);
                QString::~QString((QString *)local_1f0);
                if (bVar3) {
                  QString::~QString((QString *)&local_1f8);
                  goto LAB_140097aaa;
                }
                uVar5 = thunk_FUN_140034c40(local_1f0);
                cVar2 = thunk_FUN_140034800(uVar5,50000);
                thunk_FUN_140033660(local_1f0);
                if (cVar2 != '\0') {
                  thunk_FUN_140010ab0(&local_1c8,2,1);
                  thunk_FUN_140010400(&local_1c8,"failed to register additional qml resources");
                  uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
                  uVar9 = thunk_FUN_140034c40(local_1f0);
                  thunk_FUN_140033e30(uVar9,50000,uVar5,
                                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                                     );
                  thunk_FUN_140033660(local_1f0);
                  thunk_FUN_140011240(local_90);
                  goto LAB_140097a7e;
                }
              }
              else {
                uVar5 = thunk_FUN_140034c40(local_1f0);
                cVar2 = thunk_FUN_140034800(uVar5,50000);
                thunk_FUN_140033660(local_1f0);
                if (cVar2 != '\0') {
                  thunk_FUN_140010ab0(&local_1c8,2,1);
                  thunk_FUN_140010400(&local_1c8,"failed to open additional qml resources");
                  uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
                  uVar9 = thunk_FUN_140034c40(local_1f0);
                  thunk_FUN_140033e30(uVar9,50000,uVar5,
                                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                                     );
                  thunk_FUN_140033660(local_1f0);
                  thunk_FUN_140011240(local_90);
LAB_140097a7e:
                  thunk_FUN_1400111a0(local_140);
                  std::basic_ios<char,std::char_traits<char>_>::
                  ~basic_ios<char,std::char_traits<char>_>(local_140);
                }
              }
              QString::~QString((QString *)&local_1f8);
            }
            goto LAB_140097aad;
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,50000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 == '\0') goto LAB_140097aad;
          thunk_FUN_140010ab0(&local_1c8,2,1);
          thunk_FUN_140010400(&local_1c8,"failed to register base qml resources");
          uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
          uVar9 = thunk_FUN_140034c40(local_1f0);
          thunk_FUN_140033e30(uVar9,50000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                             );
          thunk_FUN_140033660(local_1f0);
          thunk_FUN_140011240(local_90);
        }
        else {
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,50000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 == '\0') goto LAB_140097aad;
          thunk_FUN_140010ab0(&local_1c8,2,1);
          thunk_FUN_140010400(&local_1c8,"failed to open base qml resources");
          uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
          uVar9 = thunk_FUN_140034c40(local_1f0);
          thunk_FUN_140033e30(uVar9,50000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                             );
          thunk_FUN_140033660(local_1f0);
          thunk_FUN_140011240(local_90);
        }
        thunk_FUN_1400111a0(local_140);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_140);
LAB_140097aad:
        QFile::~QFile(local_d0);
        return uVar8;
      }
      uVar8 = thunk_FUN_140034c40(&local_68);
      cVar2 = thunk_FUN_140034800(uVar8,50000);
      thunk_FUN_140033660(&local_68);
      if (cVar2 != '\0') {
        local_1c8 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_140);
        local_1e0[0] = uVar19 | 0x10;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
        *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
             *(int *)(local_1c8 + 4) + -0x88;
        thunk_FUN_140010c30(&local_1c0,2);
        thunk_FUN_140010400(&local_1c8,"unable to obtain OpenGL functions");
        uVar8 = thunk_FUN_140016d10(&local_1c8,local_90);
        uVar9 = thunk_FUN_140034c40(&local_68);
        thunk_FUN_140033e30(uVar9,50000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                           );
        thunk_FUN_140033660(&local_68);
        thunk_FUN_140011240(local_90);
        goto LAB_140095bce;
      }
    }
    else {
      uVar8 = thunk_FUN_140034c40(&local_68);
      cVar2 = thunk_FUN_140034800(uVar8,50000);
      thunk_FUN_140033660(&local_68);
      if (cVar2 != '\0') {
        local_1c8 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_140);
        local_1e0[0] = uVar19 | 8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
        *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
             *(int *)(local_1c8 + 4) + -0x88;
        thunk_FUN_140010c30(&local_1c0,2);
        thunk_FUN_140010400(&local_1c8,"unable to make OpenGL context current");
        uVar8 = thunk_FUN_140016d10(&local_1c8,local_90);
        uVar9 = thunk_FUN_140034c40(&local_68);
        thunk_FUN_140033e30(uVar9,50000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                           );
        thunk_FUN_140033660(&local_68);
        if (0xf < local_78) {
          uVar16 = local_90[0];
          if (0xfff < local_78 + 1) {
            if ((local_90[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar16 = *(ulonglong *)(local_90[0] - 8);
            if (local_90[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_90[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_90[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar16);
        }
        goto LAB_140095bce;
      }
    }
  }
  else {
    uVar8 = thunk_FUN_140034c40(&local_68);
    cVar2 = thunk_FUN_140034800(uVar8,50000);
    thunk_FUN_140033660(&local_68);
    if (cVar2 != '\0') {
      local_1c8 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_140);
      local_1e0[0] = uVar19 | 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
      *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
           *(int *)(local_1c8 + 4) + -0x88;
      thunk_FUN_140010c30(&local_1c0,2);
      thunk_FUN_140010400(&local_1c8,"unable to create OpenGL context");
      uVar8 = thunk_FUN_140016d10(&local_1c8,local_90);
      uVar9 = thunk_FUN_140034c40(&local_68);
      thunk_FUN_140033e30(uVar9,50000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                         );
      thunk_FUN_140033660(&local_68);
      if (0xf < local_78) {
        uVar16 = local_90[0];
        if (0xfff < local_78 + 1) {
          if ((local_90[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar16 = *(ulonglong *)(local_90[0] - 8);
          if (local_90[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_90[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_90[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar16);
      }
LAB_140095bce:
      thunk_FUN_1400111a0(local_140);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_140);
    }
  }
  QOpenGLContext::~QOpenGLContext((QOpenGLContext *)local_1f0);
  QWindow::~QWindow(local_c0);
LAB_1400975ae:
  QString::~QString((QString *)&local_1f8);
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400996a0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400996a0(QObject *param_1,longlong param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  int *piVar8;
  ulonglong uVar9;
  QObject *pQVar10;
  QScreen *this;
  undefined1 auStack_188 [32];
  code *local_168;
  undefined4 local_160;
  undefined4 local_158 [2];
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
  this = (QScreen *)0x0;
  local_158[0] = 0;
  if (param_2 == 0) {
    uVar6 = thunk_FUN_140034c40(local_150);
    cVar5 = thunk_FUN_140034800(uVar6,30000);
    thunk_FUN_140033660(local_150);
    if (cVar5 != '\0') {
      local_138 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158[0] = 1;
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
      thunk_FUN_140010400(&local_138,"ui: null screen track attempt");
      uVar6 = thunk_FUN_140016d10(&local_138,local_48);
      uVar7 = thunk_FUN_140034c40(local_150);
      local_168 = (code *)CONCAT44(local_168._4_4_,0x3b);
      thunk_FUN_140033e30(uVar7,30000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\screenTracker.cpp"
                         );
      thunk_FUN_140033660(local_150);
      if (0xf < local_30) {
        uVar9 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar9 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar9);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_140012670(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
  }
  else {
    lVar1 = *(longlong *)(param_1 + 0x20);
    if (((lVar1 != 0) && (*(int *)(lVar1 + 4) != 0)) && (*(longlong *)(param_1 + 0x28) != 0)) {
      pQVar10 = (QObject *)this;
      if ((lVar1 != 0) && (*(int *)(lVar1 + 4) != 0)) {
        pQVar10 = *(QObject **)(param_1 + 0x28);
      }
      QObject::disconnect(pQVar10,(char *)0x0,param_1,(char *)0x0);
      thunk_FUN_140099550(param_1 + 0x20,0,0);
    }
    thunk_FUN_140098e10(param_1 + 0x20,param_2);
    pQVar10 = (QObject *)this;
    if ((*(longlong *)(param_1 + 0x20) != 0) && (*(int *)(*(longlong *)(param_1 + 0x20) + 4) != 0))
    {
      pQVar10 = *(QObject **)(param_1 + 0x28);
    }
    local_160 = 0;
    local_168 = thunk_FUN_140099630;
    thunk_FUN_140098f30(local_158,pQVar10,availableGeometryChanged_exref,param_1);
    QMetaObject::Connection::~Connection((Connection *)local_158);
    if ((*(longlong *)(param_1 + 0x20) != 0) && (*(int *)(*(longlong *)(param_1 + 0x20) + 4) != 0))
    {
      this = *(QScreen **)(param_1 + 0x28);
    }
    piVar8 = (int *)QScreen::availableGeometry(this);
    if ((((*(int *)(param_1 + 0x10) != *piVar8) || (*(int *)(param_1 + 0x18) != piVar8[2])) ||
        (*(int *)(param_1 + 0x14) != piVar8[1])) || (*(int *)(param_1 + 0x1c) != piVar8[3])) {
      iVar2 = piVar8[1];
      iVar3 = piVar8[2];
      iVar4 = piVar8[3];
      *(int *)(param_1 + 0x10) = *piVar8;
      *(int *)(param_1 + 0x14) = iVar2;
      *(int *)(param_1 + 0x18) = iVar3;
      *(int *)(param_1 + 0x1c) = iVar4;
      thunk_FUN_140098890(param_1);
    }
  }
  return;
}

// =============================================================================
// NOTIFICARI SI SELF CONTROL
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400a20d0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400a20d0(undefined8 param_1,QJsonObject *param_2)

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
LAB_1400a219d:
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
    goto LAB_1400a219d;
  }
  QString::~QString((QString *)&local_310);
  if (pQVar8 == (QMapDataBase *)0x0) {
    if (*(uint *)local_338 != 0) {
      if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a2a62;
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 != 1) goto LAB_1400a2a62;
    }
    thunk_FUN_1400132a0(local_338);
    goto LAB_1400a2a62;
  }
  local_328 = QString::fromAscii_helper("data",4);
  if (1 < *(uint *)local_338) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_338 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_338 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_338 == 0) {
LAB_1400a2251:
      thunk_FUN_1400132a0(local_338);
    }
    else if (*(uint *)local_338 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2251;
    }
    local_338 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = *(QMapDataBase **)(local_338 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a22c1:
    pQVar7 = (QVariant *)QVariant::QVariant(local_2d0);
    if (1 < *(uint *)local_338) {
      pQVar5 = QMapDataBase::createData();
      if (*(longlong *)(local_338 + 0x10) != 0) {
        puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_338 + 0x10),pQVar5);
        *(ulonglong **)(pQVar5 + 0x10) = puVar6;
        *puVar6 = *puVar6 & 3;
        *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
      }
      if (*(uint *)local_338 == 0) {
LAB_1400a2329:
        thunk_FUN_1400132a0(local_338);
      }
      else if (*(uint *)local_338 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_338;
        *(uint *)local_338 = *(uint *)local_338 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400a2329;
      }
      local_338 = pQVar5;
      QMapDataBase::recalcMostLeftNode(pQVar5);
    }
    pQVar5 = local_338 + 8;
    bVar2 = true;
    pQVar13 = *(QMapDataBase **)(local_338 + 0x10);
    pQVar8 = pQVar9;
    if (*(QMapDataBase **)(local_338 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400a23ae:
      local_348 = CONCAT31(local_348._1_3_,bVar2);
      pQVar8 = (QMapDataBase *)thunk_FUN_140012ee0(local_338,&local_328,pQVar7,pQVar5);
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
      goto LAB_1400a23ae;
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
    goto LAB_1400a22c1;
  }
  QVariant::toMap((QVariant *)(pQVar8 + 0x20));
  QString::~QString((QString *)&local_328);
  local_308 = QString::fromAscii_helper("logo",4);
  pQVar5 = *(QMapDataBase **)(local_330 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a245d:
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
    goto LAB_1400a245d;
  }
  QString::~QString((QString *)&local_308);
  if (pQVar8 == (QMapDataBase *)0x0) {
    if (*(uint *)local_330 == 0) {
LAB_1400a2490:
      thunk_FUN_1400132a0(local_330);
    }
    else if (*(uint *)local_330 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_330;
      *(uint *)local_330 = *(uint *)local_330 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2490;
    }
    if (*(uint *)local_338 != 0) {
      if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a2a62;
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 != 1) goto LAB_1400a2a62;
    }
    thunk_FUN_1400132a0(local_338);
    goto LAB_1400a2a62;
  }
  local_320 = QString::fromAscii_helper("logo",4);
  if (1 < *(uint *)local_330) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_330 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_330 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_330 == 0) {
LAB_1400a2531:
      thunk_FUN_1400132a0(local_330);
    }
    else if (*(uint *)local_330 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_330;
      *(uint *)local_330 = *(uint *)local_330 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2531;
    }
    local_330 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = *(QMapDataBase **)(local_330 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a25a1:
    pQVar7 = (QVariant *)QVariant::QVariant(local_2c0);
    if (1 < *(uint *)local_330) {
      pQVar5 = QMapDataBase::createData();
      if (*(longlong *)(local_330 + 0x10) != 0) {
        puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_330 + 0x10),pQVar5);
        *(ulonglong **)(pQVar5 + 0x10) = puVar6;
        *puVar6 = *puVar6 & 3;
        *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
      }
      if (*(uint *)local_330 == 0) {
LAB_1400a2609:
        thunk_FUN_1400132a0(local_330);
      }
      else if (*(uint *)local_330 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_330;
        *(uint *)local_330 = *(uint *)local_330 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400a2609;
      }
      local_330 = pQVar5;
      QMapDataBase::recalcMostLeftNode(pQVar5);
    }
    pQVar5 = local_330 + 8;
    bVar2 = true;
    pQVar8 = *(QMapDataBase **)(local_330 + 0x10);
    if (*(QMapDataBase **)(local_330 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400a268b:
      local_348 = CONCAT31(local_348._1_3_,bVar2);
      pQVar9 = (QMapDataBase *)thunk_FUN_140012ee0(local_330,&local_320,pQVar7,pQVar5);
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
      goto LAB_1400a268b;
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
    goto LAB_1400a25a1;
  }
  QVariant::toString((QVariant *)(pQVar8 + 0x20));
  QString::~QString((QString *)&local_320);
  if (*(int *)(local_300 + 4) == 0) {
    uVar10 = thunk_FUN_140083150();
    cVar4 = thunk_FUN_140085520(uVar10);
    if (cVar4 != '\0') {
      uVar10 = thunk_FUN_140034c40(local_2b0);
      cVar4 = thunk_FUN_140034800(uVar10,10000);
      thunk_FUN_140033660(local_2b0);
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
        thunk_FUN_140010c30(local_260,2);
        thunk_FUN_140010400((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                            "Manager::onBrandingLogoChanged: branding logo has not been loaded, image is empty"
                           );
        uVar10 = thunk_FUN_140016d10((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                                     local_88);
        uVar11 = thunk_FUN_140034c40(local_2a0);
        local_348 = 0x5f;
        thunk_FUN_140033e30(uVar11,10000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-selfControl\\model.cpp"
                           );
        thunk_FUN_140033660(local_2a0);
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
          FUN_1400025c7(uVar12);
        }
        thunk_FUN_1400111a0(local_1e0);
        this = local_1e0;
LAB_1400a2a05:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this)
        ;
      }
    }
  }
  else {
    uVar10 = QString::QString(local_2f0,(QString *)&local_300);
    thunk_FUN_1400a3ef0(local_2e8,uVar10);
    uVar10 = thunk_FUN_140083150();
    cVar4 = thunk_FUN_140085520(uVar10);
    if (cVar4 != '\0') {
      uVar10 = thunk_FUN_140034c40(local_290);
      cVar4 = thunk_FUN_140034800(uVar10,10000);
      thunk_FUN_140033660(local_290);
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
        thunk_FUN_140010c30(local_170,2);
        thunk_FUN_140010400(&local_178,
                            "Manager::onBrandingLogoChanged: branding logo has been loaded");
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_280);
        local_348 = 100;
        thunk_FUN_140033e30(uVar11,10000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-selfControl\\model.cpp"
                           );
        thunk_FUN_140033660(local_280);
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
          FUN_1400025c7(uVar12);
        }
        thunk_FUN_1400111a0(local_f0);
        this = local_f0;
        goto LAB_1400a2a05;
      }
    }
  }
  QString::~QString((QString *)&local_300);
  if (*(uint *)local_330 == 0) {
LAB_1400a2a38:
    thunk_FUN_1400132a0(local_330);
  }
  else if (*(uint *)local_330 != 0xffffffff) {
    LOCK();
    uVar1 = *(uint *)local_330;
    *(uint *)local_330 = *(uint *)local_330 - 1;
    UNLOCK();
    if (uVar1 == 1) goto LAB_1400a2a38;
  }
  if (*(uint *)local_338 != 0) {
    if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a2a62;
    LOCK();
    uVar1 = *(uint *)local_338;
    *(uint *)local_338 = *(uint *)local_338 - 1;
    UNLOCK();
    if (uVar1 != 1) goto LAB_1400a2a62;
  }
  thunk_FUN_1400132a0(local_338);
LAB_1400a2a62:
  QJsonObject::~QJsonObject(param_2);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400a27e0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400a20d0(undefined8 param_1,QJsonObject *param_2)

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
LAB_1400a219d:
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
    goto LAB_1400a219d;
  }
  QString::~QString((QString *)&local_310);
  if (pQVar8 == (QMapDataBase *)0x0) {
    if (*(uint *)local_338 != 0) {
      if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a2a62;
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 != 1) goto LAB_1400a2a62;
    }
    thunk_FUN_1400132a0(local_338);
    goto LAB_1400a2a62;
  }
  local_328 = QString::fromAscii_helper("data",4);
  if (1 < *(uint *)local_338) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_338 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_338 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_338 == 0) {
LAB_1400a2251:
      thunk_FUN_1400132a0(local_338);
    }
    else if (*(uint *)local_338 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2251;
    }
    local_338 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = *(QMapDataBase **)(local_338 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a22c1:
    pQVar7 = (QVariant *)QVariant::QVariant(local_2d0);
    if (1 < *(uint *)local_338) {
      pQVar5 = QMapDataBase::createData();
      if (*(longlong *)(local_338 + 0x10) != 0) {
        puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_338 + 0x10),pQVar5);
        *(ulonglong **)(pQVar5 + 0x10) = puVar6;
        *puVar6 = *puVar6 & 3;
        *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
      }
      if (*(uint *)local_338 == 0) {
LAB_1400a2329:
        thunk_FUN_1400132a0(local_338);
      }
      else if (*(uint *)local_338 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_338;
        *(uint *)local_338 = *(uint *)local_338 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400a2329;
      }
      local_338 = pQVar5;
      QMapDataBase::recalcMostLeftNode(pQVar5);
    }
    pQVar5 = local_338 + 8;
    bVar2 = true;
    pQVar13 = *(QMapDataBase **)(local_338 + 0x10);
    pQVar8 = pQVar9;
    if (*(QMapDataBase **)(local_338 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400a23ae:
      local_348 = CONCAT31(local_348._1_3_,bVar2);
      pQVar8 = (QMapDataBase *)thunk_FUN_140012ee0(local_338,&local_328,pQVar7,pQVar5);
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
      goto LAB_1400a23ae;
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
    goto LAB_1400a22c1;
  }
  QVariant::toMap((QVariant *)(pQVar8 + 0x20));
  QString::~QString((QString *)&local_328);
  local_308 = QString::fromAscii_helper("logo",4);
  pQVar5 = *(QMapDataBase **)(local_330 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a245d:
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
    goto LAB_1400a245d;
  }
  QString::~QString((QString *)&local_308);
  if (pQVar8 == (QMapDataBase *)0x0) {
    if (*(uint *)local_330 == 0) {
LAB_1400a2490:
      thunk_FUN_1400132a0(local_330);
    }
    else if (*(uint *)local_330 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_330;
      *(uint *)local_330 = *(uint *)local_330 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2490;
    }
    if (*(uint *)local_338 != 0) {
      if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a2a62;
      LOCK();
      uVar1 = *(uint *)local_338;
      *(uint *)local_338 = *(uint *)local_338 - 1;
      UNLOCK();
      if (uVar1 != 1) goto LAB_1400a2a62;
    }
    thunk_FUN_1400132a0(local_338);
    goto LAB_1400a2a62;
  }
  local_320 = QString::fromAscii_helper("logo",4);
  if (1 < *(uint *)local_330) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_330 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_330 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_330 == 0) {
LAB_1400a2531:
      thunk_FUN_1400132a0(local_330);
    }
    else if (*(uint *)local_330 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_330;
      *(uint *)local_330 = *(uint *)local_330 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400a2531;
    }
    local_330 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = *(QMapDataBase **)(local_330 + 0x10);
  pQVar8 = pQVar9;
  if (pQVar5 == (QMapDataBase *)0x0) {
LAB_1400a25a1:
    pQVar7 = (QVariant *)QVariant::QVariant(local_2c0);
    if (1 < *(uint *)local_330) {
      pQVar5 = QMapDataBase::createData();
      if (*(longlong *)(local_330 + 0x10) != 0) {
        puVar6 = (ulonglong *)thunk_FUN_140012cf0(*(longlong *)(local_330 + 0x10),pQVar5);
        *(ulonglong **)(pQVar5 + 0x10) = puVar6;
        *puVar6 = *puVar6 & 3;
        *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
      }
      if (*(uint *)local_330 == 0) {
LAB_1400a2609:
        thunk_FUN_1400132a0(local_330);
      }
      else if (*(uint *)local_330 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_330;
        *(uint *)local_330 = *(uint *)local_330 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400a2609;
      }
      local_330 = pQVar5;
      QMapDataBase::recalcMostLeftNode(pQVar5);
    }
    pQVar5 = local_330 + 8;
    bVar2 = true;
    pQVar8 = *(QMapDataBase **)(local_330 + 0x10);
    if (*(QMapDataBase **)(local_330 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400a268b:
      local_348 = CONCAT31(local_348._1_3_,bVar2);
      pQVar9 = (QMapDataBase *)thunk_FUN_140012ee0(local_330,&local_320,pQVar7,pQVar5);
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
      goto LAB_1400a268b;
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
    goto LAB_1400a25a1;
  }
  QVariant::toString((QVariant *)(pQVar8 + 0x20));
  QString::~QString((QString *)&local_320);
  if (*(int *)(local_300 + 4) == 0) {
    uVar10 = thunk_FUN_140083150();
    cVar4 = thunk_FUN_140085520(uVar10);
    if (cVar4 != '\0') {
      uVar10 = thunk_FUN_140034c40(local_2b0);
      cVar4 = thunk_FUN_140034800(uVar10,10000);
      thunk_FUN_140033660(local_2b0);
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
        thunk_FUN_140010c30(local_260,2);
        thunk_FUN_140010400((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                            "Manager::onBrandingLogoChanged: branding logo has not been loaded, image is empty"
                           );
        uVar10 = thunk_FUN_140016d10((basic_ostream<char,std::char_traits<char>_> *)&local_268,
                                     local_88);
        uVar11 = thunk_FUN_140034c40(local_2a0);
        local_348 = 0x5f;
        thunk_FUN_140033e30(uVar11,10000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-selfControl\\model.cpp"
                           );
        thunk_FUN_140033660(local_2a0);
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
          FUN_1400025c7(uVar12);
        }
        thunk_FUN_1400111a0(local_1e0);
        this = local_1e0;
LAB_1400a2a05:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this)
        ;
      }
    }
  }
  else {
    uVar10 = QString::QString(local_2f0,(QString *)&local_300);
    thunk_FUN_1400a3ef0(local_2e8,uVar10);
    uVar10 = thunk_FUN_140083150();
    cVar4 = thunk_FUN_140085520(uVar10);
    if (cVar4 != '\0') {
      uVar10 = thunk_FUN_140034c40(local_290);
      cVar4 = thunk_FUN_140034800(uVar10,10000);
      thunk_FUN_140033660(local_290);
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
        thunk_FUN_140010c30(local_170,2);
        thunk_FUN_140010400(&local_178,
                            "Manager::onBrandingLogoChanged: branding logo has been loaded");
        uVar10 = thunk_FUN_140016d10(&local_178,local_68);
        uVar11 = thunk_FUN_140034c40(local_280);
        local_348 = 100;
        thunk_FUN_140033e30(uVar11,10000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-grabberSubAgent-ui-selfControl\\model.cpp"
                           );
        thunk_FUN_140033660(local_280);
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
          FUN_1400025c7(uVar12);
        }
        thunk_FUN_1400111a0(local_f0);
        this = local_f0;
        goto LAB_1400a2a05;
      }
    }
  }
  QString::~QString((QString *)&local_300);
  if (*(uint *)local_330 == 0) {
LAB_1400a2a38:
    thunk_FUN_1400132a0(local_330);
  }
  else if (*(uint *)local_330 != 0xffffffff) {
    LOCK();
    uVar1 = *(uint *)local_330;
    *(uint *)local_330 = *(uint *)local_330 - 1;
    UNLOCK();
    if (uVar1 == 1) goto LAB_1400a2a38;
  }
  if (*(uint *)local_338 != 0) {
    if (*(uint *)local_338 == 0xffffffff) goto LAB_1400a2a62;
    LOCK();
    uVar1 = *(uint *)local_338;
    *(uint *)local_338 = *(uint *)local_338 - 1;
    UNLOCK();
    if (uVar1 != 1) goto LAB_1400a2a62;
  }
  thunk_FUN_1400132a0(local_338);
LAB_1400a2a62:
  QJsonObject::~QJsonObject(param_2);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400a2d10
// -----------------------------------------------------------------------------
void FUN_1400a2d10(QObject *param_1,QObject param_2,QObject param_3,QObject param_4)

{
  QObject QVar1;
  QObject QVar2;
  
  QVar1 = param_1[0x10];
  if (QVar1 != param_2) {
    param_1[0x10] = param_2;
  }
  QVar2 = param_1[0x11];
  if (QVar2 != param_3) {
    param_1[0x11] = param_3;
  }
  if (param_1[0x12] != param_4) {
    param_1[0x12] = param_4;
    thunk_FUN_1400a3ed0();
    return;
  }
  if (QVar2 == param_3 && QVar1 == param_2) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0001400a3edd. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  QMetaObject::activate(param_1,(QMetaObject *)&DAT_140763488,0,(void **)0x0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400a36d0
// -----------------------------------------------------------------------------
ulonglong FUN_1400a3670(undefined8 param_1,int param_2,undefined8 param_3,undefined8 *param_4)

{
  int iVar1;
  ulonglong uVar2;
  
  uVar2 = thunk_FUN_1400a03c0();
  iVar1 = (int)uVar2;
  if (-1 < iVar1) {
    if (param_2 == 0) {
      if (iVar1 < 1) {
        thunk_FUN_1400a3520(param_1,0,uVar2 & 0xffffffff,param_4);
        return (ulonglong)(iVar1 - 1);
      }
    }
    else {
      if (param_2 != 0xc) {
        if ((param_2 - 1U < 3) || (param_2 == 0xb)) {
          thunk_FUN_1400a3520(param_1,param_2,uVar2 & 0xffffffff,param_4);
        }
        else if ((((param_2 != 4) && (param_2 != 5)) && (param_2 != 6)) &&
                ((param_2 != 7 && (param_2 != 8)))) {
          return uVar2 & 0xffffffff;
        }
        return (ulonglong)(iVar1 - 2);
      }
      if (iVar1 < 1) {
        *(undefined4 *)*param_4 = 0xffffffff;
      }
    }
    uVar2 = (ulonglong)(iVar1 - 1);
  }
  return uVar2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400a3ca0
// -----------------------------------------------------------------------------
QObject * FUN_1400a3ca0(QObject *param_1,uint param_2)

{
  QTimer::~QTimer((QTimer *)(param_1 + 0x20));
  QVariant::~QVariant((QVariant *)(param_1 + 0x10));
  QObject::~QObject(param_1);
  if ((param_2 & 1) != 0) {
    thunk_FUN_1400025c7(param_1,0x48);
  }
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400a47c0
// -----------------------------------------------------------------------------
QObject * FUN_1400a4780(QObject *param_1,undefined8 *param_2)

{
  int *piVar1;
  QTypedArrayData<unsigned_short> *pQVar2;
  
  QObject::QObject(param_1,(QObject *)0x0);
  *(undefined ***)param_1 = gui::ScreenTracker::vftable;
  QRect::QRect((QRect *)(param_1 + 0x10));
  *(undefined8 *)(param_1 + 0x20) = 0;
  *(undefined8 *)(param_1 + 0x28) = 0;
  *(undefined8 *)(param_1 + 0x30) = 0;
  *(undefined8 *)(param_1 + 0x38) = 0;
  *(undefined ***)param_1 = grabberSubAgent::ui::taskTracker::Controller::vftable;
  *(undefined8 *)(param_1 + 0x40) = 0;
  *(undefined8 *)(param_1 + 0x48) = 0;
  *(undefined8 *)(param_1 + 0x50) = *param_2;
  piVar1 = (int *)param_2[1];
  *(int **)(param_1 + 0x58) = piVar1;
  if (piVar1 != (int *)0x0) {
    LOCK();
    *piVar1 = *piVar1 + 1;
    UNLOCK();
    LOCK();
    *(int *)(*(longlong *)(param_1 + 0x58) + 4) = *(int *)(*(longlong *)(param_1 + 0x58) + 4) + 1;
    UNLOCK();
  }
  pQVar2 = QString::fromAscii_helper("qrc:/qml/TaskTrackerEntry.qml",0x1d);
  *(QTypedArrayData<unsigned_short> **)(param_1 + 0x60) = pQVar2;
  thunk_FUN_14001b770(param_2[1]);
  return param_1;
}

// =============================================================================
// GUI RESOURCES SI SCREEN
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400974a0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_140094cd0(longlong param_1,longlong *param_2)

{
  ulonglong uVar1;
  char cVar2;
  bool bVar3;
  DWORD DVar4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *pbVar6;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  QString *pQVar10;
  QByteArray *pQVar11;
  QGLContext *pQVar12;
  QRect *pQVar13;
  QOpenGLFunctions *this;
  char *pcVar14;
  uchar *puVar15;
  ulonglong uVar16;
  longlong lVar17;
  undefined *puVar18;
  uint uVar19;
  undefined *puVar20;
  undefined1 auStackY_228 [32];
  QTypedArrayData<unsigned_short> *local_1f8;
  undefined ***local_1f0 [2];
  uint local_1e0 [2];
  QTypedArrayData<unsigned_short> *local_1d8;
  int iStack_1cc;
  undefined *local_1c8;
  undefined **local_1c0;
  basic_ostream<char,std::char_traits<char>_> local_1b8 [96];
  undefined8 local_158;
  undefined4 local_150;
  basic_ios<char,std::char_traits<char>_> local_140 [104];
  QTypedArrayData<unsigned_short> *local_d8;
  QFile local_d0 [16];
  QWindow local_c0 [16];
  QSurface local_b0 [24];
  undefined8 local_98;
  ulonglong local_90 [3];
  ulonglong local_78;
  uint local_68;
  undefined4 uStack_64;
  uchar *puStack_60;
  ulonglong local_50;
  ulonglong local_48;
  
  local_98 = 0xfffffffffffffffe;
  local_48 = DAT_140763590 ^ (ulonglong)auStackY_228;
  uVar19 = 0;
  local_1e0[0] = 0;
  DVar4 = GetVersion();
  uVar5 = thunk_FUN_140034c40(local_1f0);
  cVar2 = thunk_FUN_140034800(uVar5,20000);
  thunk_FUN_140033660(local_1f0);
  uVar5 = 1;
  if (cVar2 != '\0') {
    local_1c8 = &DAT_1400dbe28;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_140)
    ;
    local_1e0[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
    *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
         *(int *)(local_1c8 + 4) + -0x88;
    local_1f0[0] = &local_1c0;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_1c0);
    local_1c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_158 = 0;
    local_150 = 4;
    pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_140010400(&local_1c8,"OS version mj: ");
    pbVar7 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,DVar4 & 0xff);
    pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_140010400(pbVar7," mn: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,DVar4 >> 8 & 0xff);
    uVar8 = thunk_FUN_140016d10(&local_1c8,&local_68);
    uVar9 = thunk_FUN_140034c40(local_1f0);
    thunk_FUN_140033e30(uVar9,20000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                       );
    thunk_FUN_140033660(local_1f0);
    if (0xf < local_50) {
      uVar1 = CONCAT44(uStack_64,local_68);
      uVar16 = uVar1;
      if (0xfff < local_50 + 1) {
        if ((local_68 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar16 = *(ulonglong *)(uVar1 - 8);
        if (uVar1 <= uVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (uVar1 - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < uVar1 - uVar16) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_1400025c7(uVar16);
    }
    *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
         *(int *)(local_1c8 + 4) + -0x88;
    local_1c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_140012670(&local_1c0);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_1c0);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_1b8);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_140);
    uVar19 = 1;
  }
  if ((DVar4 & 0xff) < 6) {
    return 4;
  }
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Bold.ttf",0x19);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-BoldItalic.ttf",0x1f);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-CondBold.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-CondLight.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-CondLightItalic.ttf",0x24);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-ExtraBold.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-ExtraBoldItalic.ttf",0x24);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Italic.ttf",0x1b);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Light.ttf",0x1a);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-LightItalic.ttf",0x20);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Regular.ttf",0x1c);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-Semibold.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/OpenSans-SemiboldItalic.ttf",0x23);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Black.ttf",0x18);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-BlackItalic.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Bold.ttf",0x17);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-BoldItalic.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Italic.ttf",0x19);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Light.ttf",0x18);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-LightItalic.ttf",0x1e);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Medium.ttf",0x19);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-MediumItalic.ttf",0x1f);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Regular.ttf",0x1a);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-Thin.ttf",0x17);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/Roboto-ThinItalic.ttf",0x1d);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Bold.ttf",0x20);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-BoldItalic.ttf",0x26);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Italic.ttf",0x22);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Light.ttf",0x21);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-LightItalic.ttf",0x27);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper(":/fonts/RobotoCondensed-Regular.ttf",0x23);
  QFontDatabase::addApplicationFont((QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  local_1f8 = QString::fromAscii_helper("Open Sans",9);
  QFont::QFont((QFont *)local_1f0,(QString *)&local_1f8,7,0x19,false);
  QString::~QString((QString *)&local_1f8);
  QApplication::setFont((QFont *)local_1f0,(char *)0x0);
  QFont::~QFont((QFont *)local_1f0);
  pQVar10 = (QString *)QCoreApplication::applicationDirPath();
  QDir::QDir((QDir *)&local_1d8,pQVar10);
  QString::~QString((QString *)&local_d8);
  QDir::cdUp((QDir *)&local_1d8);
  local_1f8 = QString::fromAscii_helper("fonts",5);
  QDir::cd((QDir *)&local_1d8,(QString *)&local_1f8);
  QString::~QString((QString *)&local_1f8);
  pQVar10 = (QString *)QDir::path((QDir *)&local_1d8);
  pQVar11 = (QByteArray *)QString::toLocal8Bit(pQVar10);
  qputenv("QT_QPA_FONTDIR",pQVar11);
  QByteArray::~QByteArray((QByteArray *)&local_1f8);
  QString::~QString((QString *)local_1e0);
  QDir::~QDir((QDir *)&local_1d8);
  bVar3 = QGLFormat::hasOpenGL();
  if (!bVar3) {
    uVar5 = thunk_FUN_140034c40(local_1f0);
    cVar2 = thunk_FUN_140034800(uVar5,50000);
    thunk_FUN_140033660(local_1f0);
    if (cVar2 != '\0') {
      local_1c8 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_140);
      local_1e0[0] = uVar19 | 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
      *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
           *(int *)(local_1c8 + 4) + -0x88;
      thunk_FUN_140010c30(&local_1c0,2);
      thunk_FUN_140010400(&local_1c8,"System does not support OpenGL");
      uVar5 = thunk_FUN_140016d10(&local_1c8,&local_68);
      uVar8 = thunk_FUN_140034c40(local_1f0);
      thunk_FUN_140033e30(uVar8,50000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                         );
      thunk_FUN_140033660(local_1f0);
      if (0xf < local_50) {
        uVar1 = CONCAT44(uStack_64,local_68);
        uVar16 = uVar1;
        if (0xfff < local_50 + 1) {
          if ((local_68 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar16 = *(ulonglong *)(uVar1 - 8);
          if (uVar1 <= uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (uVar1 - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < uVar1 - uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar16);
      }
      thunk_FUN_1400111a0(local_140);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_140);
    }
    return 1;
  }
  QString::QString((QString *)&local_1f8);
  pQVar12 = QGLContext::currentContext();
  QWindow::QWindow(local_c0,(QScreen *)0x0);
  QWindow::setSurfaceType(local_c0,1);
  uVar8 = 3;
  pQVar13 = (QRect *)QRect::QRect((QRect *)&local_68,0,0,3,3);
  QWindow::setGeometry(local_c0,pQVar13);
  QWindow::create(local_c0);
  QOpenGLContext::QOpenGLContext((QOpenGLContext *)local_1f0,(QObject *)0x0);
  bVar3 = QOpenGLContext::create((QOpenGLContext *)local_1f0);
  if (bVar3) {
    bVar3 = QOpenGLContext::makeCurrent((QOpenGLContext *)local_1f0,local_b0);
    if (bVar3) {
      this = QOpenGLContext::functions((QOpenGLContext *)local_1f0);
      if (this != (QOpenGLFunctions *)0x0) {
        puStack_60 = QOpenGLFunctions::glGetString(this,0x1f02);
        if (puStack_60 == (uchar *)0x0) {
          local_68 = 0;
        }
        else {
          lVar17 = -1;
          do {
            lVar17 = lVar17 + 1;
          } while (puStack_60[lVar17] != '\0');
          local_68 = (uint)lVar17;
        }
        QString::operator=((QString *)&local_1f8,&local_68);
        if (pQVar12 != (QGLContext *)0x0) {
          (**(code **)(*(longlong *)pQVar12 + 0x10))(pQVar12);
        }
        QOpenGLContext::~QOpenGLContext((QOpenGLContext *)local_1f0);
        QWindow::~QWindow(local_c0);
        uVar5 = thunk_FUN_140034c40(local_1f0);
        cVar2 = thunk_FUN_140034800(uVar5,20000);
        thunk_FUN_140033660(local_1f0);
        if (cVar2 != '\0') {
          local_1c8 = &DAT_1400dbe28;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_140);
          uVar19 = uVar19 | 0x20;
          local_1e0[0] = uVar19;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
          *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
               *(int *)(local_1c8 + 4) + -0x88;
          thunk_FUN_140010c30(&local_1c0,2);
          uVar5 = thunk_FUN_140010400(&local_1c8,"glVersionString: ");
          pQVar11 = (QByteArray *)QString::toLocal8Bit((QString *)&local_1f8);
          pcVar14 = QByteArray::data(pQVar11);
          thunk_FUN_140010400(uVar5,pcVar14);
          QByteArray::~QByteArray((QByteArray *)&local_d8);
          uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
          uVar9 = thunk_FUN_140034c40(local_1f0);
          thunk_FUN_140033e30(uVar9,20000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                             );
          thunk_FUN_140033660(local_1f0);
          thunk_FUN_140011240(local_90);
          thunk_FUN_1400111a0(local_140);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_140);
        }
        QGLFormat::openGLVersionFlags();
        if ((((ulonglong)local_1d8 & 0x40) == 0) && (((ulonglong)local_1d8 & 0x800) == 0)) {
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          puVar20 = &DAT_1400f67a4;
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x40;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 1) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x80;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 2) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.2: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x100;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 4) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.3: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x200;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 8) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.4: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x400;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x10) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 1.5: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x800;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x20) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 2.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x1000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x40) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 2.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x2000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x1000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x4000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x2000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x8000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x4000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.2: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x10000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x8000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 3.3: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x20000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x10000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x40000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x20000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x80000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x40000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.2: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x100000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x80000) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL 4.3: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x200000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x80) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common 1.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x400000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x200) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common 1.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x800000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x100) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common Lite 1.0: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x1000000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            puVar18 = &DAT_1400f67a4;
            if (((ulonglong)local_1d8 & 0x400) != 0) {
              puVar18 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES Common Lite 1.1: ");
            thunk_FUN_140010400(uVar5,puVar18);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,20000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            uVar19 = uVar19 | 0x2000000;
            local_1e0[0] = uVar19;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            if (((uint)local_1d8 & 0x800) != 0) {
              puVar20 = &DAT_1400f67a0;
            }
            uVar5 = thunk_FUN_140010400(&local_1c8,"OGL ES 2.0: ");
            thunk_FUN_140010400(uVar5,puVar20);
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,20000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,50000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 != '\0') {
            local_1c8 = &DAT_1400dbe28;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_140);
            local_1e0[0] = uVar19 | 0x4000000;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
            *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
                 *(int *)(local_1c8 + 4) + -0x88;
            thunk_FUN_140010c30(&local_1c0,2);
            thunk_FUN_140010400(&local_1c8,"OpenGL version lower than 2.1");
            uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
            uVar8 = thunk_FUN_140034c40(local_1f0);
            thunk_FUN_140033e30(uVar8,50000,uVar5,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                               );
            thunk_FUN_140033660(local_1f0);
            thunk_FUN_140011240(local_90);
            thunk_FUN_1400111a0(local_140);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_140);
          }
          uVar5 = 2;
          goto LAB_1400975ae;
        }
        QString::~QString((QString *)&local_1f8);
        local_1d8 = QString::fromAscii_helper(":/base_qml.rcc",0xe);
        QFile::QFile(local_d0,(QString *)&local_1d8);
        QString::~QString((QString *)&local_1d8);
        bVar3 = QFile::open(local_d0,1);
        if (bVar3) {
          pQVar11 = (QByteArray *)QIODevice::readAll((QIODevice *)local_d0);
          QByteArray::operator=((QByteArray *)(param_1 + 0x68),pQVar11);
          QByteArray::~QByteArray((QByteArray *)&local_d8);
          pQVar11 = (QByteArray *)thunk_FUN_140098690(&local_1f8,param_1 + 0x68);
          QByteArray::operator=((QByteArray *)(param_1 + 0x68),pQVar11);
          QByteArray::~QByteArray((QByteArray *)&local_1f8);
          QFileDevice::close((QFileDevice *)local_d0);
          pQVar10 = (QString *)QString::QString((QString *)local_1e0);
          puVar15 = (uchar *)QByteArray::constData((QByteArray *)(param_1 + 0x68));
          bVar3 = QResource::registerResource(puVar15,pQVar10);
          QString::~QString((QString *)local_1e0);
          if (bVar3) {
            if (*(int *)(*param_2 + 4) == 0) {
LAB_140097aaa:
              uVar8 = 0;
            }
            else {
              local_d8 = QString::fromAscii_helper(":/%1_qml.rcc",0xc);
              QChar::QChar((QChar *)local_1e0,0x20);
              QString::arg((QString *)&local_d8,&local_1f8,param_2,0);
              QString::~QString((QString *)&local_d8);
              QFile::setFileName(local_d0,(QString *)&local_1f8);
              bVar3 = QFile::open(local_d0,1);
              if (bVar3) {
                pQVar11 = (QByteArray *)QIODevice::readAll((QIODevice *)local_d0);
                QByteArray::operator=((QByteArray *)(param_1 + 0x70),pQVar11);
                QByteArray::~QByteArray((QByteArray *)&local_1d8);
                pQVar11 = (QByteArray *)thunk_FUN_140098690(&local_68,param_1 + 0x70);
                QByteArray::operator=((QByteArray *)(param_1 + 0x70),pQVar11);
                QByteArray::~QByteArray((QByteArray *)&local_68);
                QFileDevice::close((QFileDevice *)local_d0);
                pQVar10 = (QString *)QString::QString((QString *)local_1f0);
                puVar15 = (uchar *)QByteArray::constData((QByteArray *)(param_1 + 0x70));
                bVar3 = QResource::registerResource(puVar15,pQVar10);
                QString::~QString((QString *)local_1f0);
                if (bVar3) {
                  QString::~QString((QString *)&local_1f8);
                  goto LAB_140097aaa;
                }
                uVar5 = thunk_FUN_140034c40(local_1f0);
                cVar2 = thunk_FUN_140034800(uVar5,50000);
                thunk_FUN_140033660(local_1f0);
                if (cVar2 != '\0') {
                  thunk_FUN_140010ab0(&local_1c8,2,1);
                  thunk_FUN_140010400(&local_1c8,"failed to register additional qml resources");
                  uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
                  uVar9 = thunk_FUN_140034c40(local_1f0);
                  thunk_FUN_140033e30(uVar9,50000,uVar5,
                                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                                     );
                  thunk_FUN_140033660(local_1f0);
                  thunk_FUN_140011240(local_90);
                  goto LAB_140097a7e;
                }
              }
              else {
                uVar5 = thunk_FUN_140034c40(local_1f0);
                cVar2 = thunk_FUN_140034800(uVar5,50000);
                thunk_FUN_140033660(local_1f0);
                if (cVar2 != '\0') {
                  thunk_FUN_140010ab0(&local_1c8,2,1);
                  thunk_FUN_140010400(&local_1c8,"failed to open additional qml resources");
                  uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
                  uVar9 = thunk_FUN_140034c40(local_1f0);
                  thunk_FUN_140033e30(uVar9,50000,uVar5,
                                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                                     );
                  thunk_FUN_140033660(local_1f0);
                  thunk_FUN_140011240(local_90);
LAB_140097a7e:
                  thunk_FUN_1400111a0(local_140);
                  std::basic_ios<char,std::char_traits<char>_>::
                  ~basic_ios<char,std::char_traits<char>_>(local_140);
                }
              }
              QString::~QString((QString *)&local_1f8);
            }
            goto LAB_140097aad;
          }
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,50000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 == '\0') goto LAB_140097aad;
          thunk_FUN_140010ab0(&local_1c8,2,1);
          thunk_FUN_140010400(&local_1c8,"failed to register base qml resources");
          uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
          uVar9 = thunk_FUN_140034c40(local_1f0);
          thunk_FUN_140033e30(uVar9,50000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                             );
          thunk_FUN_140033660(local_1f0);
          thunk_FUN_140011240(local_90);
        }
        else {
          uVar5 = thunk_FUN_140034c40(local_1f0);
          cVar2 = thunk_FUN_140034800(uVar5,50000);
          thunk_FUN_140033660(local_1f0);
          if (cVar2 == '\0') goto LAB_140097aad;
          thunk_FUN_140010ab0(&local_1c8,2,1);
          thunk_FUN_140010400(&local_1c8,"failed to open base qml resources");
          uVar5 = thunk_FUN_140016d10(&local_1c8,local_90);
          uVar9 = thunk_FUN_140034c40(local_1f0);
          thunk_FUN_140033e30(uVar9,50000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                             );
          thunk_FUN_140033660(local_1f0);
          thunk_FUN_140011240(local_90);
        }
        thunk_FUN_1400111a0(local_140);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_140);
LAB_140097aad:
        QFile::~QFile(local_d0);
        return uVar8;
      }
      uVar8 = thunk_FUN_140034c40(&local_68);
      cVar2 = thunk_FUN_140034800(uVar8,50000);
      thunk_FUN_140033660(&local_68);
      if (cVar2 != '\0') {
        local_1c8 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_140);
        local_1e0[0] = uVar19 | 0x10;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
        *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
             *(int *)(local_1c8 + 4) + -0x88;
        thunk_FUN_140010c30(&local_1c0,2);
        thunk_FUN_140010400(&local_1c8,"unable to obtain OpenGL functions");
        uVar8 = thunk_FUN_140016d10(&local_1c8,local_90);
        uVar9 = thunk_FUN_140034c40(&local_68);
        thunk_FUN_140033e30(uVar9,50000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                           );
        thunk_FUN_140033660(&local_68);
        thunk_FUN_140011240(local_90);
        goto LAB_140095bce;
      }
    }
    else {
      uVar8 = thunk_FUN_140034c40(&local_68);
      cVar2 = thunk_FUN_140034800(uVar8,50000);
      thunk_FUN_140033660(&local_68);
      if (cVar2 != '\0') {
        local_1c8 = &DAT_1400dbe28;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_140);
        local_1e0[0] = uVar19 | 8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
        *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
             *(int *)(local_1c8 + 4) + -0x88;
        thunk_FUN_140010c30(&local_1c0,2);
        thunk_FUN_140010400(&local_1c8,"unable to make OpenGL context current");
        uVar8 = thunk_FUN_140016d10(&local_1c8,local_90);
        uVar9 = thunk_FUN_140034c40(&local_68);
        thunk_FUN_140033e30(uVar9,50000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                           );
        thunk_FUN_140033660(&local_68);
        if (0xf < local_78) {
          uVar16 = local_90[0];
          if (0xfff < local_78 + 1) {
            if ((local_90[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar16 = *(ulonglong *)(local_90[0] - 8);
            if (local_90[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_90[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_90[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_1400025c7(uVar16);
        }
        goto LAB_140095bce;
      }
    }
  }
  else {
    uVar8 = thunk_FUN_140034c40(&local_68);
    cVar2 = thunk_FUN_140034800(uVar8,50000);
    thunk_FUN_140033660(&local_68);
    if (cVar2 != '\0') {
      local_1c8 = &DAT_1400dbe28;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_140);
      local_1e0[0] = uVar19 | 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1c8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1c0,false);
      *(undefined ***)((longlong)&local_1c8 + (longlong)*(int *)(local_1c8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1cc + (longlong)*(int *)(local_1c8 + 4)) =
           *(int *)(local_1c8 + 4) + -0x88;
      thunk_FUN_140010c30(&local_1c0,2);
      thunk_FUN_140010400(&local_1c8,"unable to create OpenGL context");
      uVar8 = thunk_FUN_140016d10(&local_1c8,local_90);
      uVar9 = thunk_FUN_140034c40(&local_68);
      thunk_FUN_140033e30(uVar9,50000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-gui-base\\initializer.cpp"
                         );
      thunk_FUN_140033660(&local_68);
      if (0xf < local_78) {
        uVar16 = local_90[0];
        if (0xfff < local_78 + 1) {
          if ((local_90[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar16 = *(ulonglong *)(local_90[0] - 8);
          if (local_90[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_90[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_90[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_1400025c7(uVar16);
      }
LAB_140095bce:
      thunk_FUN_1400111a0(local_140);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_140);
    }
  }
  QOpenGLContext::~QOpenGLContext((QOpenGLContext *)local_1f0);
  QWindow::~QWindow(local_c0);
LAB_1400975ae:
  QString::~QString((QString *)&local_1f8);
  return uVar5;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14009d390
// -----------------------------------------------------------------------------
int FUN_14009d1a0(undefined8 param_1,undefined4 param_2,undefined4 param_3,undefined8 param_4,
                 QString *param_5)

{
  char *pcVar1;
  QByteArray *pQVar2;
  char *pcVar3;
  int iVar4;
  longlong lVar5;
  int iVar7;
  int iVar8;
  size_t _Size;
  longlong local_res20;
  QByteArray local_168 [16];
  int local_158;
  int local_154;
  char *local_150;
  char local_148 [48];
  int local_118;
  int local_114;
  char *local_110;
  char local_108 [64];
  undefined4 local_c8;
  int local_c4;
  int local_c0;
  undefined4 local_bc;
  undefined8 local_b8;
  QString local_b0 [8];
  undefined8 local_a8;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined8 local_98;
  undefined *local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined4 local_50;
  undefined8 local_48;
  longlong lVar6;
  
  local_48 = 0xfffffffffffffffe;
  pcVar1 = QMetaObject::className((QMetaObject *)&DAT_140763448);
  lVar5 = -1;
  do {
    lVar6 = lVar5;
    lVar5 = lVar6 + 1;
  } while (pcVar1[lVar5] != '\0');
  iVar8 = (int)lVar6;
  local_154 = iVar8 + 3;
  if (local_154 < 0) {
    qt_assert_x("QVarLengthArray::QVarLengthArray()","Size must be greater than or equal to 0.",
                "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qvarlengtharray.h",
                0xe9);
  }
  if (local_154 < 0x31) {
    local_150 = local_148;
    local_158 = 0x30;
  }
  else {
    local_150 = malloc((longlong)local_154);
    if (local_150 == (char *)0x0) {
      qBadAlloc();
    }
    local_158 = local_154;
  }
  iVar4 = (int)lVar5;
  _Size = (size_t)iVar4;
  memcpy(local_150,pcVar1,_Size);
  if ((iVar4 < 0) || (local_154 <= iVar4)) {
    qt_assert("idx >= 0 && idx < s",
              "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qvarlengtharray.h",0x81)
    ;
  }
  local_150[_Size] = '*';
  iVar7 = iVar8 + 2;
  if ((iVar7 < 0) || (local_154 <= iVar7)) {
    qt_assert("idx >= 0 && idx < s",
              "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qvarlengtharray.h",0x81)
    ;
  }
  local_150[iVar7] = '\0';
  local_114 = iVar8 + 0x14;
  if (local_114 < 0) {
    qt_assert_x("QVarLengthArray::QVarLengthArray()","Size must be greater than or equal to 0.",
                "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qvarlengtharray.h",
                0xe9);
  }
  if (local_114 < 0x41) {
    local_110 = local_108;
    local_118 = 0x40;
  }
  else {
    local_110 = malloc((longlong)local_114);
    if (local_110 == (char *)0x0) {
      qBadAlloc();
    }
    local_118 = local_114;
  }
  *(undefined8 *)local_110 = s_QQmlListProperty<_1400f4c30._0_8_;
  *(undefined8 *)(local_110 + 8) = s_QQmlListProperty<_1400f4c30._8_8_;
  local_110[0x10] = s_QQmlListProperty<_1400f4c30[0x10];
  memcpy(local_110 + 0x11,pcVar1,_Size);
  iVar8 = iVar8 + 0x12;
  if ((iVar8 < 0) || (local_114 <= iVar8)) {
    qt_assert("idx >= 0 && idx < s",
              "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qvarlengtharray.h",0x81)
    ;
  }
  local_110[iVar8] = '>';
  iVar4 = iVar4 + 0x12;
  if ((iVar4 < 0) || (local_114 <= iVar4)) {
    qt_assert("idx >= 0 && idx < s",
              "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qvarlengtharray.h",0x81)
    ;
  }
  local_110[iVar4] = '\0';
  local_c8 = 0;
  QByteArray::QByteArray((QByteArray *)&local_res20,local_150,-1);
  QByteArray::constData((QByteArray *)&local_res20);
  pQVar2 = (QByteArray *)QMetaObject::normalizedType((char *)local_168);
  iVar8 = *(int *)(local_res20 + 4);
  if (iVar8 == *(int *)(*(longlong *)pQVar2 + 4)) {
    pcVar1 = QByteArray::constData(pQVar2);
    pcVar3 = QByteArray::constData((QByteArray *)&local_res20);
    iVar8 = memcmp(pcVar3,pcVar1,(longlong)iVar8);
    if (iVar8 != 0) goto LAB_14009d445;
  }
  else {
LAB_14009d445:
    qt_assert_x("qRegisterNormalizedMetaType",
                "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
                ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
  }
  QByteArray::~QByteArray(local_168);
  iVar8 = thunk_FUN_14009efd0();
  if (iVar8 == -1) {
    local_c4 = QMetaType::registerNormalizedType
                         ((QByteArray *)&local_res20,&LAB_140001717,&LAB_140004052,8,0x10c,
                          &DAT_140763448);
  }
  else {
    local_c4 = QMetaType::registerNormalizedTypedef((QByteArray *)&local_res20,iVar8);
  }
  QByteArray::~QByteArray((QByteArray *)&local_res20);
  QByteArray::QByteArray((QByteArray *)&local_res20,local_110,-1);
  QByteArray::constData((QByteArray *)&local_res20);
  pQVar2 = (QByteArray *)QMetaObject::normalizedType((char *)local_168);
  iVar8 = *(int *)(local_res20 + 4);
  if (iVar8 == *(int *)(*(longlong *)pQVar2 + 4)) {
    pcVar1 = QByteArray::constData(pQVar2);
    pcVar3 = QByteArray::constData((QByteArray *)&local_res20);
    iVar8 = memcmp(pcVar3,pcVar1,(longlong)iVar8);
    if (iVar8 == 0) goto LAB_14009d567;
  }
  qt_assert_x("qRegisterNormalizedMetaType",
              "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
              ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
LAB_14009d567:
  QByteArray::~QByteArray(local_168);
  local_c0 = QMetaType::registerNormalizedType(&local_res20,&LAB_14000533f,&LAB_140003a03,0x40,3,0);
  QByteArray::~QByteArray((QByteArray *)&local_res20);
  local_bc = 0;
  local_b8 = 0;
  QString::QString(local_b0,param_5);
  local_90 = &DAT_140763448;
  local_88 = 0;
  local_80 = 0;
  local_78 = 0xffffffffffffffff;
  local_70 = 0xffffffff;
  local_68 = 0;
  local_60 = 0;
  local_58 = 0;
  local_50 = 0;
  local_a8 = param_1;
  local_a0 = param_2;
  local_9c = param_3;
  local_98 = param_4;
  iVar8 = QQmlPrivate::qmlregister(0,&local_c8);
  QString::~QString(local_b0);
  if (local_110 != local_108) {
    free(local_110);
  }
  if (local_150 != local_148) {
    free(local_150);
  }
  return iVar8;
}
