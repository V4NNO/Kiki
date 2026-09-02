/*
 * viewer.exe - pseudocod decompilat cu Ghidra MCP
 * Data analizei: 2026-09-02
 *
 * IMPORTANT:
 * - Acesta nu este codul-sursa original si nu este garantat compilabil.
 * - Tipurile undefined*, numele FUN_/DAT_/LAB_ si prototipurile sunt cele
 *   produse de decompilatorul Ghidra.
 * - Sunt incluse numai functii existente in programul analizat; nu au fost
 *   inventate functii si nu au fost rescrise implementari.
 * - Selectia acopera pornirea, initializarea QML, controlul remote,
 *   video online/istoric, audio, keylogger si callback-urile TLS PE.
 */

// =============================================================================
// Ghidra: entry @ 0x140024a8c
// =============================================================================
void entry(void)

{
  __security_init_cookie();
  FUN_1404d5f00();
  return;
}

// =============================================================================
// Ghidra: FUN_1404d5f00 @ 0x1404d5f00
// =============================================================================
int FUN_1404d5f00(void)

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
  if (DAT_140ea1260 == 1) {
                    /* WARNING: Subroutine does not return */
    __scrt_fastfail(7);
  }
  if (DAT_140ea1260 == 0) {
    DAT_140ea1260 = 1;
    iVar6 = _initterm_e(&DAT_14059dce8,&DAT_14059e128);
    if (iVar6 != 0) {
      return 0xff;
    }
    _initterm(&DAT_14059b000,&DAT_14059d8a8);
    DAT_140ea1260 = 2;
  }
  else {
    bVar2 = true;
  }
  __scrt_release_startup_lock(uVar4);
  plVar7 = (longlong *)thunk_FUN_1404d6fd4();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    pcVar1 = (code *)*plVar7;
    _guard_check_icall(pcVar1);
    (*pcVar1)(0,2);
  }
  plVar7 = (longlong *)thunk_FUN_1404d6fe0();
  if ((*plVar7 != 0) && (cVar3 = __scrt_is_nonwritable_in_current_image(plVar7), cVar3 != '\0')) {
    _register_thread_local_exe_atexit_callback(*plVar7);
  }
  uVar5 = __scrt_get_show_window_mode();
  uVar8 = _get_narrow_winmain_command_line();
  iVar6 = thunk_FUN_1404ebac0(&IMAGE_DOS_HEADER_140000000,0,uVar8,uVar5);
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
// Ghidra: FUN_140108c30 @ 0x140108c30
// =============================================================================
int FUN_140108c30(int param_1,longlong param_2)

{
  bool bVar1;
  code *this;
  char cVar2;
  bool bVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  QChar *pQVar9;
  QString *this_00;
  QByteArray *this_01;
  char *pcVar10;
  longlong lVar11;
  longlong lVar12;
  int local_res8 [2];
  QTypedArrayData<unsigned_short> *local_res10;
  QTypedArrayData<unsigned_short> *local_res18;
  QTypedArrayData<unsigned_short> *local_res20;
  undefined8 in_stack_fffffffffffffe48;
  QEventLoop local_1a8 [64];
  undefined8 local_168;
  undefined1 local_158 [48];
  undefined1 local_128 [240];
  
  uVar5 = (undefined4)((ulonglong)in_stack_fffffffffffffe48 >> 0x20);
  local_168 = 0xfffffffffffffffe;
  bVar3 = false;
  local_res10 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_res10 & 0xffffffff00000000);
  local_res8[0] = param_1;
  local_res10 = QString::fromAscii_helper("KickidlerViewer",0xf);
  QCoreApplication::setApplicationName((QString *)&local_res10);
  QString::~QString((QString *)&local_res10);
  local_res18 = (QTypedArrayData<unsigned_short> *)&local_res10;
  uVar7 = QFile::decodeName((char *)&local_res10);
  uVar8 = thunk_FUN_1400ec280();
  thunk_FUN_1401785e0(uVar8,4,uVar7);
  uVar7 = thunk_FUN_1400f4c80(&local_res10,0);
  thunk_FUN_140393630(uVar7);
  QString::~QString((QString *)&local_res10);
  lVar12 = -1;
  if (local_res8[0] == 2) {
    local_res18 = QString::fromAscii_helper("-utilize-invite",0xf);
    bVar3 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_res10._4_4_,1);
    pcVar10 = *(char **)(param_2 + 8);
    iVar4 = *(int *)(local_res18 + 4);
    pQVar9 = QString::constData((QString *)&local_res18);
    iVar4 = QString::compare_helper(pQVar9,iVar4,pcVar10,-1,1);
    if (iVar4 != 0) goto LAB_140108d59;
    bVar1 = true;
  }
  else {
LAB_140108d59:
    bVar1 = false;
  }
  if (bVar3) {
    QString::~QString((QString *)&local_res18);
  }
  bVar3 = false;
  if (bVar1) {
    thunk_FUN_14010c420(local_1a8,2,local_res8,param_2);
    iVar4 = QCoreApplication::exec();
    thunk_FUN_14010ce20(local_1a8);
    return iVar4;
  }
  if (local_res8[0] == 3) {
    local_res18 = QString::fromAscii_helper("-encrypt",8);
    bVar3 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)
                  ((ulonglong)local_res10 & 0xffffffff00000000 | 2);
    pcVar10 = *(char **)(param_2 + 8);
    iVar4 = *(int *)(local_res18 + 4);
    pQVar9 = QString::constData((QString *)&local_res18);
    iVar4 = QString::compare_helper(pQVar9,iVar4,pcVar10,-1,1);
    if (iVar4 != 0) goto LAB_140108e09;
    bVar1 = true;
  }
  else {
LAB_140108e09:
    bVar1 = false;
  }
  if (bVar3) {
    QString::~QString((QString *)&local_res18);
  }
  bVar3 = false;
  if (bVar1) {
    pcVar10 = *(char **)(param_2 + 0x10);
    if (pcVar10 != (char *)0x0) {
      do {
        lVar12 = lVar12 + 1;
      } while (pcVar10[lVar12] != '\0');
    }
    local_res10 = QString::fromAscii_helper(pcVar10,(int)lVar12);
    this_00 = (QString *)thunk_FUN_1401e87b0(&local_res20,&local_res10);
    this = cout_exref;
    this_01 = (QByteArray *)QString::toLocal8Bit(this_00);
    pcVar10 = QByteArray::data(this_01);
    thunk_FUN_14006fb70(this,pcVar10);
    QByteArray::~QByteArray((QByteArray *)&local_res18);
    std::basic_ostream<char,std::char_traits<char>_>::operator<<
              ((basic_ostream<char,std::char_traits<char>_> *)this,thunk_FUN_140106c80);
    QString::~QString((QString *)&local_res20);
    QString::~QString((QString *)&local_res10);
    return 0;
  }
  if (local_res8[0] < 4) {
LAB_140108f11:
    bVar1 = false;
  }
  else {
    local_res18 = QString::fromAscii_helper("report",6);
    bVar3 = true;
    local_res10 = (QTypedArrayData<unsigned_short> *)
                  ((ulonglong)local_res10 & 0xffffffff00000000 | 4);
    cVar2 = thunk_FUN_1401071c0(*(undefined8 *)(param_2 + 8),&local_res18);
    if (cVar2 == '\0') goto LAB_140108f11;
    bVar1 = true;
  }
  if (bVar3) {
    QString::~QString((QString *)&local_res18);
  }
  if (!bVar1) {
    uVar7 = thunk_FUN_1400f4c80(&local_res18,8);
    uVar7 = thunk_FUN_140079410(&local_res10,uVar7,"/viewer");
    thunk_FUN_14039c470(uVar7,0xa00000,10);
    QString::~QString((QString *)&local_res18);
    qInstallMessageHandler(thunk_FUN_140107d50);
    thunk_FUN_1400beee0(local_158,local_res8,param_2);
    local_res10 = QString::fromAscii_helper(":/images/kickidler.ico",0x16);
    uVar7 = QIcon::QIcon((QIcon *)&local_res18,(QString *)&local_res10);
    thunk_FUN_1400f9210(local_128,uVar7);
    QString::~QString((QString *)&local_res10);
    uVar6 = thunk_FUN_1400e6a10(local_158);
    switch(uVar6) {
    case 0:
      iVar4 = QApplication::exec();
      thunk_FUN_1400db350(local_158);
      QEventLoop::QEventLoop(local_1a8,(QObject *)0x0);
      do {
        bVar3 = QEventLoop::processEvents(local_1a8,0);
      } while (bVar3);
      QEventLoop::~QEventLoop(local_1a8);
      break;
    case 1:
      iVar4 = 0;
      break;
    case 2:
      local_res18 = QString::fromAscii_helper("Application failed to initialize",0x20);
      local_res10 = QString::fromAscii_helper("Kickidler",9);
      QMessageBox::QMessageBox
                ((QMessageBox *)local_1a8,3,&local_res10,&local_res18,0x400,0,CONCAT44(uVar5,0x103))
      ;
      QString::~QString((QString *)&local_res10);
      QString::~QString((QString *)&local_res18);
      QWidget::show((QWidget *)local_1a8);
      iVar4 = QApplication::exec();
      QMessageBox::~QMessageBox((QMessageBox *)local_1a8);
      break;
    case 3:
      local_res18 = QString::fromAscii_helper("Application already running",0x1b);
      local_res10 = QString::fromAscii_helper("Kickidler",9);
      QMessageBox::QMessageBox
                ((QMessageBox *)local_1a8,3,&local_res10,&local_res18,0x400,0,CONCAT44(uVar5,0x103))
      ;
      QString::~QString((QString *)&local_res10);
      QString::~QString((QString *)&local_res18);
      QWidget::show((QWidget *)local_1a8);
      iVar4 = QApplication::exec();
      QMessageBox::~QMessageBox((QMessageBox *)local_1a8);
      break;
    case 4:
      local_res18 = QString::fromAscii_helper("System does not support OpenGL",0x1e);
      local_res10 = QString::fromAscii_helper("Kickidler",9);
      QMessageBox::QMessageBox
                ((QMessageBox *)local_1a8,3,&local_res10,&local_res18,0x400,0,CONCAT44(uVar5,0x103))
      ;
      QString::~QString((QString *)&local_res10);
      QString::~QString((QString *)&local_res18);
      QWidget::show((QWidget *)local_1a8);
      iVar4 = QApplication::exec();
      QMessageBox::~QMessageBox((QMessageBox *)local_1a8);
      break;
    case 5:
      local_res18 = QString::fromAscii_helper("OpenGL version lower than 2.1",0x1d);
      local_res10 = QString::fromAscii_helper("Kickidler",9);
      QMessageBox::QMessageBox
                ((QMessageBox *)local_1a8,3,&local_res10,&local_res18,0x400,0,CONCAT44(uVar5,0x103))
      ;
      QString::~QString((QString *)&local_res10);
      QString::~QString((QString *)&local_res18);
      QWidget::show((QWidget *)local_1a8);
      iVar4 = QApplication::exec();
      QMessageBox::~QMessageBox((QMessageBox *)local_1a8);
      break;
    default:
      qt_assert("!\"bad application initialization result\"",
                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\main.cpp",0xdb);
      iVar4 = -9;
    }
    thunk_FUN_1400c46f0(local_158);
    return iVar4;
  }
  if (local_res8[0] < 5) {
    pcVar10 = "";
  }
  else {
    pcVar10 = *(char **)(param_2 + 0x20);
    if (pcVar10 == (char *)0x0) {
      lVar11 = 0xffffffff;
      goto LAB_140108f59;
    }
  }
  lVar11 = -1;
  do {
    lVar11 = lVar11 + 1;
  } while (pcVar10[lVar11] != '\0');
LAB_140108f59:
  local_res20 = QString::fromAscii_helper(pcVar10,(int)lVar11);
  pcVar10 = *(char **)(param_2 + 0x18);
  if (pcVar10 == (char *)0x0) {
    lVar11 = 0xffffffff;
  }
  else {
    lVar11 = -1;
    do {
      lVar11 = lVar11 + 1;
    } while (pcVar10[lVar11] != '\0');
  }
  local_res18 = QString::fromAscii_helper(pcVar10,(int)lVar11);
  pcVar10 = *(char **)(param_2 + 0x10);
  if (pcVar10 != (char *)0x0) {
    do {
      lVar12 = lVar12 + 1;
    } while (pcVar10[lVar12] != '\0');
  }
  local_res10 = QString::fromAscii_helper(pcVar10,(int)lVar12);
  uVar5 = thunk_FUN_140180d90(&local_res10);
  thunk_FUN_14017cc50(local_1a8,2,uVar5,&local_res18,&local_res20,local_res8,param_2);
  QString::~QString((QString *)&local_res10);
  QString::~QString((QString *)&local_res18);
  QString::~QString((QString *)&local_res20);
  iVar4 = QCoreApplication::exec();
  thunk_FUN_14017d850(local_1a8);
  return iVar4;
}

// =============================================================================
// Ghidra: FUN_140107d50 @ 0x140107d50
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140107d50(int param_1,undefined8 param_2,QString *param_3)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  QByteArray *pQVar4;
  char *pcVar5;
  undefined8 uVar6;
  ulonglong uVar7;
  int *piVar8;
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
  local_10 = DAT_140e88880 ^ (ulonglong)auStack_168;
  local_138[0] = 0;
  piVar8 = (int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
                  0x110);
  if ((*piVar8 < DAT_140e9f0cc) && (_Init_thread_header(&DAT_140e9f0cc), DAT_140e9f0cc == -1)) {
    _DAT_140e9f0c0 =
         QString::fromAscii_helper("qUncompress: Z_DATA_ERROR: Input data is corrupted",0x32);
    atexit(FUN_140593570);
    _Init_thread_footer(&DAT_140e9f0cc);
  }
  if ((*piVar8 < DAT_140e9f0dc) && (_Init_thread_header(&DAT_140e9f0dc), DAT_140e9f0dc == -1)) {
    _DAT_140e9f0d0 = QString::fromAscii_helper("qUncompress: Input data is corrupted",0x24);
    atexit(FUN_140593590);
    _Init_thread_footer(&DAT_140e9f0dc);
  }
  bVar1 = operator==(param_3,(QString *)&DAT_140e9f0c0);
  if ((!bVar1) && (bVar1 = operator==(param_3,(QString *)&DAT_140e9f0d0), !bVar1)) {
    if (param_1 == 0) {
      uVar3 = thunk_FUN_14039be80(local_138);
      cVar2 = thunk_FUN_14039ba40(uVar3,10000);
      thunk_FUN_14039a8a0(local_138);
      if (cVar2 == '\0') {
        return;
      }
      local_128 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_138[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_140076630(local_120,2);
      uVar3 = thunk_FUN_14006fb70(&local_128,"qt say: [");
      pQVar4 = (QByteArray *)QString::toLocal8Bit(param_3);
      pcVar5 = QByteArray::data(pQVar4);
      thunk_FUN_14006fb70(uVar3,pcVar5);
      QByteArray::~QByteArray((QByteArray *)local_138);
      thunk_FUN_14006fb70(uVar3,&DAT_1405a2258);
      uVar3 = thunk_FUN_1400876c0(&local_128,local_30);
      uVar6 = thunk_FUN_14039be80(local_138);
      local_148 = 0x44;
      thunk_FUN_14039b070(uVar6,10000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\main.cpp");
      thunk_FUN_14039a8a0(local_138);
      if (0xf < local_18) {
        uVar7 = local_30[0];
        if (0xfff < local_18 + 1) {
          if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_30[0] - 8);
          if (local_30[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_30[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_30[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar7);
      }
    }
    else if (param_1 == 1) {
      uVar3 = thunk_FUN_14039be80(local_138);
      cVar2 = thunk_FUN_14039ba40(uVar3,30000);
      thunk_FUN_14039a8a0(local_138);
      if (cVar2 == '\0') {
        return;
      }
      local_128 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_138[0] = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_140076630(local_120,2);
      uVar3 = thunk_FUN_14006fb70(&local_128,"qt say: [");
      pQVar4 = (QByteArray *)QString::toLocal8Bit(param_3);
      pcVar5 = QByteArray::data(pQVar4);
      thunk_FUN_14006fb70(uVar3,pcVar5);
      QByteArray::~QByteArray((QByteArray *)local_138);
      thunk_FUN_14006fb70(uVar3,&DAT_1405a2258);
      uVar3 = thunk_FUN_1400876c0(&local_128,local_30);
      uVar6 = thunk_FUN_14039be80(local_138);
      local_148 = 0x47;
      thunk_FUN_14039b070(uVar6,30000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\main.cpp");
      thunk_FUN_14039a8a0(local_138);
      if (0xf < local_18) {
        uVar7 = local_30[0];
        if (0xfff < local_18 + 1) {
          if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_30[0] - 8);
          if (local_30[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_30[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_30[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar7);
      }
    }
    else if (param_1 == 2) {
      uVar3 = thunk_FUN_14039be80(local_138);
      cVar2 = thunk_FUN_14039ba40(uVar3,40000);
      thunk_FUN_14039a8a0(local_138);
      if (cVar2 == '\0') {
        return;
      }
      local_128 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a0);
      local_138[0] = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
      *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
           *(int *)(local_128 + 4) + -0x88;
      thunk_FUN_140076630(local_120,2);
      uVar3 = thunk_FUN_14006fb70(&local_128,"qt say: [");
      pQVar4 = (QByteArray *)QString::toLocal8Bit(param_3);
      pcVar5 = QByteArray::data(pQVar4);
      thunk_FUN_14006fb70(uVar3,pcVar5);
      QByteArray::~QByteArray((QByteArray *)local_138);
      thunk_FUN_14006fb70(uVar3,&DAT_1405a2258);
      uVar3 = thunk_FUN_1400876c0(&local_128,local_30);
      uVar6 = thunk_FUN_14039be80(local_138);
      local_148 = 0x4a;
      thunk_FUN_14039b070(uVar6,40000,uVar3,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\main.cpp");
      thunk_FUN_14039a8a0(local_138);
      if (0xf < local_18) {
        uVar7 = local_30[0];
        if (0xfff < local_18 + 1) {
          if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_30[0] - 8);
          if (local_30[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_30[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_30[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar7);
      }
    }
    else {
      uVar3 = thunk_FUN_14039be80(local_138);
      if (param_1 == 3) {
        cVar2 = thunk_FUN_14039ba40(uVar3,50000);
        thunk_FUN_14039a8a0(local_138);
        if (cVar2 == '\0') {
          return;
        }
        local_128 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_a0);
        local_138[0] = 8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
        *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
             *(int *)(local_128 + 4) + -0x88;
        thunk_FUN_140076630(local_120,2);
        uVar3 = thunk_FUN_14006fb70(&local_128,"qt say: [");
        pQVar4 = (QByteArray *)QString::toLocal8Bit(param_3);
        pcVar5 = QByteArray::data(pQVar4);
        thunk_FUN_14006fb70(uVar3,pcVar5);
        QByteArray::~QByteArray((QByteArray *)local_138);
        thunk_FUN_14006fb70(uVar3,&DAT_1405a2258);
        uVar3 = thunk_FUN_1400876c0(&local_128,local_30);
        uVar6 = thunk_FUN_14039be80(local_138);
        local_148 = 0x4d;
        thunk_FUN_14039b070(uVar6,50000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\main.cpp");
        thunk_FUN_14039a8a0(local_138);
        if (0xf < local_18) {
          uVar7 = local_30[0];
          if (0xfff < local_18 + 1) {
            if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_30[0] - 8);
            if (local_30[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_30[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_30[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar7);
        }
      }
      else {
        cVar2 = thunk_FUN_14039ba40(uVar3,20000);
        thunk_FUN_14039a8a0(local_138);
        if (cVar2 == '\0') {
          return;
        }
        local_128 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_a0);
        local_138[0] = 0x10;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_128,local_120,false);
        *(undefined ***)(local_120 + (longlong)*(int *)(local_128 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_12c + (longlong)*(int *)(local_128 + 4)) =
             *(int *)(local_128 + 4) + -0x88;
        thunk_FUN_140076630(local_120,2);
        uVar3 = thunk_FUN_14006fb70(&local_128,"qt say: [");
        pQVar4 = (QByteArray *)QString::toLocal8Bit(param_3);
        pcVar5 = QByteArray::data(pQVar4);
        thunk_FUN_14006fb70(uVar3,pcVar5);
        QByteArray::~QByteArray((QByteArray *)local_138);
        thunk_FUN_14006fb70(uVar3,&DAT_1405a2258);
        uVar3 = thunk_FUN_1400876c0(&local_128,local_30);
        uVar6 = thunk_FUN_14039be80(local_138);
        local_148 = 0x50;
        thunk_FUN_14039b070(uVar6,20000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\main.cpp");
        thunk_FUN_14039a8a0(local_138);
        if (0xf < local_18) {
          uVar7 = local_30[0];
          if (0xfff < local_18 + 1) {
            if ((local_30[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_30[0] - 8);
            if (local_30[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_30[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_30[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar7);
        }
      }
    }
    thunk_FUN_140077af0(local_a0);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_a0)
    ;
  }
  return;
}

// =============================================================================
// Ghidra: FUN_1400beee0 @ 0x1400beee0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QCoreApplication * FUN_1400beee0(QCoreApplication *param_1)

{
  QTimer *this;
  int iVar1;
  char cVar2;
  bool bVar3;
  QTypedArrayData<unsigned_short> *pQVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  QStringList *pQVar7;
  QTimeZone *pQVar8;
  QByteArray *this_00;
  char *pcVar9;
  ulonglong uVar10;
  QCoreApplication *pQVar11;
  QString *pQVar12;
  undefined1 auStack_348 [32];
  undefined4 *local_328;
  undefined4 local_320;
  uint local_318;
  Data *local_310;
  QTypedArrayData<unsigned_short> *local_308;
  undefined ***local_300;
  undefined ***local_2f8 [2];
  code *local_2e8;
  undefined4 uStack_2e0;
  int iStack_2dc;
  QByteArray local_2d8 [8];
  QByteArray local_2d0 [8];
  undefined8 local_2c8;
  QCoreApplication *local_2c0;
  undefined1 local_2b8 [16];
  undefined1 local_2a8 [16];
  undefined1 local_298 [16];
  undefined1 local_288 [16];
  undefined4 local_278;
  undefined4 uStack_274;
  undefined4 uStack_270;
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
  
  local_2c8 = 0xfffffffffffffffe;
  local_48 = DAT_140e88880 ^ (ulonglong)auStack_348;
  local_318 = 0;
  local_2c0 = param_1;
  thunk_FUN_1403d4050();
  QAbstractNativeEventFilter::QAbstractNativeEventFilter
            ((QAbstractNativeEventFilter *)(param_1 + 0x20));
  thunk_FUN_1401573f0(param_1 + 0x30);
  *(undefined ***)param_1 = viewer::Application::vftable;
  *(undefined ***)(param_1 + 0x20) = viewer::Application::vftable;
  *(undefined ***)(param_1 + 0x30) = viewer::Application::vftable;
  *(undefined2 *)(param_1 + 0xa8) = 0x100;
  *(code **)(param_1 + 0xb0) = shared_null_exref;
  *(undefined8 *)(param_1 + 0xb8) = 0;
  *(undefined8 *)(param_1 + 0xc0) = 0;
  *(undefined8 *)(param_1 + 200) = 0;
  QTimeZone::QTimeZone((QTimeZone *)(param_1 + 0xd0));
  this = (QTimer *)(param_1 + 0xd8);
  QTimer::QTimer(this,(QObject *)0x0);
  param_1[0xf8] = (QCoreApplication)0x0;
  *(undefined8 *)(param_1 + 0x100) = 0;
  *(undefined8 *)(param_1 + 0x108) = 0;
  QIcon::QIcon((QIcon *)(param_1 + 0x110));
  pQVar4 = QString::fromAscii_helper("qrc:/qml/application/Entry.qml",0x1e);
  *(QTypedArrayData<unsigned_short> **)(param_1 + 0x118) = pQVar4;
  uVar5 = thunk_FUN_14039be80(local_2b8);
  cVar2 = thunk_FUN_14039ba40(uVar5,20000);
  thunk_FUN_14039a8a0(local_2b8);
  if (cVar2 != '\0') {
    local_268 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1e0)
    ;
    local_318 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_2f8[0] = &local_260;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1f8 = 0;
    local_1f0 = 4;
    thunk_FUN_14006fb70(&local_268,"application startup");
    uVar5 = thunk_FUN_1400876c0(&local_268,local_88);
    uVar6 = thunk_FUN_14039be80(local_2a8);
    local_328 = (undefined4 *)CONCAT44(local_328._4_4_,0xc0);
    thunk_FUN_14039b070(uVar6,20000,uVar5,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp");
    thunk_FUN_14039a8a0(local_2a8);
    if (0xf < local_70) {
      uVar10 = local_88[0];
      if (0xfff < local_70 + 1) {
        if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_88[0] - 8);
        if (local_88[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_88[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_88[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar10);
    }
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_260);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_258);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_1e0);
  }
  local_308 = QString::fromAscii_helper("--no-splash",0xb);
  pQVar7 = (QStringList *)QCoreApplication::arguments();
  bVar3 = QtPrivate::QStringList_contains(pQVar7,(QString *)&local_308,1);
  param_1[0xa9] = (QCoreApplication)!bVar3;
  if (*(int *)local_310 != 0) {
    if (*(int *)local_310 == -1) goto LAB_1400bf243;
    LOCK();
    iVar1 = *(int *)local_310;
    *(int *)local_310 = *(int *)local_310 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_1400bf243;
  }
  pQVar12 = (QString *)(local_310 + ((longlong)*(int *)(local_310 + 0xc) + 2) * 8);
  iVar1 = *(int *)(local_310 + 8);
  while ((QString *)(local_310 + ((longlong)iVar1 + 2) * 8) != pQVar12) {
    pQVar12 = pQVar12 + -8;
    QString::~QString(pQVar12);
  }
  QListData::dispose(local_310);
LAB_1400bf243:
  QString::~QString((QString *)&local_308);
  QApplication::desktop();
  pQVar11 = param_1 + 0x20;
  if (param_1 == (QCoreApplication *)0x0) {
    pQVar11 = (QCoreApplication *)(QAbstractNativeEventFilter *)0x0;
  }
  QCoreApplication::installNativeEventFilter(param_1,(QAbstractNativeEventFilter *)pQVar11);
  pQVar8 = (QTimeZone *)QTimeZone::systemTimeZone();
  QTimeZone::operator=((QTimeZone *)(param_1 + 0xd0),pQVar8);
  QTimeZone::~QTimeZone((QTimeZone *)&local_300);
  uVar5 = thunk_FUN_14039be80(local_298);
  cVar2 = thunk_FUN_14039ba40(uVar5,20000);
  thunk_FUN_14039a8a0(local_298);
  if (cVar2 != '\0') {
    local_178 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_f0);
    local_318 = local_318 | 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    local_300 = &local_170;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
    local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_108 = 0;
    local_100 = 4;
    QTimeZone::id((QTimeZone *)(param_1 + 0xd0));
    pQVar12 = (QString *)QString::fromLatin1(local_2d0);
    uVar5 = thunk_FUN_14006fb70(&local_178,"timezone: ");
    this_00 = (QByteArray *)QString::toLocal8Bit(pQVar12);
    pcVar9 = QByteArray::data(this_00);
    thunk_FUN_14006fb70(uVar5,pcVar9);
    QByteArray::~QByteArray(local_2d8);
    QString::~QString((QString *)local_2d0);
    QByteArray::~QByteArray((QByteArray *)&local_2e8);
    uVar5 = thunk_FUN_1400876c0(&local_178,local_68);
    uVar6 = thunk_FUN_14039be80(local_288);
    local_328 = (undefined4 *)CONCAT44(local_328._4_4_,0xca);
    thunk_FUN_14039b070(uVar6,20000,uVar5,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp");
    thunk_FUN_14039a8a0(local_288);
    if (0xf < local_50) {
      uVar10 = local_68[0];
      if (0xfff < local_50 + 1) {
        if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_68[0] - 8);
        if (local_68[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_68[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_68[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar10);
    }
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_170);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_168);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_f0)
    ;
  }
  QTimer::setInterval(this,200);
  *(uint *)(param_1 + 0xf4) = *(uint *)(param_1 + 0xf4) & 0xfffffffe;
  local_2e8 = thunk_FUN_1400d5b90;
  uStack_2e0 = 0;
  local_278 = 0x40008b25;
  uStack_274 = 1;
  uStack_270 = 0;
  iStack_26c = iStack_2dc;
  local_320 = 0;
  local_328 = &local_278;
  thunk_FUN_14008e210(local_2f8,this,timeout_exref,param_1);
  QMetaObject::Connection::~Connection((Connection *)local_2f8);
  QTimer::start(this);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_1400e6a10 @ 0x1400e6a10
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1400e6a10(QObject *param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  char cVar3;
  char cVar4;
  QObject QVar5;
  int iVar6;
  char *pcVar7;
  QObject *pQVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  QString *pQVar12;
  code *pcVar13;
  longlong *plVar14;
  QWindow *this;
  QQuickView *pQVar15;
  QQmlEngine *pQVar16;
  QQmlContext *pQVar17;
  QObject *pQVar18;
  longlong *plVar19;
  QVariant *this_00;
  undefined8 uVar20;
  longlong lVar21;
  uint uVar22;
  undefined1 auStackY_218 [32];
  undefined8 local_1e8;
  code *local_1d8;
  undefined8 uStack_1d0;
  undefined4 local_1c8;
  undefined4 uStack_1c4;
  undefined4 uStack_1c0;
  uint uStack_1bc;
  undefined *local_1b8;
  basic_streambuf<char,std::char_traits<char>_> local_1b0 [128];
  basic_ios<char,std::char_traits<char>_> local_130 [104];
  undefined8 local_c8;
  undefined1 local_b8 [96];
  QObject *local_58;
  undefined4 uStack_50;
  ulonglong local_40;
  ulonglong local_38;
  
  local_c8 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStackY_218;
  puVar10 = (undefined8 *)0x0;
  uVar22 = 0;
  local_1e8 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_1e8 & 0xffffffff00000000);
  pcVar7 = qFlagLocation("1handleMessage(const QString&)");
  pQVar8 = (QObject *)qFlagLocation("2messageReceived(const QString&)");
  QObject::connect((QObject *)&local_1e8,(char *)param_1,pQVar8,(char *)param_1,
                   (ConnectionType)pcVar7);
  QMetaObject::Connection::~Connection((Connection *)&local_1e8);
  thunk_FUN_1400d3be0(param_1);
  cVar3 = thunk_FUN_1400e2740(param_1);
  cVar4 = thunk_FUN_1403d4270(param_1);
  if (cVar4 == '\0') {
    local_1e8 = QString::fromAscii_helper("viewer",6);
    iVar6 = thunk_FUN_140157800(param_1 + 0x30,&local_1e8);
    QString::~QString((QString *)&local_1e8);
    if (iVar6 == 1) {
      uVar9 = 4;
    }
    else if (iVar6 == 2) {
      uVar9 = 5;
    }
    else if (iVar6 - 3U < 2) {
      uVar9 = 2;
    }
    else {
      local_1d8 = operator_new(0x90);
      if (local_1d8 != (code *)0x0) {
        puVar10 = (undefined8 *)thunk_FUN_140104440(local_1d8);
      }
      puVar1 = *(undefined8 **)(param_1 + 0x100);
      if ((puVar1 != puVar10) &&
         (*(undefined8 **)(param_1 + 0x100) = puVar10, puVar1 != (undefined8 *)0x0)) {
        (**(code **)*puVar1)(puVar1,1);
      }
      uVar9 = thunk_FUN_1400c9e20(param_1 + 0x100);
      cVar3 = thunk_FUN_14015f980(uVar9);
      if (cVar3 == '\0') {
        uVar9 = thunk_FUN_14039be80(&local_1d8);
        cVar3 = thunk_FUN_14039ba40(uVar9,50000);
        thunk_FUN_14039a8a0(&local_1d8);
        if (cVar3 != '\0') {
          local_1b8 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_130);
          local_1e8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1e8._4_4_,4);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,local_1b0,false);
          *(undefined ***)(local_1b0 + (longlong)*(int *)(local_1b8 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&uStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
               *(int *)(local_1b8 + 4) + -0x88;
          thunk_FUN_140076630(local_1b0,2);
          thunk_FUN_14006fb70(&local_1b8,"unable to open config database");
          uVar9 = thunk_FUN_1400876c0(&local_1b8,&local_58);
          uVar11 = thunk_FUN_14039be80(&local_1d8);
          thunk_FUN_14039b070(uVar11,50000,uVar9,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                             );
          thunk_FUN_14039a8a0(&local_1d8);
          if (0xf < local_40) {
            pQVar8 = local_58;
            if (0xfff < local_40 + 1) {
              if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              pQVar8 = *(QObject **)(local_58 + -8);
              if (local_58 <= pQVar8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if ((ulonglong)((longlong)local_58 - (longlong)pQVar8) < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pQVar8)) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(pQVar8);
          }
          thunk_FUN_140077af0(local_130);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_130);
        }
        uVar9 = 2;
      }
      else {
        thunk_FUN_1401637d0(*(undefined8 *)(param_1 + 0x100));
        cVar3 = FUN_1400f9c90();
        if (cVar3 != '\0') {
          FUN_1400f38e0();
        }
        uVar9 = thunk_FUN_14039be80(&local_1d8);
        cVar3 = thunk_FUN_14039ba40(uVar9,20000);
        thunk_FUN_14039a8a0(&local_1d8);
        if (cVar3 != '\0') {
          local_1b8 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_130);
          uVar22 = 8;
          local_1e8 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_1e8._4_4_,8);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,local_1b0,false);
          *(undefined ***)(local_1b0 + (longlong)*(int *)(local_1b8 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&uStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
               *(int *)(local_1b8 + 4) + -0x88;
          thunk_FUN_140076630(local_1b0,2);
          thunk_FUN_14006fb70(&local_1b8,"application initialize");
          uVar9 = thunk_FUN_1400876c0(&local_1b8,&local_58);
          uVar11 = thunk_FUN_14039be80(&local_1d8);
          thunk_FUN_14039b070(uVar11,20000,uVar9,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                             );
          thunk_FUN_14039a8a0(&local_1d8);
          if (0xf < local_40) {
            pQVar8 = local_58;
            if (0xfff < local_40 + 1) {
              if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              pQVar8 = *(QObject **)(local_58 + -8);
              if (local_58 <= pQVar8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if ((ulonglong)((longlong)local_58 - (longlong)pQVar8) < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pQVar8)) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(pQVar8);
          }
          thunk_FUN_140077af0(local_130);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_130);
        }
        pQVar12 = (QString *)QCoreApplication::applicationDirPath();
        QDir::setCurrent(pQVar12);
        QString::~QString((QString *)&local_1e8);
        thunk_FUN_140083800();
        uVar9 = thunk_FUN_1400ec460();
        uVar11 = thunk_FUN_140083800();
        thunk_FUN_140150600(uVar9,uVar11);
        pcVar13 = operator_new(0x1b8);
        uVar9 = 0;
        local_1d8 = pcVar13;
        if (pcVar13 != (code *)0x0) {
          uVar9 = thunk_FUN_140083800();
          uVar9 = thunk_FUN_1401e08a0(pcVar13,uVar9);
        }
        *(undefined8 *)(param_1 + 200) = uVar9;
        lVar2 = *(longlong *)(param_1 + 0x100);
        uVar9 = thunk_FUN_1400ec1e0();
        uVar11 = thunk_FUN_140083800();
        lVar21 = lVar2 + 0x68;
        if (lVar2 == 0) {
          lVar21 = 0;
        }
        thunk_FUN_14016f8e0(uVar9,lVar21,uVar11);
        local_1d8 = thunk_FUN_140172d20;
        uStack_1d0._0_4_ = 0;
        uVar9 = thunk_FUN_1400ec1e0();
        thunk_FUN_14008e1c0(&local_1e8,uVar9,&local_1d8,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_1e8);
        local_1d8 = thunk_FUN_140172d40;
        uStack_1d0 = (char *)((ulonglong)uStack_1d0._4_4_ << 0x20);
        uVar9 = thunk_FUN_1400ec1e0();
        thunk_FUN_14008e1c0(&local_1e8,uVar9,&local_1d8,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_1e8);
        uVar9 = thunk_FUN_1400ec1e0();
        QVar5 = (QObject)thunk_FUN_14016fa60(uVar9);
        if (param_1[0xf8] != QVar5) {
          param_1[0xf8] = QVar5;
          thunk_FUN_140082990(param_1);
        }
        plVar14 = (longlong *)(**(code **)(*(longlong *)(param_1 + 0x30) + 8))();
        pQVar8 = param_1 + 0x108;
        plVar19 = *(longlong **)pQVar8;
        if ((plVar19 != plVar14) && (*(longlong **)pQVar8 = plVar14, plVar19 != (longlong *)0x0)) {
          (**(code **)(*plVar19 + 0x18))(plVar19,1);
        }
        this = (QWindow *)thunk_FUN_1400c9e70(pQVar8);
        QWindow::setIcon(this,(QIcon *)(param_1 + 0x110));
        local_1e8 = QString::fromAscii_helper(":/qml/application",0x11);
        pQVar15 = (QQuickView *)thunk_FUN_1400c9e70(pQVar8);
        pQVar16 = QQuickView::engine(pQVar15);
        QQmlEngine::addImportPath(pQVar16,(QString *)&local_1e8);
        QString::~QString((QString *)&local_1e8);
        local_1e8 = QString::fromAscii_helper(":/qml/desktop",0xd);
        pQVar15 = (QQuickView *)thunk_FUN_1400c9e70(pQVar8);
        pQVar16 = QQuickView::engine(pQVar15);
        QQmlEngine::addImportPath(pQVar16,(QString *)&local_1e8);
        QString::~QString((QString *)&local_1e8);
        local_1d8._0_4_ = 0x29;
        uStack_1d0 = "Do not create objects of type Application";
        QString::QString((QString *)&local_1e8,&local_1d8);
        thunk_FUN_1400b8550("Viewer",0,1,"Application");
        QString::~QString((QString *)&local_1e8);
        thunk_FUN_1400ac590("Utils",0,1,"DirectoryInformer");
        thunk_FUN_1400b6750("Utils",0,1,"ValueAmortisseur");
        thunk_FUN_1400ab390("Online",0,1,"Client");
        thunk_FUN_1400b2570("Online",0,1,&DAT_1405b4a7c);
        thunk_FUN_1400ae990("Online",0,1,"Grabber");
        thunk_FUN_1400b4950("Online",0,1,"Session");
        thunk_FUN_1400b1f70("Online",0,1,"Monitor");
        thunk_FUN_1400ab990("Config",2,0,"Config");
        thunk_FUN_1400b1370("Windows",2,0,"Manager");
        thunk_FUN_1400b7f50("Windows",2,0,"Window");
        thunk_FUN_1400b0d70("Windows",2,0,"KeyboardGrabber");
        thunk_FUN_1400b0790();
        thunk_FUN_1400b5b50("Windows",2,0,"Timer");
        thunk_FUN_1400aa790("History",0,1,"PlayerBlock");
        thunk_FUN_1400ae390("History",0,1,"PlayerGauge");
        thunk_FUN_1400b0190("History",0,1,"PlayerInfoFrame");
        thunk_FUN_1400b6d50("History",0,1,"PlayerVideoFrame");
        thunk_FUN_1400b7350("History",0,1,"PlayerVideoSaver");
        thunk_FUN_1400b3770();
        thunk_FUN_1400b3d50("Charts",0,1,"Selector");
        thunk_FUN_1400aef90("Violations",0,1,"Group");
        thunk_FUN_1400ad790("Violations",0,1,"Filter");
        thunk_FUN_1400ad190("Violations",0,1,"Event");
        thunk_FUN_1400b1970("Control",0,1,"Master");
        local_1d8._0_4_ = 0x25;
        uStack_1d0 = "Do not create objects of type Channel";
        QString::QString((QString *)&local_1e8,&local_1d8);
        thunk_FUN_1400b8b50("Control",0,1,"Channel");
        QString::~QString((QString *)&local_1e8);
        local_1d8 = (code *)CONCAT44(local_1d8._4_4_,0x2e);
        uStack_1d0._0_4_ = 0x405b4c10;
        uStack_1d0._4_4_ = 1;
        QString::QString((QString *)&local_1e8,&local_1d8);
        thunk_FUN_1400b9150("Control",0,1,"FilesTransporter");
        QString::~QString((QString *)&local_1e8);
        thunk_FUN_1400b7950("WebinterfaceSettings",0,1,"SettingsProvider");
        thunk_FUN_1400acb90("Organization",0,1,"Employee");
        thunk_FUN_1400abf90("Organization",0,1,"Department");
        thunk_FUN_1400add90("Organization",0,1,"FilteredDepartment");
        thunk_FUN_1400b4f50("Organization",0,1,"SessionMarker");
        thunk_FUN_1400afb90("Organization",0,1,"Index");
        thunk_FUN_1400af590("Applications",0,1,"Index");
        thunk_FUN_1400b2b70("Applications",0,1,"Rating");
        thunk_FUN_1400b5550("Applications",0,1,"Solver");
        thunk_FUN_1400aad90("Keylogger",0,1,"Chart");
        thunk_FUN_1400b3170("Keylogger",0,1,"Result");
        thunk_FUN_1400b4350("Keylogger",0,1,"Selector");
        thunk_FUN_1400b6150("Keylogger",0,1,"Translator");
        local_1e8 = QString::fromAscii_helper("app",3);
        pQVar15 = (QQuickView *)thunk_FUN_1400c9e70(pQVar8);
        pQVar16 = QQuickView::engine(pQVar15);
        pQVar17 = QQmlEngine::rootContext(pQVar16);
        QQmlContext::setContextProperty(pQVar17,(QString *)&local_1e8,param_1);
        QString::~QString((QString *)&local_1e8);
        local_1e8 = QString::fromAscii_helper("kickUrl",7);
        lVar21 = thunk_FUN_140083800();
        pQVar15 = (QQuickView *)thunk_FUN_1400c9e70(pQVar8);
        pQVar16 = QQuickView::engine(pQVar15);
        pQVar17 = QQmlEngine::rootContext(pQVar16);
        QQmlContext::setContextProperty(pQVar17,(QString *)&local_1e8,(QObject *)(lVar21 + 0x2b0));
        QString::~QString((QString *)&local_1e8);
        local_1e8 = QString::fromAscii_helper("filtersContainer",0x10);
        pQVar18 = (QObject *)thunk_FUN_1400ec320();
        pQVar15 = (QQuickView *)thunk_FUN_1400c9e70(pQVar8);
        pQVar16 = QQuickView::engine(pQVar15);
        pQVar17 = QQmlEngine::rootContext(pQVar16);
        QQmlContext::setContextProperty(pQVar17,(QString *)&local_1e8,pQVar18);
        QString::~QString((QString *)&local_1e8);
        local_1e8 = QString::fromAscii_helper("historyCacheIndex",0x11);
        pQVar18 = (QObject *)thunk_FUN_1400ec140();
        pQVar15 = (QQuickView *)thunk_FUN_1400c9e70(pQVar8);
        pQVar16 = QQuickView::engine(pQVar15);
        pQVar17 = QQmlEngine::rootContext(pQVar16);
        QQmlContext::setContextProperty(pQVar17,(QString *)&local_1e8,pQVar18);
        QString::~QString((QString *)&local_1e8);
        uVar11 = 0;
        local_1d8 = thunk_FUN_1400ebde0;
        uStack_1d0._0_4_ = 0;
        local_58 = (QObject *)thunk_FUN_140126ea0;
        uStack_50 = 0;
        uVar9 = thunk_FUN_140083800();
        thunk_FUN_14008e4e0(&local_1e8,uVar9,&local_58,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_1e8);
        local_1d8 = thunk_FUN_140082b60;
        uStack_1d0._0_4_ = 0;
        local_58 = (QObject *)thunk_FUN_140126f70;
        uStack_50 = 0;
        uVar9 = thunk_FUN_140083800();
        thunk_FUN_14008e4e0(&local_1e8,uVar9,&local_58,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_1e8);
        plVar19 = (longlong *)thunk_FUN_140083800();
        (**(code **)(*plVar19 + 0x58))(plVar19);
        thunk_FUN_140082b60(param_1);
        local_1d8 = thunk_FUN_1400eba20;
        uStack_1d0 = (char *)((ulonglong)uStack_1d0._4_4_ << 0x20);
        thunk_FUN_14008e3f0(&local_1e8,param_1 + 0x70,thunk_FUN_140156f20,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_1e8);
        thunk_FUN_140162050(local_b8,0);
        local_1e8 = QString::fromAscii_helper("preferredLang",0xd);
        thunk_FUN_140163ad0(local_b8,&local_1e8);
        QString::~QString((QString *)&local_1e8);
        this_00 = (QVariant *)thunk_FUN_140162d10(local_b8,&local_1d8);
        uVar9 = QVariant::toString(this_00);
        thunk_FUN_1401560d0(param_1 + 0x70,uVar9);
        QString::~QString((QString *)&local_1e8);
        QVariant::~QVariant((QVariant *)&local_1d8);
        thunk_FUN_1401621e0(local_b8);
        thunk_FUN_1400f3400(param_1);
        local_1d8 = operator_new(0x38);
        uVar9 = uVar11;
        if (local_1d8 != (code *)0x0) {
          uVar9 = thunk_FUN_1401b8e90(local_1d8);
        }
        *(undefined8 *)(param_1 + 0xc0) = uVar9;
        uVar9 = thunk_FUN_1400f4c80(&local_1c8,9);
        uVar9 = thunk_FUN_140079410(&local_1e8,uVar9,"/viewer");
        thunk_FUN_140083800();
        cVar3 = thunk_FUN_1401bb9b0(*(undefined8 *)(param_1 + 0xc0),uVar9);
        QString::~QString((QString *)&local_1e8);
        QString::~QString((QString *)&local_1c8);
        if (cVar3 == '\0') {
          uVar9 = thunk_FUN_14039be80(&local_1d8);
          cVar3 = thunk_FUN_14039ba40(uVar9,50000);
          thunk_FUN_14039a8a0(&local_1d8);
          if (cVar3 != '\0') {
            local_1b8 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_130);
            local_1e8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_1e8._4_4_,uVar22) | 0x10)
            ;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,local_1b0,false);
            *(undefined ***)(local_1b0 + (longlong)*(int *)(local_1b8 + 4) + -8) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&uStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
                 *(int *)(local_1b8 + 4) + -0x88;
            thunk_FUN_140076630(local_1b0,2);
            thunk_FUN_14006fb70(&local_1b8,"unable to setup aup collector");
            uVar9 = thunk_FUN_1400876c0(&local_1b8,&local_58);
            uVar20 = thunk_FUN_14039be80(&local_1d8);
            thunk_FUN_14039b070(uVar20,50000,uVar9,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                               );
            thunk_FUN_14039a8a0(&local_1d8);
            thunk_FUN_140077b90(&local_58);
            thunk_FUN_140077af0(local_130);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_130);
            uVar22 = uVar22 | 0x10;
          }
          QCoreApplication::exit(-0x23);
        }
        local_1d8 = operator_new(0x30);
        if (local_1d8 != (code *)0x0) {
          uVar11 = thunk_FUN_1401bc3c0(local_1d8,*(undefined8 *)(param_1 + 0xc0));
        }
        *(undefined8 *)(param_1 + 0xb8) = uVar11;
        uStack_1d0._4_4_ = (uint)((ulonglong)uStack_1d0 >> 0x20);
        local_1d8 = (code *)&LAB_14001d0ed;
        uStack_1d0._0_4_ = 0;
        thunk_FUN_14008dfe0(&local_1c8,uVar11,thunk_FUN_1401c1f40,param_1);
        QMetaObject::Connection::~Connection((Connection *)&local_1c8);
        thunk_FUN_140083800();
        thunk_FUN_140083800();
        uVar9 = QCoreApplication::applicationFilePath();
        uVar11 = thunk_FUN_140114d10(&local_58);
        uVar20 = thunk_FUN_140114d90(&local_1e8);
        uVar11 = thunk_FUN_1400793a0(&local_1c8,uVar20,uVar11);
        thunk_FUN_1401bc9e0(*(undefined8 *)(param_1 + 0xb8),uVar11,uVar9);
        QString::~QString((QString *)&local_1c8);
        QString::~QString((QString *)&local_1e8);
        QString::~QString((QString *)&local_58);
        QString::~QString((QString *)&local_1d8);
        thunk_FUN_1401bed40(*(undefined8 *)(param_1 + 0xb8));
        uStack_1bc = uStack_1d0._4_4_;
        local_1d8 = thunk_FUN_140126d80;
        uStack_1d0 = (char *)((ulonglong)uStack_1d0._4_4_ << 0x20);
        local_1c8 = 0x40012b25;
        uStack_1c4 = 1;
        uStack_1c0 = 0;
        local_58 = param_1;
        uVar9 = thunk_FUN_140083800();
        thunk_FUN_14008e5d0(&local_1d8,uVar9,&local_1c8,*(undefined8 *)(param_1 + 0xc0));
        QMetaObject::Connection::~Connection((Connection *)&local_1d8);
        FUN_1400ca330(&local_58);
        uVar9 = thunk_FUN_14039be80(&local_1d8);
        cVar3 = thunk_FUN_14039ba40(uVar9,20000);
        thunk_FUN_14039a8a0(&local_1d8);
        if (cVar3 != '\0') {
          local_1b8 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_130);
          local_1e8 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_1e8._4_4_,uVar22) | 0x20);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,local_1b0,false);
          *(undefined ***)(local_1b0 + (longlong)*(int *)(local_1b8 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&uStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
               *(int *)(local_1b8 + 4) + -0x88;
          thunk_FUN_140076630(local_1b0,2);
          thunk_FUN_14006fb70(&local_1b8,"application initialized");
          uVar9 = thunk_FUN_1400876c0(&local_1b8,&local_58);
          uVar11 = thunk_FUN_14039be80(&local_1d8);
          thunk_FUN_14039b070(uVar11,20000,uVar9,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                             );
          thunk_FUN_14039a8a0(&local_1d8);
          if (0xf < local_40) {
            pQVar8 = local_58;
            if (0xfff < local_40 + 1) {
              if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              pQVar8 = *(QObject **)(local_58 + -8);
              if (local_58 <= pQVar8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if ((ulonglong)((longlong)local_58 - (longlong)pQVar8) < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < (ulonglong)((longlong)local_58 - (longlong)pQVar8)) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(pQVar8);
          }
          thunk_FUN_140077af0(local_130);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_130);
        }
        uVar9 = 0;
      }
    }
  }
  else if (cVar3 == '\0') {
    local_1e8 = QString::fromAscii_helper("Activate window",0xf);
    cVar3 = thunk_FUN_1403d4280(param_1,&local_1e8,5000);
    QString::~QString((QString *)&local_1e8);
    uVar9 = 1;
    if (cVar3 == '\0') {
      uVar9 = 3;
    }
  }
  else {
    uVar9 = 1;
  }
  return uVar9;
}

// =============================================================================
// Ghidra: FUN_1400f38e0 @ 0x1400f38e0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400f38e0(void)

{
  QString *pQVar1;
  int iVar2;
  longlong lVar3;
  Data *pDVar4;
  QMapDataBase *this;
  char cVar5;
  bool bVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  QString *pQVar9;
  QFileInfo *pQVar10;
  QFileInfo *pQVar11;
  QByteArray *this_00;
  char *pcVar12;
  ulonglong uVar13;
  undefined1 auStack_4f8 [32];
  undefined4 local_4d8;
  uint local_4c8;
  Data *local_4c0;
  Data *local_4b8;
  QMapDataBase *local_4b0;
  Data *local_4a8;
  QTypedArrayData<unsigned_short> *local_4a0;
  QTypedArrayData<unsigned_short> *local_498;
  QTypedArrayData<unsigned_short> *local_490;
  QTypedArrayData<unsigned_short> *local_488;
  QDir local_480 [8];
  undefined ***local_478;
  undefined4 local_470;
  undefined4 local_46c;
  QString local_468 [8];
  QString local_460 [8];
  QByteArray local_458 [8];
  undefined8 local_450;
  undefined ***local_448;
  undefined1 local_440 [16];
  undefined1 local_430 [16];
  undefined1 local_420 [16];
  undefined1 local_410 [16];
  QVariant local_400 [16];
  undefined1 local_3f0 [16];
  undefined1 local_3e0 [20];
  int iStack_3cc;
  undefined *local_3c8;
  undefined **local_3c0;
  basic_ostream<char,std::char_traits<char>_> local_3b8 [96];
  undefined8 local_358;
  undefined4 local_350;
  basic_ios<char,std::char_traits<char>_> local_340 [100];
  int iStack_2dc;
  undefined *local_2d8;
  undefined **local_2d0;
  basic_ostream<char,std::char_traits<char>_> local_2c8 [96];
  undefined8 local_268;
  undefined4 local_260;
  basic_ios<char,std::char_traits<char>_> local_250 [100];
  int iStack_1ec;
  undefined *local_1e8;
  undefined **local_1e0;
  basic_ostream<char,std::char_traits<char>_> local_1d8 [96];
  undefined8 local_178;
  undefined4 local_170;
  basic_ios<char,std::char_traits<char>_> local_160 [104];
  undefined1 local_f8 [96];
  ulonglong local_98 [3];
  ulonglong local_80;
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_450 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_4f8;
  local_4c8 = 0;
  uVar7 = thunk_FUN_14039be80(local_440);
  cVar5 = thunk_FUN_14039ba40(uVar7,20000);
  thunk_FUN_14039a8a0(local_440);
  if (cVar5 != '\0') {
    local_3c8 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_340)
    ;
    local_4c8 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_3c8,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_3c0,false);
    *(undefined ***)((longlong)&local_3c8 + (longlong)*(int *)(local_3c8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_3cc + (longlong)*(int *)(local_3c8 + 4)) =
         *(int *)(local_3c8 + 4) + -0x88;
    local_478 = &local_3c0;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_3c0);
    local_3c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_358 = 0;
    local_350 = 4;
    thunk_FUN_14006fb70(&local_3c8,"removing Viewer local cache by user request");
    uVar7 = thunk_FUN_1400876c0(&local_3c8,local_98);
    uVar8 = thunk_FUN_14039be80(local_430);
    local_4d8 = 0x8f;
    thunk_FUN_14039b070(uVar8,20000,uVar7,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp");
    thunk_FUN_14039a8a0(local_430);
    if (0xf < local_80) {
      uVar13 = local_98[0];
      if (0xfff < local_80 + 1) {
        if ((local_98[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar13 = *(ulonglong *)(local_98[0] - 8);
        if (local_98[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_98[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_98[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar13);
    }
    *(undefined ***)((longlong)&local_3c8 + (longlong)*(int *)(local_3c8 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_3cc + (longlong)*(int *)(local_3c8 + 4)) =
         *(int *)(local_3c8 + 4) + -0x88;
    local_3c0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_3c0);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_3c0);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_3b8);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_340);
  }
  pQVar9 = (QString *)thunk_FUN_1400f4c80(local_468,7);
  QDir::QDir(local_480,pQVar9);
  QString::~QString(local_468);
  local_490 = QString::fromAscii_helper("??????_*.mp3",0xc);
  local_498 = QString::fromAscii_helper("adb.??????.sqlite",0x11);
  local_4a0 = QString::fromAscii_helper("vhdb.??????.sqlite",0x12);
  local_4c0 = (Data *)shared_null_exref;
  thunk_FUN_14007bf20(&local_4c0,&local_4a0);
  thunk_FUN_14007bf20(&local_4c0,&local_498);
  thunk_FUN_14007bf20(&local_4c0,&local_490);
  thunk_FUN_1400755c0(&local_4a8,&local_4c0);
  pDVar4 = local_4c0;
  if (*(int *)local_4c0 == 0) {
LAB_1400f3bba:
    pQVar9 = (QString *)(local_4c0 + ((longlong)*(int *)(local_4c0 + 0xc) + 2) * 8);
    pQVar1 = (QString *)(local_4c0 + ((longlong)*(int *)(local_4c0 + 8) + 2) * 8);
    while (pQVar1 != pQVar9) {
      pQVar9 = pQVar9 + -8;
      QString::~QString(pQVar9);
    }
    QListData::dispose(pDVar4);
  }
  else if (*(int *)local_4c0 != -1) {
    LOCK();
    iVar2 = *(int *)local_4c0;
    *(int *)local_4c0 = *(int *)local_4c0 + -1;
    UNLOCK();
    if (iVar2 == 1) goto LAB_1400f3bba;
  }
  QString::~QString((QString *)&local_4a0);
  QString::~QString((QString *)&local_498);
  QString::~QString((QString *)&local_490);
  local_470 = 0xffffffff;
  local_46c = 2;
  local_4d8 = 0xffffffff;
  QDir::entryInfoList(local_480,&local_4b8,&local_4a8,2);
  pQVar10 = (QFileInfo *)QListData::begin((QListData *)&local_4b8);
  pQVar11 = (QFileInfo *)QListData::end((QListData *)&local_4b8);
  for (; pQVar10 != pQVar11; pQVar10 = pQVar10 + 8) {
    pQVar9 = (QString *)QFileInfo::absoluteFilePath(pQVar10);
    bVar6 = QFile::remove(pQVar9);
    QString::~QString(local_460);
    if (!bVar6) {
      uVar7 = thunk_FUN_14039be80(local_420);
      cVar5 = thunk_FUN_14039ba40(uVar7);
      thunk_FUN_14039a8a0(local_420);
      if (cVar5 != '\0') {
        local_2d8 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_250);
        local_4c8 = local_4c8 | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_2d8,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_2d0,false);
        *(undefined ***)((longlong)&local_2d8 + (longlong)*(int *)(local_2d8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_2dc + (longlong)*(int *)(local_2d8 + 4)) =
             *(int *)(local_2d8 + 4) + -0x88;
        local_4b0 = (QMapDataBase *)&local_2d0;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2d0);
        local_2d0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_268 = 0;
        local_260 = 4;
        pQVar9 = (QString *)QFileInfo::absoluteFilePath(pQVar10);
        uVar7 = thunk_FUN_14006fb70(&local_2d8,"unable to remove Viewer local cache file: ");
        this_00 = (QByteArray *)QString::toLocal8Bit(pQVar9);
        pcVar12 = QByteArray::data(this_00);
        thunk_FUN_14006fb70(uVar7,pcVar12);
        QByteArray::~QByteArray(local_458);
        QString::~QString((QString *)&local_478);
        uVar7 = thunk_FUN_1400876c0(&local_2d8,local_78);
        uVar8 = thunk_FUN_14039be80(local_410);
        local_4d8 = 0x9c;
        thunk_FUN_14039b070(uVar8,40000,uVar7,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                           );
        thunk_FUN_14039a8a0(local_410);
        if (0xf < local_60) {
          uVar13 = local_78[0];
          if (0xfff < local_60 + 1) {
            if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar13 = *(ulonglong *)(local_78[0] - 8);
            if (local_78[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_78[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_78[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar13);
        }
        *(undefined ***)((longlong)&local_2d8 + (longlong)*(int *)(local_2d8 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_2dc + (longlong)*(int *)(local_2d8 + 4)) =
             *(int *)(local_2d8 + 4) + -0x88;
        local_2d0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14007bb20(&local_2d0);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2d0);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_2c8);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_250);
      }
    }
  }
  thunk_FUN_140162050(local_f8,0);
  local_488 = QString::fromAscii_helper("historyCache",0xc);
  thunk_FUN_140163ad0(local_f8,&local_488);
  QString::~QString((QString *)&local_488);
  local_4b0 = (QMapDataBase *)shared_null_exref;
  thunk_FUN_140163870(local_f8,&local_4b0);
  this = local_4b0;
  if (*(int *)local_4b0 == 0) {
LAB_1400f3f6e:
    lVar3 = *(longlong *)(local_4b0 + 0x10);
    if (lVar3 != 0) {
      QString::~QString((QString *)(lVar3 + 0x18));
      QVariant::~QVariant((QVariant *)(lVar3 + 0x20));
      if (*(longlong *)(lVar3 + 8) != 0) {
        thunk_FUN_1400df120();
      }
      if (*(longlong *)(lVar3 + 0x10) != 0) {
        thunk_FUN_1400df120();
      }
      QMapDataBase::freeTree(this,*(QMapNodeBase **)(this + 0x10),8);
    }
    QMapDataBase::freeData(this);
  }
  else if (*(int *)local_4b0 != -1) {
    LOCK();
    iVar2 = *(int *)local_4b0;
    *(int *)local_4b0 = *(int *)local_4b0 + -1;
    UNLOCK();
    if (iVar2 == 1) goto LAB_1400f3f6e;
  }
  uVar7 = QVariant::QVariant(local_400);
  thunk_FUN_1401639e0(local_f8,uVar7);
  QVariant::~QVariant(local_400);
  cVar5 = thunk_FUN_140162900(local_f8);
  if (cVar5 == '\0') {
    uVar7 = thunk_FUN_14039be80(local_3f0);
    cVar5 = thunk_FUN_14039ba40(uVar7,40000);
    thunk_FUN_14039a8a0(local_3f0);
    if (cVar5 != '\0') {
      local_1e8 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_160);
      local_4c8 = local_4c8 | 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1e8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1e0,false);
      *(undefined ***)((longlong)&local_1e8 + (longlong)*(int *)(local_1e8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1ec + (longlong)*(int *)(local_1e8 + 4)) =
           *(int *)(local_1e8 + 4) + -0x88;
      local_448 = &local_1e0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1e0);
      local_1e0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_178 = 0;
      local_170 = 4;
      thunk_FUN_14006fb70(&local_1e8,
                          "unable to remove Viewer local cache index from settings database");
      uVar7 = thunk_FUN_1400876c0(&local_1e8,local_58);
      uVar8 = thunk_FUN_14039be80(local_3e0);
      local_4d8 = 0xa6;
      thunk_FUN_14039b070(uVar8,40000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                         );
      thunk_FUN_14039a8a0(local_3e0);
      if (0xf < local_40) {
        uVar13 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar13 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar13);
      }
      *(undefined ***)((longlong)&local_1e8 + (longlong)*(int *)(local_1e8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1ec + (longlong)*(int *)(local_1e8 + 4)) =
           *(int *)(local_1e8 + 4) + -0x88;
      local_1e0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_1e0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1e0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_1d8);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_160);
    }
  }
  thunk_FUN_1401621e0(local_f8);
  if (*(int *)local_4b8 == 0) {
LAB_1400f4211:
    pQVar10 = (QFileInfo *)(local_4b8 + ((longlong)*(int *)(local_4b8 + 0xc) + 2) * 8);
    iVar2 = *(int *)(local_4b8 + 8);
    while ((QFileInfo *)(local_4b8 + ((longlong)iVar2 + 2) * 8) != pQVar10) {
      pQVar10 = pQVar10 + -8;
      QFileInfo::~QFileInfo(pQVar10);
    }
    QListData::dispose(local_4b8);
  }
  else if (*(int *)local_4b8 != -1) {
    LOCK();
    iVar2 = *(int *)local_4b8;
    *(int *)local_4b8 = *(int *)local_4b8 + -1;
    UNLOCK();
    if (iVar2 == 1) goto LAB_1400f4211;
  }
  if (*(int *)local_4a8 != 0) {
    if (*(int *)local_4a8 == -1) goto LAB_1400f42ac;
    LOCK();
    iVar2 = *(int *)local_4a8;
    *(int *)local_4a8 = *(int *)local_4a8 + -1;
    UNLOCK();
    if (iVar2 != 1) goto LAB_1400f42ac;
  }
  pQVar9 = (QString *)(local_4a8 + ((longlong)*(int *)(local_4a8 + 0xc) + 2) * 8);
  iVar2 = *(int *)(local_4a8 + 8);
  while ((QString *)(local_4a8 + ((longlong)iVar2 + 2) * 8) != pQVar9) {
    pQVar9 = pQVar9 + -8;
    QString::~QString(pQVar9);
  }
  QListData::dispose(local_4a8);
LAB_1400f42ac:
  QDir::~QDir(local_480);
  return;
}

// =============================================================================
// Ghidra: FUN_1400f5410 @ 0x1400f5410
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400f5410(void)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  QVariant *this;
  QMapDataBase *pQVar5;
  ulonglong *puVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  basic_ios<char,std::char_traits<char>_> *this_00;
  QMapDataBase *pQVar10;
  QMapDataBase *pQVar11;
  undefined1 auStack_378 [32];
  undefined4 local_358;
  QMapDataBase *local_348;
  undefined4 local_340;
  QTypedArrayData<unsigned_short> *local_338;
  QTypedArrayData<unsigned_short> *local_330;
  undefined ***local_328;
  QVariant local_320 [16];
  QVariant local_310 [16];
  undefined8 local_300;
  QVariant local_2f8 [16];
  undefined1 local_2e8 [16];
  undefined1 local_2d8 [16];
  undefined1 local_2c8 [16];
  undefined1 local_2b8 [12];
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
  undefined1 local_c8 [96];
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_300 = 0xfffffffffffffffe;
  local_28 = DAT_140e88880 ^ (ulonglong)auStack_378;
  local_340 = 0;
  thunk_FUN_140162050(local_c8,0);
  local_330 = QString::fromAscii_helper("viewerMaintenance",0x11);
  thunk_FUN_140163ad0(local_c8,&local_330);
  QString::~QString((QString *)&local_330);
  this = (QVariant *)thunk_FUN_140162d10(local_c8,local_2f8);
  QVariant::toMap(this);
  QVariant::~QVariant(local_2f8);
  QVariant::QVariant(local_320,true);
  local_338 = QString::fromAscii_helper("removeLocalCacheOnStartup",0x19);
  if (1 < *(uint *)local_348) {
    pQVar5 = QMapDataBase::createData();
    if (*(longlong *)(local_348 + 0x10) != 0) {
      puVar6 = (ulonglong *)thunk_FUN_1400da660(*(longlong *)(local_348 + 0x10),pQVar5);
      *(ulonglong **)(pQVar5 + 0x10) = puVar6;
      *puVar6 = *puVar6 & 3;
      *puVar6 = *puVar6 | (ulonglong)(pQVar5 + 8);
    }
    if (*(uint *)local_348 == 0) {
LAB_1400f5546:
      thunk_FUN_1400deec0(local_348);
    }
    else if (*(uint *)local_348 != 0xffffffff) {
      LOCK();
      uVar1 = *(uint *)local_348;
      *(uint *)local_348 = *(uint *)local_348 - 1;
      UNLOCK();
      if (uVar1 == 1) goto LAB_1400f5546;
    }
    local_348 = pQVar5;
    QMapDataBase::recalcMostLeftNode(pQVar5);
  }
  pQVar5 = local_348 + 8;
  pQVar11 = (QMapDataBase *)0x0;
  bVar2 = true;
  pQVar10 = *(QMapDataBase **)(local_348 + 0x10);
  if (*(QMapDataBase **)(local_348 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400f55cc:
    local_358 = CONCAT31(local_358._1_3_,bVar2);
    thunk_FUN_1400daa20(local_348,&local_338,local_320,pQVar5);
  }
  else {
    do {
      pQVar5 = pQVar10;
      bVar2 = operator<((QString *)(pQVar5 + 0x18),(QString *)&local_338);
      if (bVar2) {
        pQVar10 = *(QMapDataBase **)(pQVar5 + 0x10);
      }
      else {
        pQVar10 = *(QMapDataBase **)(pQVar5 + 8);
        pQVar11 = pQVar5;
      }
      bVar2 = !bVar2;
    } while (pQVar10 != (QMapDataBase *)0x0);
    if ((pQVar11 == (QMapDataBase *)0x0) ||
       (bVar3 = operator<((QString *)&local_338,(QString *)(pQVar11 + 0x18)), bVar3))
    goto LAB_1400f55cc;
    QVariant::operator=((QVariant *)(pQVar11 + 0x20),local_320);
  }
  QString::~QString((QString *)&local_338);
  QVariant::~QVariant(local_320);
  QVariant::QVariant(local_310,(QMap<QString,QVariant> *)&local_348);
  thunk_FUN_1401639e0(local_c8,local_310);
  QVariant::~QVariant(local_310);
  cVar4 = thunk_FUN_140162900(local_c8);
  if (cVar4 == '\0') {
    uVar7 = thunk_FUN_14039be80(local_2e8);
    cVar4 = thunk_FUN_14039ba40(uVar7,40000);
    thunk_FUN_14039a8a0(local_2e8);
    if (cVar4 != '\0') {
      local_2a8 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_220);
      local_340 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_2a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_2a0,false);
      *(undefined ***)((longlong)&local_2a8 + (longlong)*(int *)(local_2a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
           *(int *)(local_2a8 + 4) + -0x88;
      local_328 = &local_2a0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_2a0);
      local_2a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_238 = 0;
      local_230 = 4;
      thunk_FUN_14006fb70(&local_2a8,"unable to schedule Viewer local cache removal");
      uVar7 = thunk_FUN_1400876c0(&local_2a8,local_68);
      uVar8 = thunk_FUN_14039be80(local_2d8);
      local_358 = 0x2c4;
      thunk_FUN_14039b070(uVar8,40000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                         );
      thunk_FUN_14039a8a0(local_2d8);
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
        FUN_140002d6f(uVar9);
      }
      *(undefined ***)((longlong)&local_2a8 + (longlong)*(int *)(local_2a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
           *(int *)(local_2a8 + 4) + -0x88;
      local_2a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_2a0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_2a0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_298);
      this_00 = local_220;
LAB_1400f59e6:
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (this_00);
    }
  }
  else {
    uVar7 = thunk_FUN_14039be80(local_2c8);
    cVar4 = thunk_FUN_14039ba40(uVar7,20000);
    thunk_FUN_14039a8a0(local_2c8);
    if (cVar4 != '\0') {
      local_1b8 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_130);
      local_340 = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1b0,false);
      *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
           *(int *)(local_1b8 + 4) + -0x88;
      local_328 = &local_1b0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
      local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_148 = 0;
      local_140 = 4;
      thunk_FUN_14006fb70(&local_1b8,"Viewer local cache removal scheduled by user request");
      uVar7 = thunk_FUN_1400876c0(&local_1b8,local_48);
      uVar8 = thunk_FUN_14039be80(local_2b8);
      local_358 = 0x2c8;
      thunk_FUN_14039b070(uVar8,20000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                         );
      thunk_FUN_14039a8a0(local_2b8);
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
        FUN_140002d6f(uVar9);
      }
      *(undefined ***)((longlong)&local_1b8 + (longlong)*(int *)(local_1b8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
           *(int *)(local_1b8 + 4) + -0x88;
      local_1b0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_1b0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1b0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_1a8);
      this_00 = local_130;
      goto LAB_1400f59e6;
    }
  }
  if (*(uint *)local_348 != 0) {
    if (*(uint *)local_348 == 0xffffffff) goto LAB_1400f5a13;
    LOCK();
    uVar1 = *(uint *)local_348;
    *(uint *)local_348 = *(uint *)local_348 - 1;
    UNLOCK();
    if (uVar1 != 1) goto LAB_1400f5a13;
  }
  thunk_FUN_1400deec0(local_348);
LAB_1400f5a13:
  thunk_FUN_1401621e0(local_c8);
  return;
}

// =============================================================================
// Ghidra: FUN_1400f9c90 @ 0x1400f9c90
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined1 FUN_1400f9c90(void)

{
  uint uVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  QVariant *pQVar5;
  undefined8 uVar6;
  QMapDataBase *pQVar7;
  ulonglong *puVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  undefined1 uVar11;
  QMapDataBase *pQVar12;
  QMapDataBase *pQVar13;
  undefined1 auStack_268 [32];
  undefined4 local_248;
  QMapDataBase *local_238;
  QTypedArrayData<unsigned_short> *local_230;
  undefined4 local_228;
  QTypedArrayData<unsigned_short> *local_220;
  QTypedArrayData<unsigned_short> *local_218;
  QVariant local_210 [16];
  QVariant local_200 [16];
  undefined8 local_1f0;
  QVariant local_1e8 [16];
  QVariant local_1d8 [16];
  QVariant local_1c8 [16];
  undefined1 local_1b8 [16];
  undefined1 local_1a8 [16];
  undefined1 local_198 [92];
  int iStack_13c;
  undefined *local_138;
  basic_streambuf<char,std::char_traits<char>_> local_130 [128];
  basic_ios<char,std::char_traits<char>_> local_b0 [104];
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_1f0 = 0xfffffffffffffffe;
  local_28 = DAT_140e88880 ^ (ulonglong)auStack_268;
  pQVar13 = (QMapDataBase *)0x0;
  local_228 = 0;
  thunk_FUN_140162050(local_198,0);
  local_220 = QString::fromAscii_helper("viewerMaintenance",0x11);
  thunk_FUN_140163ad0(local_198,&local_220);
  QString::~QString((QString *)&local_220);
  pQVar5 = (QVariant *)thunk_FUN_140162d10(local_198,local_1e8);
  QVariant::toMap(pQVar5);
  QVariant::~QVariant(local_1e8);
  local_218 = QString::fromAscii_helper("removeLocalCacheOnStartup",0x19);
  uVar6 = QVariant::QVariant(local_1c8);
  pQVar5 = (QVariant *)thunk_FUN_140100930(&local_238,local_1d8,&local_218,uVar6);
  bVar2 = QVariant::toBool(pQVar5);
  QVariant::~QVariant(local_1d8);
  QVariant::~QVariant(local_1c8);
  QString::~QString((QString *)&local_218);
  uVar11 = 0;
  if (bVar2) {
    QVariant::QVariant(local_210,false);
    local_230 = QString::fromAscii_helper("removeLocalCacheOnStartup",0x19);
    if (1 < *(uint *)local_238) {
      pQVar7 = QMapDataBase::createData();
      if (*(longlong *)(local_238 + 0x10) != 0) {
        puVar8 = (ulonglong *)thunk_FUN_1400da660(*(longlong *)(local_238 + 0x10),pQVar7);
        *(ulonglong **)(pQVar7 + 0x10) = puVar8;
        *puVar8 = *puVar8 & 3;
        *puVar8 = *puVar8 | (ulonglong)(pQVar7 + 8);
      }
      if (*(uint *)local_238 == 0) {
LAB_1400f9e25:
        thunk_FUN_1400deec0(local_238);
      }
      else if (*(uint *)local_238 != 0xffffffff) {
        LOCK();
        uVar1 = *(uint *)local_238;
        *(uint *)local_238 = *(uint *)local_238 - 1;
        UNLOCK();
        if (uVar1 == 1) goto LAB_1400f9e25;
      }
      local_238 = pQVar7;
      QMapDataBase::recalcMostLeftNode(pQVar7);
    }
    pQVar7 = local_238 + 8;
    bVar2 = true;
    pQVar12 = *(QMapDataBase **)(local_238 + 0x10);
    if (*(QMapDataBase **)(local_238 + 0x10) == (QMapDataBase *)0x0) {
LAB_1400f9eac:
      local_248 = CONCAT31(local_248._1_3_,bVar2);
      thunk_FUN_1400daa20(local_238,&local_230,local_210,pQVar7);
    }
    else {
      do {
        pQVar7 = pQVar12;
        bVar2 = operator<((QString *)(pQVar7 + 0x18),(QString *)&local_230);
        if (bVar2) {
          pQVar12 = *(QMapDataBase **)(pQVar7 + 0x10);
        }
        else {
          pQVar12 = *(QMapDataBase **)(pQVar7 + 8);
          pQVar13 = pQVar7;
        }
        bVar2 = !bVar2;
      } while (pQVar12 != (QMapDataBase *)0x0);
      if ((pQVar13 == (QMapDataBase *)0x0) ||
         (bVar3 = operator<((QString *)&local_230,(QString *)(pQVar13 + 0x18)), bVar3))
      goto LAB_1400f9eac;
      QVariant::operator=((QVariant *)(pQVar13 + 0x20),local_210);
    }
    QString::~QString((QString *)&local_230);
    QVariant::~QVariant(local_210);
    QVariant::QVariant(local_200,(QMap<QString,QVariant> *)&local_238);
    thunk_FUN_1401639e0(local_198,local_200);
    QVariant::~QVariant(local_200);
    cVar4 = thunk_FUN_140162900(local_198);
    if (cVar4 == '\0') {
      uVar6 = thunk_FUN_14039be80(local_1b8);
      cVar4 = thunk_FUN_14039ba40(uVar6,40000);
      thunk_FUN_14039a8a0(local_1b8);
      if (cVar4 != '\0') {
        local_138 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_b0);
        local_228 = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_138,local_130,false);
        *(undefined ***)(local_130 + (longlong)*(int *)(local_138 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_13c + (longlong)*(int *)(local_138 + 4)) =
             *(int *)(local_138 + 4) + -0x88;
        thunk_FUN_140076630(local_130,2);
        thunk_FUN_14006fb70(&local_138,"unable to clear local cache removal request");
        uVar6 = thunk_FUN_1400876c0(&local_138,local_48);
        uVar9 = thunk_FUN_14039be80(local_1a8);
        local_248 = 0x87;
        thunk_FUN_14039b070(uVar9,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                           );
        thunk_FUN_14039a8a0(local_1a8);
        if (0xf < local_30) {
          uVar10 = local_48[0];
          if (0xfff < local_30 + 1) {
            if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar10 = *(ulonglong *)(local_48[0] - 8);
            if (local_48[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_48[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_48[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar10);
        }
        thunk_FUN_140077af0(local_b0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_b0);
      }
    }
    uVar11 = 1;
  }
  if (*(uint *)local_238 != 0) {
    if (*(uint *)local_238 == 0xffffffff) goto LAB_1400fa0a6;
    LOCK();
    uVar1 = *(uint *)local_238;
    *(uint *)local_238 = *(uint *)local_238 - 1;
    UNLOCK();
    if (uVar1 != 1) goto LAB_1400fa0a6;
  }
  thunk_FUN_1400deec0(local_238);
LAB_1400fa0a6:
  thunk_FUN_1401621e0(local_198);
  return uVar11;
}

// =============================================================================
// Ghidra: FUN_1400d3be0 @ 0x1400d3be0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400d3be0(longlong param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  void **ppvVar5;
  QString *pQVar6;
  __int64 _Var7;
  undefined8 uVar8;
  basic_ostream<char,std::char_traits<char>_> *pbVar9;
  undefined8 uVar10;
  ulonglong uVar11;
  basic_ios<char,std::char_traits<char>_> *this;
  QString *pQVar12;
  uint uVar13;
  undefined1 auStack_468 [32];
  code *local_448;
  undefined4 local_440;
  Data *local_438;
  uint local_430;
  QTypedArrayData<unsigned_short> *local_428;
  QTypedArrayData<unsigned_short> *local_420;
  undefined ***local_418;
  Connection local_410 [8];
  QEventLoop local_408 [16];
  undefined8 local_3f8;
  undefined1 local_3f0 [16];
  undefined1 local_3e0 [16];
  undefined1 local_3d0 [16];
  undefined1 local_3c0 [16];
  undefined1 local_3b0 [16];
  undefined1 local_3a0 [16];
  QTimer local_390 [36];
  int iStack_36c;
  undefined *local_368;
  undefined **local_360;
  basic_ostream<char,std::char_traits<char>_> local_358 [96];
  undefined8 local_2f8;
  undefined4 local_2f0;
  basic_ios<char,std::char_traits<char>_> local_2e0 [100];
  int iStack_27c;
  undefined *local_278;
  basic_streambuf<char,std::char_traits<char>_> local_270 [128];
  basic_ios<char,std::char_traits<char>_> local_1f0 [100];
  int iStack_18c;
  undefined *local_188;
  basic_streambuf<char,std::char_traits<char>_> local_180 [128];
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  ulonglong local_98 [3];
  ulonglong local_80;
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_3f8 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_468;
  uVar13 = 0;
  local_430 = 0;
  QCoreApplication::arguments();
  local_428 = QString::fromAscii_helper("--no-splash",0xb);
  bVar1 = QtPrivate::QStringList_contains((QStringList *)&local_438,(QString *)&local_428,1);
  *(bool *)(param_1 + 0xa9) = !bVar1;
  QString::~QString((QString *)&local_428);
  local_420 = QString::fromAscii_helper("--await-pid",0xb);
  iVar3 = QListData::size((QListData *)&local_438);
  if (0 < iVar3) {
    ppvVar5 = QListData::at((QListData *)&local_438,-1);
    pQVar6 = (QString *)QListData::end((QListData *)&local_438);
    for (pQVar12 = (QString *)(ppvVar5 + 1); pQVar12 != pQVar6; pQVar12 = pQVar12 + 8) {
      bVar1 = operator==(pQVar12,(QString *)&local_420);
      if (bVar1) {
        ppvVar5 = QListData::begin((QListData *)&local_438);
        iVar3 = (int)((longlong)pQVar12 - (longlong)ppvVar5 >> 3);
        goto LAB_1400d3d03;
      }
    }
  }
  iVar3 = -1;
LAB_1400d3d03:
  QString::~QString((QString *)&local_420);
  if (iVar3 < 0) goto LAB_1400d4343;
  iVar4 = QListData::size((QListData *)&local_438);
  if (iVar3 < iVar4 + -1) {
    bVar1 = false;
    iVar3 = iVar3 + 1;
    if ((iVar3 < 0) || (iVar4 = QListData::size((QListData *)&local_438), iVar4 <= iVar3)) {
      qt_assert_x("QList<T>::operator[]","index out of range",
                  "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
    }
    if (1 < *(uint *)local_438) {
      thunk_FUN_140080270(&local_438,*(uint *)(local_438 + 4));
    }
    pQVar12 = (QString *)QListData::at((QListData *)&local_438,iVar3);
    _Var7 = QString::toLongLong(pQVar12,(bool *)0x0,10);
    cVar2 = thunk_FUN_1401e8300(_Var7);
    if (cVar2 == '\0') goto LAB_1400d4343;
    do {
      if (!bVar1) {
        uVar8 = thunk_FUN_14039be80(local_3f0);
        cVar2 = thunk_FUN_14039ba40(uVar8);
        thunk_FUN_14039a8a0(local_3f0);
        if (cVar2 != '\0') {
          local_368 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_2e0);
          uVar13 = 1;
          local_430 = 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_368,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_360,false);
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_368 + *(int *)(local_368 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_36c + (longlong)*(int *)(local_368 + 4)) =
               *(int *)(local_368 + 4) + -0x88;
          local_418 = &local_360;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_360);
          local_360 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_2f8 = 0;
          local_2f0 = 4;
          pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *)&local_368,
                                       "await pid ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,_Var7);
          uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)&local_368,
                                      local_98);
          uVar10 = thunk_FUN_14039be80(local_3e0);
          local_448 = (code *)CONCAT44(local_448._4_4_,0x32d);
          thunk_FUN_14039b070(uVar10,20000,uVar8,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp"
                             );
          thunk_FUN_14039a8a0(local_3e0);
          if (0xf < local_80) {
            uVar11 = local_98[0];
            if (0xfff < local_80 + 1) {
              if ((local_98[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar11 = *(ulonglong *)(local_98[0] - 8);
              if (local_98[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_98[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_98[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar11);
          }
          *(undefined ***)
           ((basic_ostream<char,std::char_traits<char>_> *)&local_368 + *(int *)(local_368 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_36c + (longlong)*(int *)(local_368 + 4)) =
               *(int *)(local_368 + 4) + -0x88;
          local_360 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14007bb20(&local_360);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_360);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_358);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_2e0);
        }
        bVar1 = true;
      }
      QTimer::QTimer(local_390,(QObject *)0x0);
      QEventLoop::QEventLoop(local_408,(QObject *)0x0);
      local_440 = 0;
      local_448 = quit_exref;
      thunk_FUN_14008e300(local_410,local_390,timeout_exref,local_408);
      QMetaObject::Connection::~Connection(local_410);
      QTimer::start(local_390,100);
      local_418 = (undefined ***)((ulonglong)local_418 & 0xffffffff00000000);
      QEventLoop::exec(local_408);
      QEventLoop::~QEventLoop(local_408);
      QTimer::~QTimer(local_390);
      cVar2 = thunk_FUN_1401e8300(_Var7);
    } while (cVar2 != '\0');
    if (!bVar1) goto LAB_1400d4343;
    uVar8 = thunk_FUN_14039be80(local_3d0);
    cVar2 = thunk_FUN_14039ba40(uVar8,20000);
    thunk_FUN_14039a8a0(local_3d0);
    if (cVar2 == '\0') goto LAB_1400d4343;
    local_278 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1f0)
    ;
    local_430 = uVar13 | 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_278,local_270,false);
    *(undefined ***)(local_270 + (longlong)*(int *)(local_278 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_27c + (longlong)*(int *)(local_278 + 4)) =
         *(int *)(local_278 + 4) + -0x88;
    thunk_FUN_140076630(local_270,2);
    pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14006fb70(&local_278,"ready pid ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,_Var7);
    uVar8 = thunk_FUN_1400876c0(&local_278,local_78);
    uVar10 = thunk_FUN_14039be80(local_3c0);
    local_448 = (code *)CONCAT44(local_448._4_4_,0x33b);
    thunk_FUN_14039b070(uVar10,20000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp");
    thunk_FUN_14039a8a0(local_3c0);
    if (0xf < local_60) {
      uVar11 = local_78[0];
      if (0xfff < local_60 + 1) {
        if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar11 = *(ulonglong *)(local_78[0] - 8);
        if (local_78[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_78[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_78[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar11);
    }
    thunk_FUN_140077af0(local_1f0);
    this = local_1f0;
  }
  else {
    uVar8 = thunk_FUN_14039be80(local_3b0);
    cVar2 = thunk_FUN_14039ba40(uVar8,30000);
    thunk_FUN_14039a8a0(local_3b0);
    if (cVar2 == '\0') goto LAB_1400d4343;
    local_188 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_100)
    ;
    local_430 = 4;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_188,local_180,false);
    *(undefined ***)(local_180 + (longlong)*(int *)(local_188 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
         *(int *)(local_188 + 4) + -0x88;
    thunk_FUN_140076630(local_180,2);
    thunk_FUN_14006fb70(&local_188,"value for --await-pid missing");
    uVar8 = thunk_FUN_1400876c0(&local_188,local_58);
    uVar10 = thunk_FUN_14039be80(local_3a0);
    local_448 = (code *)CONCAT44(local_448._4_4_,0x340);
    thunk_FUN_14039b070(uVar10,30000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp");
    thunk_FUN_14039a8a0(local_3a0);
    if (0xf < local_40) {
      uVar11 = local_58[0];
      if (0xfff < local_40 + 1) {
        if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar11 = *(ulonglong *)(local_58[0] - 8);
        if (local_58[0] <= uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_58[0] - uVar11 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_58[0] - uVar11) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar11);
    }
    thunk_FUN_140077af0(local_100);
    this = local_100;
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(this);
LAB_1400d4343:
  if (*(int *)local_438 != 0) {
    if (*(int *)local_438 == -1) {
      return;
    }
    LOCK();
    iVar3 = *(int *)local_438;
    *(int *)local_438 = *(int *)local_438 + -1;
    UNLOCK();
    if (iVar3 != 1) {
      return;
    }
  }
  pQVar12 = (QString *)(local_438 + ((longlong)*(int *)(local_438 + 0xc) + 2) * 8);
  iVar3 = *(int *)(local_438 + 8);
  while ((QString *)(local_438 + ((longlong)iVar3 + 2) * 8) != pQVar12) {
    pQVar12 = pQVar12 + -8;
    QString::~QString(pQVar12);
  }
  QListData::dispose(local_438);
  return;
}

// =============================================================================
// Ghidra: FUN_1400c46f0 @ 0x1400c46f0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1400c46f0(QApplication *param_1)

{
  longlong *plVar1;
  undefined8 *puVar2;
  char cVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined4 local_168;
  undefined1 local_160 [16];
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
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_198;
  local_168 = 0;
  *(undefined ***)param_1 = viewer::Application::vftable;
  *(undefined ***)(param_1 + 0x20) = viewer::Application::vftable;
  *(undefined ***)(param_1 + 0x30) = viewer::Application::vftable;
  thunk_FUN_1400db350();
  uVar4 = thunk_FUN_14039be80(local_160);
  cVar3 = thunk_FUN_14039ba40(uVar4,20000);
  thunk_FUN_14039a8a0(local_160);
  if (cVar3 != '\0') {
    local_148 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
         *(int *)(local_148 + 4) + -0x88;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_d8 = 0;
    local_d0 = 4;
    thunk_FUN_14006fb70(&local_148,"application shutdown");
    uVar4 = thunk_FUN_1400876c0(&local_148,local_58);
    uVar5 = thunk_FUN_14039be80(local_160);
    local_178 = 0xd6;
    thunk_FUN_14039b070(uVar5,20000,uVar4,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\app-viewer\\application.cpp");
    thunk_FUN_14039a8a0(local_160);
    if (0xf < local_40) {
      uVar6 = local_58[0];
      if (0xfff < local_40 + 1) {
        if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar6 = *(ulonglong *)(local_58[0] - 8);
        if (local_58[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_58[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_58[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar6);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0)
    ;
  }
  QString::~QString((QString *)(param_1 + 0x118));
  QIcon::~QIcon((QIcon *)(param_1 + 0x110));
  plVar1 = *(longlong **)(param_1 + 0x108);
  if (plVar1 != (longlong *)0x0) {
    (**(code **)(*plVar1 + 0x18))(plVar1,1);
  }
  puVar2 = *(undefined8 **)(param_1 + 0x100);
  if (puVar2 != (undefined8 *)0x0) {
    (**(code **)*puVar2)(puVar2,1);
  }
  QTimer::~QTimer((QTimer *)(param_1 + 0xd8));
  QTimeZone::~QTimeZone((QTimeZone *)(param_1 + 0xd0));
  thunk_FUN_1400775d0(param_1 + 0xb0);
  thunk_FUN_1401574a0(param_1 + 0x30);
  QAbstractNativeEventFilter::~QAbstractNativeEventFilter
            ((QAbstractNativeEventFilter *)(param_1 + 0x20));
  QApplication::~QApplication(param_1);
  return;
}

// =============================================================================
// Ghidra: FUN_140235300 @ 0x140235300
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140235300(longlong param_1)

{
  bool bVar1;
  char cVar2;
  undefined ***pppuVar3;
  undefined ***pppuVar4;
  undefined **ppuVar5;
  undefined ***pppuVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  QByteArray *pQVar10;
  char *pcVar11;
  basic_ostream<char,std::char_traits<char>_> *this;
  QString *this_00;
  QSslCertificate *this_01;
  QByteArray *this_02;
  undefined1 auStack_1e8 [32];
  undefined ***local_1c8;
  undefined ***local_1b8 [2];
  undefined4 local_1a8 [2];
  QSslKey local_1a0 [8];
  undefined ***local_198;
  undefined ***local_190;
  QByteArray local_188 [8];
  QSslCertificate local_180 [8];
  QByteArray local_178 [8];
  undefined8 local_170;
  undefined *local_168;
  undefined **local_160;
  basic_ostream<char,std::char_traits<char>_> local_158 [96];
  undefined8 local_f8;
  undefined4 local_f0;
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  undefined1 local_78 [32];
  undefined **local_58;
  undefined ***local_50;
  ulonglong local_40;
  ulonglong local_38;
  
  local_170 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_1e8;
  local_1a8[0] = 0;
  bVar1 = QTcpServer::isListening((QTcpServer *)(*(longlong *)(param_1 + 0x1e8) + 0x78));
  if (!bVar1) {
    pppuVar3 = operator_new(0x40);
    local_1b8[0] = pppuVar3;
    if (pppuVar3 == (undefined ***)0x0) {
      pppuVar3 = (undefined ***)0x0;
    }
    else {
      thunk_FUN_14018c0c0(pppuVar3);
      *pppuVar3 = node::protocol::control::master::PostMasterPoint::vftable;
      QString::QString((QString *)(pppuVar3 + 1));
      QString::QString((QString *)(pppuVar3 + 3));
      QString::QString((QString *)(pppuVar3 + 4));
      QString::QString((QString *)(pppuVar3 + 6));
    }
    local_198 = pppuVar3;
    if (pppuVar3 == (undefined ***)0x0) {
      pppuVar4 = (undefined ***)0x0;
      local_190 = (undefined ***)0x0;
    }
    else {
      pppuVar4 = operator_new(0x18);
      if (pppuVar4 + 2 != (undefined ***)0x0) {
        pppuVar4[2] = (undefined **)pppuVar3;
      }
      if (pppuVar4 != (undefined ***)0x0) {
        pppuVar4[1] = (undefined **)&LAB_140024672;
        *(undefined4 *)((longlong)pppuVar4 + 4) = 1;
        *(undefined4 *)pppuVar4 = 1;
      }
      local_1b8[0] = pppuVar4;
      local_190 = pppuVar4;
      thunk_FUN_1400871f0(pppuVar4,pppuVar3,1);
      FUN_14001273d(&local_198,pppuVar3);
    }
    QString::operator=((QString *)(pppuVar3 + 1),(QString *)(param_1 + 0x230));
    pppuVar3[2] = *(undefined ***)(param_1 + 0x1c8);
    QString::operator=((QString *)(pppuVar3 + 3),(QString *)(param_1 + 0x1d0));
    local_1b8[0] = operator_new(0x18);
    if (local_1b8[0] == (undefined ***)0x0) {
      ppuVar5 = (undefined **)0x0;
    }
    else {
      ppuVar5 = (undefined **)
                thunk_FUN_1404ddd10(local_1b8[0],*(undefined8 *)(param_1 + 0x1b0),
                                    *(undefined8 *)(param_1 + 0x1a8));
    }
    local_58 = ppuVar5;
    if (ppuVar5 == (undefined **)0x0) {
      pppuVar6 = (undefined ***)0x0;
      local_50 = (undefined ***)0x0;
    }
    else {
      pppuVar6 = operator_new(0x18);
      if (pppuVar6 + 2 != (undefined ***)0x0) {
        pppuVar6[2] = ppuVar5;
      }
      if (pppuVar6 != (undefined ***)0x0) {
        pppuVar6[1] = (undefined **)&LAB_140007270;
        *(undefined4 *)((longlong)pppuVar6 + 4) = 1;
        *(undefined4 *)pppuVar6 = 1;
      }
      local_1b8[0] = pppuVar6;
      local_50 = pppuVar6;
      thunk_FUN_1400871f0(pppuVar6,ppuVar5,1);
      FUN_140015adc(&local_58,ppuVar5);
    }
    uVar7 = thunk_FUN_140083800();
    uVar7 = thunk_FUN_14011d400(uVar7,local_78);
    uVar8 = thunk_FUN_140083800();
    uVar8 = thunk_FUN_14011d220(uVar8,local_1a0);
    uVar9 = thunk_FUN_140083800();
    uVar9 = thunk_FUN_14011d1a0(uVar9,local_1a8);
    local_1c8 = &local_58;
    thunk_FUN_140237ad0(*(undefined8 *)(param_1 + 0x1e8),uVar9,uVar8,uVar7);
    QSslCertificate::~QSslCertificate((QSslCertificate *)local_1a8);
    QSslKey::~QSslKey(local_1a0);
    thunk_FUN_14019b6f0(local_78);
    thunk_FUN_1400db8e0(pppuVar6);
    thunk_FUN_140238080(*(undefined8 *)(param_1 + 0x1e8),pppuVar3 + 4);
    uVar7 = thunk_FUN_14039be80(local_1b8);
    cVar2 = thunk_FUN_14039ba40(uVar7,20000);
    thunk_FUN_14039a8a0(local_1b8);
    if (cVar2 != '\0') {
      local_168 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_e0);
      local_1a8[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_168,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_160,false);
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_170 + (longlong)*(int *)(local_168 + 4) + 4) =
           *(int *)(local_168 + 4) + -0x88;
      local_1b8[0] = &local_160;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_f8 = 0;
      local_f0 = 4;
      uVar7 = thunk_FUN_14006fb70(&local_168,"control master start server: ");
      pQVar10 = (QByteArray *)QString::toLocal8Bit((QString *)(pppuVar3 + 4));
      pcVar11 = QByteArray::data(pQVar10);
      thunk_FUN_14006fb70(uVar7,pcVar11);
      QByteArray::~QByteArray((QByteArray *)local_1a0);
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14006fb70(uVar7,&DAT_1405a2ac4);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,*(ushort *)(pppuVar3 + 5));
      uVar7 = thunk_FUN_1400876c0(&local_168,&local_58);
      uVar8 = thunk_FUN_14039be80(local_1b8);
      local_1c8 = (undefined ***)CONCAT44(local_1c8._4_4_,0x150);
      thunk_FUN_14039b070(uVar8,20000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                         );
      thunk_FUN_14039a8a0(local_1b8);
      if (0xf < local_40) {
        ppuVar5 = local_58;
        if (0xfff < local_40 + 1) {
          if (((ulonglong)local_58 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          ppuVar5 = (undefined **)local_58[-1];
          if (local_58 <= ppuVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if ((ulonglong)((longlong)local_58 - (longlong)ppuVar5) < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < (ulonglong)((longlong)local_58 - (longlong)ppuVar5)) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(ppuVar5);
      }
      *(undefined ***)((longlong)&local_168 + (longlong)*(int *)(local_168 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_170 + (longlong)*(int *)(local_168 + 4) + 4) =
           *(int *)(local_168 + 4) + -0x88;
      local_160 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_160);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_160);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_158);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_e0);
    }
    uVar7 = thunk_FUN_1401831a0(local_1b8,param_1 + 0x230,*(undefined1 *)(param_1 + 0x1c8));
    this_00 = (QString *)thunk_FUN_1400793a0(&local_58,uVar7,param_1 + 0x1d0);
    pQVar10 = (QByteArray *)QString::toUtf8(this_00);
    uVar7 = thunk_FUN_140083800();
    this_01 = (QSslCertificate *)thunk_FUN_14011d1a0(uVar7,local_180);
    this_02 = (QByteArray *)QSslCertificate::toPem(this_01);
    pQVar10 = QByteArray::append(this_02,pQVar10);
    pQVar10 = (QByteArray *)QCryptographicHash::hash((QByteArray *)local_1a8,(Algorithm)pQVar10);
    pQVar10 = (QByteArray *)QByteArray::toHex(pQVar10);
    QString::operator=((QString *)(pppuVar3 + 6),pQVar10);
    QByteArray::~QByteArray((QByteArray *)local_1a0);
    QByteArray::~QByteArray((QByteArray *)local_1a8);
    QByteArray::~QByteArray(local_188);
    QSslCertificate::~QSslCertificate(local_180);
    QByteArray::~QByteArray(local_178);
    QString::~QString((QString *)&local_58);
    QString::~QString((QString *)local_1b8);
    *(undefined1 *)(pppuVar3 + 7) = 1;
    thunk_FUN_140249640(*(undefined8 *)(param_1 + 0x1e8),pppuVar3 + 6);
    thunk_FUN_140232a70(param_1,&local_198);
    thunk_FUN_140234ed0(pppuVar4);
  }
  return;
}

// =============================================================================
// Ghidra: FUN_140236a10 @ 0x140236a10
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140236a10(longlong param_1,longlong *param_2)

{
  int iVar1;
  bool bVar2;
  char cVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  QString *this;
  QByteArray *this_00;
  char *pcVar6;
  ulonglong uVar7;
  undefined1 auStack_2f8 [32];
  undefined4 local_2d8;
  undefined4 local_2c8;
  undefined ***local_2c0;
  QByteArray local_2b8 [8];
  undefined1 local_2b0 [8];
  Data *local_2a8;
  undefined8 local_2a0;
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
  basic_streambuf<char,std::char_traits<char>_> local_160 [128];
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_2a0 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_2f8;
  local_2c8 = 0;
  bVar2 = operator==((QString *)(*param_2 + 8),(QString *)(param_1 + 0x230));
  if (((!bVar2) || (*(longlong *)(*param_2 + 0x10) != *(longlong *)(param_1 + 0x1c8))) ||
     (bVar2 = operator==((QString *)(*param_2 + 0x18),(QString *)(param_1 + 0x1d0)), !bVar2)) {
    uVar4 = thunk_FUN_14039be80(local_278);
    cVar3 = thunk_FUN_14039ba40(uVar4,40000);
    thunk_FUN_14039a8a0(local_278);
    if (cVar3 != '\0') {
      local_258 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_1d0);
      local_2c8 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_258,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_250,false);
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_2c0 = &local_250;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1e8 = 0;
      local_1e0 = 4;
      this = (QString *)(**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,&local_2c0)
      ;
      uVar4 = thunk_FUN_14006fb70(&local_258,"crazy node during control talk: ");
      this_00 = (QByteArray *)QString::toLocal8Bit(this);
      pcVar6 = QByteArray::data(this_00);
      thunk_FUN_14006fb70(uVar4,pcVar6);
      QByteArray::~QByteArray(local_2b8);
      QString::~QString((QString *)&local_2c0);
      uVar4 = thunk_FUN_1400876c0(&local_258,local_58);
      uVar5 = thunk_FUN_14039be80(local_268);
      local_2d8 = 0x18d;
      thunk_FUN_14039b070(uVar5,40000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                         );
      thunk_FUN_14039a8a0(local_268);
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
        FUN_140002d6f(uVar7);
      }
      *(undefined ***)((longlong)&local_258 + (longlong)*(int *)(local_258 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_25c + (longlong)*(int *)(local_258 + 4)) =
           *(int *)(local_258 + 4) + -0x88;
      local_250 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_250);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_250);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_248);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_1d0);
    }
    thunk_FUN_14018ec00(param_1);
    return;
  }
  uVar4 = thunk_FUN_14039be80(local_298);
  cVar3 = thunk_FUN_14039ba40(uVar4,20000);
  thunk_FUN_14039a8a0(local_298);
  if (cVar3 == '\0') goto LAB_140236c81;
  local_168 = &DAT_1405a3878;
  std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_e0);
  local_2c8 = 2;
  std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
            ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
  *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
       std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
  *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
       *(int *)(local_168 + 4) + -0x88;
  thunk_FUN_140076630(local_160,2);
  uVar4 = thunk_FUN_140198d70(local_2b0,*param_2 + 0x20,*param_2 + 0x28);
  uVar5 = thunk_FUN_14006fb70(&local_168,"control master receive slave point: ");
  thunk_FUN_1401196d0(uVar5,uVar4);
  if (*(int *)local_2a8 == 0) {
LAB_140236baa:
    QListData::dispose(local_2a8);
  }
  else if (*(int *)local_2a8 != -1) {
    LOCK();
    iVar1 = *(int *)local_2a8;
    *(int *)local_2a8 = *(int *)local_2a8 + -1;
    UNLOCK();
    if (iVar1 == 1) goto LAB_140236baa;
  }
  thunk_FUN_1400775d0(local_2b0);
  uVar4 = thunk_FUN_1400876c0(&local_168,local_78);
  uVar5 = thunk_FUN_14039be80(local_288);
  local_2d8 = 0x192;
  thunk_FUN_14039b070(uVar5,20000,uVar4,
                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                     );
  thunk_FUN_14039a8a0(local_288);
  if (0xf < local_60) {
    uVar7 = local_78[0];
    if (0xfff < local_60 + 1) {
      if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
      uVar7 = *(ulonglong *)(local_78[0] - 8);
      if (local_78[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
      if (local_78[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
      if (0x27 < local_78[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
        _invalid_parameter_noinfo_noreturn();
      }
    }
    FUN_140002d6f(uVar7);
  }
  thunk_FUN_140077af0(local_e0);
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_e0);
LAB_140236c81:
  thunk_FUN_1400c8c10(param_1 + 0x240,*param_2 + 0x20);
  thunk_FUN_140198fa0(param_1 + 0x248,*param_2 + 0x28);
  QString::operator=((QString *)(param_1 + 0x250),(QString *)(*param_2 + 0x30));
  *(undefined1 *)(param_1 + 600) = *(undefined1 *)(*param_2 + 0x38);
  thunk_FUN_140238b20(param_1);
  return;
}

// =============================================================================
// Ghidra: FUN_140238b20 @ 0x140238b20
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140238b20(QObject *param_1)

{
  QListData *this;
  int iVar1;
  longlong lVar2;
  longlong *plVar3;
  bool bVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  void **ppvVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  QSlotObjectBase *pQVar12;
  QSlotObjectBase *pQVar13;
  undefined8 uVar14;
  ulonglong uVar15;
  undefined1 auStack_478 [32];
  QSlotObjectBase **local_458;
  undefined4 local_448;
  QSlotObjectBase *local_440;
  undefined ***local_438;
  QSlotObjectBase *local_430;
  QSlotObjectBase *local_428;
  QSslCertificate local_420 [8];
  undefined1 local_418 [8];
  Data *local_410;
  undefined1 local_408 [8];
  Data *local_400;
  undefined8 local_3f8;
  undefined1 local_3f0 [16];
  undefined1 local_3e0 [16];
  undefined1 local_3d0 [16];
  undefined1 local_3c0 [16];
  undefined1 local_3b0 [16];
  undefined1 local_3a0 [20];
  int iStack_38c;
  undefined *local_388;
  undefined **local_380;
  basic_ostream<char,std::char_traits<char>_> local_378 [96];
  undefined8 local_318;
  undefined4 local_310;
  basic_ios<char,std::char_traits<char>_> local_300 [100];
  int iStack_29c;
  undefined *local_298;
  undefined **local_290;
  basic_ostream<char,std::char_traits<char>_> local_288 [96];
  undefined8 local_228;
  undefined4 local_220;
  basic_ios<char,std::char_traits<char>_> local_210 [100];
  int iStack_1ac;
  undefined *local_1a8;
  undefined **local_1a0;
  basic_ostream<char,std::char_traits<char>_> local_198 [96];
  undefined8 local_138;
  undefined4 local_130;
  basic_ios<char,std::char_traits<char>_> local_120 [104];
  undefined1 local_b8 [32];
  ulonglong local_98 [3];
  ulonglong local_80;
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_3f8 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_478;
  pQVar13 = (QSlotObjectBase *)0x0;
  local_448 = 0;
  iVar1 = *(int *)(param_1 + 0x238);
  bVar4 = false;
  this = (QListData *)(param_1 + 0x240);
  iVar7 = QListData::size(this);
  pQVar12 = pQVar13;
  bVar5 = false;
  if (0 < iVar7) {
    do {
      bVar5 = bVar4;
      if (bVar4) break;
      iVar7 = (int)pQVar12;
      if ((iVar7 < 0) || (iVar8 = QListData::size(this), iVar8 <= iVar7)) {
        qt_assert_x("QList<T>::at","index out of range",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x213);
      }
      ppvVar9 = QListData::at(this,iVar7);
      if (*(int *)((longlong)*ppvVar9 + 4) != 0) {
        if ((iVar7 < 0) || (iVar8 = QListData::size((QListData *)(param_1 + 0x248)), iVar8 <= iVar7)
           ) {
          qt_assert_x("QList<T>::at","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x213);
        }
        ppvVar9 = QListData::at((QListData *)(param_1 + 0x248),iVar7);
        if (*(short *)ppvVar9 != 0) {
          bVar4 = true;
        }
      }
      pQVar12 = (QSlotObjectBase *)(ulonglong)(iVar7 + 1U);
      iVar8 = QListData::size(this);
      bVar5 = bVar4;
    } while ((int)(iVar7 + 1U) < iVar8);
  }
  if (1 < iVar1 - 4U) {
    uVar10 = thunk_FUN_14039be80(local_3f0);
    cVar6 = thunk_FUN_14039ba40(uVar10,20000);
    thunk_FUN_14039a8a0(local_3f0);
    if (cVar6 != '\0') {
      local_388 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_300);
      local_448 = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_388,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_380,false);
      *(undefined ***)((longlong)&local_388 + (longlong)*(int *)(local_388 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_38c + (longlong)*(int *)(local_388 + 4)) =
           *(int *)(local_388 + 4) + -0x88;
      local_438 = &local_380;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_380);
      local_380 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_318 = 0;
      local_310 = 4;
      thunk_FUN_14006fb70(&local_388,"control master stop client");
      uVar10 = thunk_FUN_1400876c0(&local_388,local_98);
      uVar11 = thunk_FUN_14039be80(local_3e0);
      local_458 = (QSlotObjectBase **)CONCAT44(local_458._4_4_,0xdc);
      thunk_FUN_14039b070(uVar11,20000,uVar10,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                         );
      thunk_FUN_14039a8a0(local_3e0);
      if (0xf < local_80) {
        uVar15 = local_98[0];
        if (0xfff < local_80 + 1) {
          if ((local_98[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_98[0] - 8);
          if (local_98[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_98[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_98[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar15);
      }
      *(undefined ***)((longlong)&local_388 + (longlong)*(int *)(local_388 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_38c + (longlong)*(int *)(local_388 + 4)) =
           *(int *)(local_388 + 4) + -0x88;
      local_380 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_380);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_380);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_378);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_300);
    }
    lVar2 = *(longlong *)(param_1 + 0x1e8);
    plVar3 = *(longlong **)(lVar2 + 0x70);
    if (plVar3 != (longlong *)0x0) {
      *(undefined8 *)(lVar2 + 0x70) = 0;
      (**(code **)(*plVar3 + 0x18))(plVar3,1);
    }
    *(undefined2 *)(lVar2 + 0xf0) = 0x101;
    if (*(longlong **)(lVar2 + 0xe8) == (longlong *)0x0) {
      return;
    }
    (**(code **)(**(longlong **)(lVar2 + 0xe8) + 0x98))();
    *(undefined8 *)(lVar2 + 0xe8) = 0;
    return;
  }
  cVar6 = thunk_FUN_14018eed0(*(undefined8 *)(param_1 + 0x1e8));
  if (cVar6 != '\0') {
    return;
  }
  if (*(longlong *)(*(longlong *)(param_1 + 0x1e8) + 0xe8) != 0) {
    return;
  }
  if (!bVar5) {
    uVar10 = thunk_FUN_14039be80(local_3b0);
    cVar6 = thunk_FUN_14039ba40(uVar10,20000);
    thunk_FUN_14039a8a0(local_3b0);
    if (cVar6 != '\0') {
      local_1a8 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_120);
      local_448 = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_1a8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_1a0,false);
      *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
           *(int *)(local_1a8 + 4) + -0x88;
      local_440 = (QSlotObjectBase *)&local_1a0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1a0);
      local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_138 = 0;
      local_130 = 4;
      thunk_FUN_14006fb70(&local_1a8,"control master not start client, no server");
      uVar10 = thunk_FUN_1400876c0(&local_1a8,local_58);
      uVar11 = thunk_FUN_14039be80(local_3a0);
      local_458 = (QSlotObjectBase **)CONCAT44(local_458._4_4_,0xf4);
      thunk_FUN_14039b070(uVar11,20000,uVar10,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                         );
      thunk_FUN_14039a8a0(local_3a0);
      if (0xf < local_40) {
        uVar15 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar15);
      }
      *(undefined ***)((longlong)&local_1a8 + (longlong)*(int *)(local_1a8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_1ac + (longlong)*(int *)(local_1a8 + 4)) =
           *(int *)(local_1a8 + 4) + -0x88;
      local_1a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_1a0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_1a0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_198);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_120);
    }
    local_440 = operator_new(0x18);
    if (local_440 != (QSlotObjectBase *)0x0) {
      *(undefined4 *)local_440 = 1;
      *(undefined1 **)(local_440 + 8) = &LAB_14001990c;
      *(QObject **)(local_440 + 0x10) = param_1;
      pQVar13 = local_440;
    }
    QTimer::singleShotImpl(3000,1,param_1,pQVar13);
    return;
  }
  uVar10 = thunk_FUN_14039be80(local_3d0);
  cVar6 = thunk_FUN_14039ba40(uVar10,20000);
  thunk_FUN_14039a8a0(local_3d0);
  if (cVar6 != '\0') {
    local_298 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_210)
    ;
    local_448 = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_298,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_290,false);
    *(undefined ***)((longlong)&local_298 + (longlong)*(int *)(local_298 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_29c + (longlong)*(int *)(local_298 + 4)) =
         *(int *)(local_298 + 4) + -0x88;
    local_438 = &local_290;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_290);
    local_290 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_228 = 0;
    local_220 = 4;
    uVar10 = thunk_FUN_140198d70(local_418,this,param_1 + 0x248);
    uVar11 = thunk_FUN_14006fb70(&local_298,"control master start client: ");
    thunk_FUN_1401196d0(uVar11,uVar10);
    if (*(int *)local_410 == 0) {
LAB_140238fb9:
      QListData::dispose(local_410);
    }
    else if (*(int *)local_410 != -1) {
      LOCK();
      iVar1 = *(int *)local_410;
      *(int *)local_410 = *(int *)local_410 + -1;
      UNLOCK();
      if (iVar1 == 1) goto LAB_140238fb9;
    }
    thunk_FUN_1400775d0(local_418);
    uVar10 = thunk_FUN_1400876c0(&local_298,local_78);
    uVar11 = thunk_FUN_14039be80(local_3c0);
    local_458 = (QSlotObjectBase **)CONCAT44(local_458._4_4_,0xe5);
    thunk_FUN_14039b070(uVar11,20000,uVar10,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                       );
    thunk_FUN_14039a8a0(local_3c0);
    if (0xf < local_60) {
      uVar15 = local_78[0];
      if (0xfff < local_60 + 1) {
        if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar15 = *(ulonglong *)(local_78[0] - 8);
        if (local_78[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_78[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_78[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar15);
    }
    *(undefined ***)((longlong)&local_298 + (longlong)*(int *)(local_298 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_29c + (longlong)*(int *)(local_298 + 4)) =
         *(int *)(local_298 + 4) + -0x88;
    local_290 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_290);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_290);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_288);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_210);
  }
  local_440 = operator_new(0x18);
  pQVar12 = pQVar13;
  if (local_440 != (QSlotObjectBase *)0x0) {
    pQVar12 = (QSlotObjectBase *)
              thunk_FUN_1404ddd10(local_440,*(undefined8 *)(param_1 + 0x1b0),
                                  *(undefined8 *)(param_1 + 0x1a8));
  }
  local_430 = pQVar12;
  if (pQVar12 == (QSlotObjectBase *)0x0) {
    local_428 = (QSlotObjectBase *)0x0;
  }
  else {
    pQVar13 = operator_new(0x18);
    if (pQVar13 + 0x10 != (QSlotObjectBase *)0x0) {
      *(QSlotObjectBase **)(pQVar13 + 0x10) = pQVar12;
    }
    if (pQVar13 != (QSlotObjectBase *)0x0) {
      *(undefined1 **)(pQVar13 + 8) = &LAB_140007270;
      *(undefined4 *)(pQVar13 + 4) = 1;
      *(undefined4 *)pQVar13 = 1;
    }
    local_440 = pQVar13;
    local_428 = pQVar13;
    thunk_FUN_1400871f0(pQVar13,pQVar12,1);
    FUN_140015adc(&local_430,pQVar12);
  }
  uVar10 = thunk_FUN_140083800();
  uVar10 = thunk_FUN_14011d400(uVar10,local_b8);
  uVar11 = thunk_FUN_140083800();
  uVar11 = thunk_FUN_14011d220(uVar11,&local_438);
  uVar14 = thunk_FUN_140083800();
  uVar14 = thunk_FUN_14011d1a0(uVar14,local_420);
  local_458 = &local_430;
  thunk_FUN_140237ad0(*(undefined8 *)(param_1 + 0x1e8),uVar14,uVar11,uVar10);
  QSslCertificate::~QSslCertificate(local_420);
  QSslKey::~QSslKey((QSslKey *)&local_438);
  thunk_FUN_14019b6f0(local_b8);
  thunk_FUN_1400db8e0(pQVar13);
  uVar10 = thunk_FUN_140198d70(local_408,this,param_1 + 0x248);
  thunk_FUN_140237de0(*(undefined8 *)(param_1 + 0x1e8),uVar10,
                      -(uint)(param_1[600] != (QObject)0x0) & 3000);
  if (*(int *)local_400 != 0) {
    if (*(int *)local_400 == -1) goto LAB_140239265;
    LOCK();
    iVar1 = *(int *)local_400;
    *(int *)local_400 = *(int *)local_400 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_140239265;
  }
  QListData::dispose(local_400);
LAB_140239265:
  thunk_FUN_1400775d0(local_408);
  thunk_FUN_140249a10(*(undefined8 *)(param_1 + 0x1e8),param_1 + 0x250);
  return;
}

// =============================================================================
// Ghidra: FUN_1402460b0 @ 0x1402460b0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402460b0(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  bool bVar2;
  char cVar3;
  undefined8 uVar4;
  QByteArray *pQVar5;
  char *pcVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  undefined1 auStack_198 [32];
  undefined4 local_178;
  undefined ***local_168 [2];
  undefined4 local_158 [3];
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
  local_30 = DAT_140e88880 ^ (ulonglong)auStack_198;
  local_158[0] = 0;
  bVar2 = operator==((QString *)(*param_2 + 8),(QString *)(param_1 + 0x118));
  if (bVar2) {
    uVar4 = thunk_FUN_14039be80(local_168);
    cVar3 = thunk_FUN_14039ba40(uVar4,20000);
    thunk_FUN_14039a8a0(local_168);
    if (cVar3 != '\0') {
      local_148 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_158[0] = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_168[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      thunk_FUN_14006fb70(&local_148,"control master authorize");
      uVar4 = thunk_FUN_1400876c0(&local_148,local_50);
      uVar7 = thunk_FUN_14039be80(local_168);
      local_178 = 0x8e;
      thunk_FUN_14039b070(uVar7,20000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp"
                         );
      thunk_FUN_14039a8a0(local_168);
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
        FUN_140002d6f(uVar8);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    thunk_FUN_14023a1e0(param_1,1);
  }
  else {
    uVar4 = thunk_FUN_14039be80(local_168);
    cVar3 = thunk_FUN_14039ba40(uVar4,30000);
    thunk_FUN_14039a8a0(local_168);
    if (cVar3 != '\0') {
      local_148 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_158[0] = 1;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_168[0] = &local_140;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_d8 = 0;
      local_d0 = 4;
      lVar1 = *param_2;
      uVar4 = thunk_FUN_14006fb70(&local_148,"control channel incoming auth failed: ");
      pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)(lVar1 + 8));
      pcVar6 = QByteArray::data(pQVar5);
      thunk_FUN_14006fb70(uVar4,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_158);
      uVar4 = thunk_FUN_14006fb70(uVar4,&DAT_140bf945c);
      pQVar5 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x118));
      pcVar6 = QByteArray::data(pQVar5);
      thunk_FUN_14006fb70(uVar4,pcVar6);
      QByteArray::~QByteArray((QByteArray *)local_168);
      uVar4 = thunk_FUN_1400876c0(&local_148,local_50);
      uVar7 = thunk_FUN_14039be80(local_168);
      local_178 = 0x8a;
      thunk_FUN_14039b070(uVar7,30000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp"
                         );
      thunk_FUN_14039a8a0(local_168);
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
        FUN_140002d6f(uVar8);
      }
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_140);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_138);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
    thunk_FUN_14018ec00(param_1);
  }
  return;
}

// =============================================================================
// Ghidra: FUN_1402478c0 @ 0x1402478c0
// =============================================================================
void FUN_1402478c0(undefined8 param_1,int param_2,undefined8 param_3,undefined8 param_4)

{
  void *pvVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  
  uVar5 = 0xfffffffffffffffe;
  if (param_2 < 0x12e) {
    if (param_2 == 0x12d) {
      puVar4 = operator_new(0x10);
      if (puVar4 == (undefined8 *)0x0) {
        puVar4 = (undefined8 *)0x0;
      }
      else {
        thunk_FUN_14018c0c0(puVar4);
        *puVar4 = grabberAgent::protocol::control::M2SKeyboardButton::vftable;
        *(undefined4 *)(puVar4 + 1) = 99;
        *(undefined4 *)((longlong)puVar4 + 0xc) = 0xb;
      }
      thunk_FUN_14023e100(param_1,puVar4);
      return;
    }
    switch(param_2) {
    case 1:
      puVar4 = operator_new(0x10);
      if (puVar4 == (undefined8 *)0x0) {
        puVar4 = (undefined8 *)0x0;
      }
      else {
        thunk_FUN_14018c0c0(puVar4);
        *puVar4 = grabberAgent::protocol::control::M2SCommand::vftable;
        *(undefined4 *)(puVar4 + 1) = 1;
      }
      thunk_FUN_14023dfb0(param_1,puVar4);
      break;
    case 2:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,1,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    default:
      goto switchD_14024791a_caseD_3;
    case 0x65:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x3b,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x66:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x3c,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x67:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x3d,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x68:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x3e,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x69:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x3f,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x6a:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x40,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x6b:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x41,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x6c:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x42,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x6d:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x43,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x6e:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x44,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x6f:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x57,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0x70:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x1d,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x58,0xb);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      uVar5 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar5 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar5);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x1d,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
      break;
    case 0xc9:
      pvVar1 = operator_new(0x10);
      uVar3 = 0;
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0x38,1,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      uVar2 = uVar3;
      if (pvVar1 != (void *)0x0) {
        uVar2 = thunk_FUN_140240290(pvVar1,0xf,0xb,param_4,uVar5);
      }
      thunk_FUN_14023e100(param_1,uVar2);
      pvVar1 = operator_new(0x10);
      if (pvVar1 != (void *)0x0) {
        uVar3 = thunk_FUN_140240290(pvVar1,0x38,2);
      }
      thunk_FUN_14023e100(param_1,uVar3);
    }
  }
  else {
switchD_14024791a_caseD_3:
    qt_assert("!\"unknown command for slave\"",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp",
              0x2fa);
  }
  return;
}

// =============================================================================
// Ghidra: FUN_140244ae0 @ 0x140244ae0
// =============================================================================
void FUN_140244ae0(longlong param_1,int param_2)

{
  char cVar1;
  bool bVar2;
  QMimeData *this;
  undefined8 uVar3;
  QString *pQVar4;
  void *pvVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  bool bVar9;
  undefined4 local_res18 [2];
  undefined8 *local_res20;
  void *local_58 [2];
  undefined8 local_48;
  QListData local_40 [8];
  QUrl *local_38;
  QUrl *local_30;
  uint local_28;
  
  local_48 = 0xfffffffffffffffe;
  puVar6 = (undefined8 *)0x0;
  local_res18[0] = 0;
  cVar1 = thunk_FUN_14018eed0();
  if (cVar1 == '\0') {
    return;
  }
  if ((*(byte *)(param_1 + 0x148) & 1) == 0) {
    return;
  }
  if (param_2 != 0) {
    return;
  }
  this = (QMimeData *)thunk_FUN_1404e4ee0(param_1 + 0x198,0);
  if (this == (QMimeData *)0x0) {
    return;
  }
  bVar2 = QMimeData::hasUrls(this);
  if (bVar2) {
    uVar3 = QMimeData::urls(this);
    thunk_FUN_140224e20(local_40,uVar3);
    local_38 = (QUrl *)QListData::begin(local_40);
    local_30 = (QUrl *)QListData::end(local_40);
    local_28 = 1;
    thunk_FUN_140225bc0(&local_res20);
    puVar8 = puVar6;
    for (; (local_28 != 0 && (local_38 != local_30)); local_38 = local_38 + 8) {
      puVar7 = (undefined8 *)(ulonglong)local_28;
      if (local_28 != 0) {
        pQVar4 = (QString *)QUrl::toLocalFile(local_38);
        QFileInfo::QFileInfo((QFileInfo *)local_res18,pQVar4);
        QString::~QString((QString *)local_58);
        bVar2 = QFileInfo::isDir((QFileInfo *)local_res18);
        if ((((bVar2) || (bVar2 = QFileInfo::isFile((QFileInfo *)local_res18), bVar2)) &&
            (bVar2 = QFileInfo::isRelative((QFileInfo *)local_res18), !bVar2)) &&
           (bVar2 = QFileInfo::isReadable((QFileInfo *)local_res18), bVar2)) {
          puVar8 = (undefined8 *)((longlong)puVar8 + 1);
        }
        QFileInfo::~QFileInfo((QFileInfo *)local_res18);
        puVar7 = puVar6;
      }
      local_28 = (uint)puVar7 ^ 1;
    }
    thunk_FUN_140225bc0(local_40);
    thunk_FUN_140249330(param_1,0);
    bVar9 = puVar8 != (undefined8 *)0x0;
LAB_140244e8f:
    thunk_FUN_140249260(param_1,bVar9);
  }
  else {
    bVar2 = QMimeData::hasHtml(this);
    if (bVar2) {
      thunk_FUN_140249260(param_1,0);
      pvVar5 = operator_new(0x10);
      local_58[0] = pvVar5;
      if (pvVar5 != (void *)0x0) {
        uVar3 = QMimeData::html(this);
        local_res18[0] = 1;
        puVar6 = (undefined8 *)thunk_FUN_14023fff0(pvVar5,uVar3);
      }
      puVar8 = (undefined8 *)(ulonglong)(pvVar5 != (void *)0x0);
      thunk_FUN_14023d3e0(param_1,puVar6);
    }
    else {
      bVar2 = QMimeData::hasText(this);
      if (!bVar2) {
        bVar2 = QMimeData::hasImage(this);
        bVar9 = false;
        if (bVar2) {
          thunk_FUN_140249260(param_1,0);
          uVar3 = QMimeData::imageData(this);
          thunk_FUN_14023cfd0(local_40,uVar3);
          QVariant::~QVariant((QVariant *)local_58);
          bVar2 = QImage::isNull((QImage *)local_40);
          if (!bVar2) {
            QByteArray::QByteArray((QByteArray *)local_res18);
            QBuffer::QBuffer((QBuffer *)local_58,(QByteArray *)local_res18,(QObject *)0x0);
            bVar2 = QBuffer::open((QBuffer *)local_58,2);
            if (!bVar2) {
              qt_assert("!\"wtf?\"",
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp"
                        ,0x37f);
                    /* WARNING: Subroutine does not return */
              abort();
            }
            bVar2 = QImage::save((QImage *)local_40,(QIODevice *)local_58,"PNG",-1);
            if (!bVar2) {
              qt_assert("!\"wtf?\"",
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp"
                        ,0x385);
                    /* WARNING: Subroutine does not return */
              abort();
            }
            QBuffer::close((QBuffer *)local_58);
            puVar8 = operator_new(0x10);
            local_res20 = puVar8;
            if (puVar8 != (undefined8 *)0x0) {
              thunk_FUN_14018c0c0(puVar8);
              *puVar8 = grabberAgent::protocol::control::ClipboardImage::vftable;
              QByteArray::QByteArray((QByteArray *)(puVar8 + 1),(QByteArray *)local_res18);
              puVar6 = puVar8;
            }
            thunk_FUN_14023d530(param_1,puVar6);
            QBuffer::~QBuffer((QBuffer *)local_58);
            QByteArray::~QByteArray((QByteArray *)local_res18);
          }
          QImage::~QImage((QImage *)local_40);
          return;
        }
        goto LAB_140244e8f;
      }
      thunk_FUN_140249260(param_1,0);
      pvVar5 = operator_new(0x10);
      puVar8 = puVar6;
      local_58[0] = pvVar5;
      if (pvVar5 != (void *)0x0) {
        uVar3 = QMimeData::text(this);
        local_res18[0] = 2;
        puVar6 = (undefined8 *)thunk_FUN_140240090(pvVar5,uVar3);
        puVar8 = (undefined8 *)0x2;
      }
      thunk_FUN_14023d680(param_1,puVar6);
    }
    if (puVar8 != (undefined8 *)0x0) {
      QString::~QString((QString *)&local_res20);
    }
  }
  return;
}

// =============================================================================
// Ghidra: FUN_140249540 @ 0x140249540
// =============================================================================
void FUN_140249540(longlong param_1,undefined4 param_2,int param_3,undefined8 param_4)

{
  char cVar1;
  undefined4 uVar2;
  void *pvVar3;
  undefined8 uVar4;
  undefined4 uVar5;
  undefined8 uVar6;
  
  uVar6 = 0xfffffffffffffffe;
  cVar1 = thunk_FUN_14018eed0();
  if ((cVar1 != '\0') && ((*(byte *)(param_1 + 0x144) & 1) != 0)) {
    uVar2 = thunk_FUN_1404e7a50(param_2);
    if (param_3 == 0) {
      uVar5 = 0xb;
    }
    else if (param_3 == 1) {
      uVar5 = 0xc;
    }
    else if (param_3 == 2) {
      uVar5 = 1;
    }
    else {
      if (param_3 != 3) {
        qt_assert("!\"unknown keyboard button action\"",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp"
                  ,0x26e);
        return;
      }
      uVar5 = 2;
    }
    pvVar3 = operator_new(0x10);
    uVar4 = 0;
    if (pvVar3 != (void *)0x0) {
      uVar4 = thunk_FUN_140240290(pvVar3,uVar2,uVar5,param_4,uVar6);
    }
    thunk_FUN_14023e100(param_1,uVar4);
  }
  return;
}

// =============================================================================
// Ghidra: FUN_140334cd0 @ 0x140334cd0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140334cd0(longlong param_1,longlong *param_2)

{
  QHashData *pQVar1;
  double dVar2;
  double dVar3;
  Node *pNVar4;
  QDateTime *pQVar5;
  double *pdVar6;
  char cVar7;
  bool bVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  longlong lVar13;
  double *pdVar14;
  undefined8 uVar15;
  QString *pQVar16;
  QByteArray *pQVar17;
  char *pcVar18;
  undefined8 uVar19;
  QImage *pQVar20;
  int *piVar21;
  uchar *puVar22;
  uint *puVar23;
  undefined4 *puVar24;
  QImage *pQVar25;
  undefined4 *puVar26;
  double *pdVar27;
  QDateTime *pQVar28;
  __int64 _Var29;
  QHashData *pQVar30;
  ulonglong uVar31;
  longlong *plVar32;
  int iVar33;
  longlong lVar34;
  QHashData *this;
  longlong lVar35;
  QHashData *pQVar36;
  undefined1 auStackY_f78 [32];
  QHashData *local_f30;
  QHashData *local_f28;
  QHashData *local_f20;
  Node *local_f18;
  Node *local_f10;
  Node *local_f08;
  uint local_f00;
  uint local_ef8 [2];
  QImage local_ef0 [32];
  QHashData *local_ed0;
  Node *local_ec8;
  QHashData *local_ec0;
  uint local_eb8;
  longlong local_eb0;
  undefined4 *local_ea8;
  QByteArray local_ea0 [8];
  QString local_e98 [8];
  QByteArray local_e90 [8];
  QString local_e88 [8];
  QString local_e80 [8];
  QString local_e78 [8];
  QByteArray local_e70 [8];
  QString local_e68 [8];
  QByteArray local_e60 [8];
  QString local_e58 [8];
  QByteArray local_e50 [8];
  QString local_e48 [8];
  QByteArray local_e40 [8];
  QString local_e38 [8];
  QString local_e30 [8];
  QString local_e28 [8];
  QDateTime *local_e20;
  double *local_e18;
  longlong local_e10;
  undefined1 local_e08 [16];
  undefined1 local_df8 [16];
  undefined1 local_de8 [16];
  undefined1 local_dd8 [16];
  undefined1 local_dc8 [16];
  undefined1 local_db8 [16];
  undefined1 local_da8 [16];
  undefined1 local_d98 [16];
  undefined1 local_d88 [16];
  undefined1 local_d78 [16];
  undefined1 local_d68 [16];
  undefined1 local_d58 [16];
  undefined1 local_d48 [16];
  undefined1 local_d38 [16];
  undefined1 local_d28 [16];
  undefined1 local_d18 [16];
  undefined1 local_d08 [16];
  undefined1 local_cf8 [16];
  undefined8 local_ce8;
  undefined1 local_ce0 [16];
  undefined1 local_cd0 [16];
  QImage local_cc0 [32];
  QImage local_ca0 [32];
  QImage local_c80 [32];
  QImage local_c60 [32];
  QImage local_c40 [32];
  QImage local_c20 [32];
  QImage local_c00 [32];
  QImage local_be0 [32];
  QImage local_bc0 [32];
  QImage local_ba0 [32];
  QImage local_b80 [32];
  QImage local_b60 [32];
  QImage local_b40 [36];
  int iStack_b1c;
  undefined *local_b18;
  basic_streambuf<char,std::char_traits<char>_> local_b10 [128];
  basic_ios<char,std::char_traits<char>_> local_a90 [100];
  int iStack_a2c;
  undefined *local_a28;
  basic_streambuf<char,std::char_traits<char>_> local_a20 [128];
  basic_ios<char,std::char_traits<char>_> local_9a0 [100];
  int iStack_93c;
  undefined *local_938;
  basic_streambuf<char,std::char_traits<char>_> local_930 [128];
  basic_ios<char,std::char_traits<char>_> local_8b0 [100];
  int iStack_84c;
  undefined *local_848;
  basic_streambuf<char,std::char_traits<char>_> local_840 [128];
  basic_ios<char,std::char_traits<char>_> local_7c0 [100];
  int iStack_75c;
  undefined *local_758;
  basic_streambuf<char,std::char_traits<char>_> local_750 [128];
  basic_ios<char,std::char_traits<char>_> local_6d0 [100];
  int iStack_66c;
  undefined *local_668;
  basic_streambuf<char,std::char_traits<char>_> local_660 [128];
  basic_ios<char,std::char_traits<char>_> local_5e0 [100];
  int iStack_57c;
  undefined *local_578;
  basic_streambuf<char,std::char_traits<char>_> local_570 [128];
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
  ulonglong local_138 [3];
  ulonglong local_120;
  ulonglong local_118 [3];
  ulonglong local_100;
  undefined1 local_f8 [32];
  undefined1 local_d8 [32];
  undefined1 local_b8 [32];
  undefined1 local_98 [32];
  ulonglong local_78;
  
  local_ce8 = 0xfffffffffffffffe;
  local_78 = DAT_140e88880 ^ (ulonglong)auStackY_f78;
  iVar33 = 0;
  local_e10 = param_1;
  lVar13 = thunk_FUN_1403316a0(*(undefined8 *)(param_1 + 0x1b8),*param_2 + 8);
  if (lVar13 == 0) {
    return;
  }
  pdVar14 = (double *)thunk_FUN_14032c2c0(param_1 + 0x1b8,*param_2 + 8);
  iVar9 = *(int *)(*param_2 + 0x38);
  local_e18 = pdVar14;
  if (iVar9 == 0) {
    bVar8 = QListData::isEmpty((QListData *)(*param_2 + 0x50));
    if (bVar8) {
      uVar15 = thunk_FUN_14039be80(local_d48);
      cVar7 = thunk_FUN_14039ba40(uVar15,40000);
      thunk_FUN_14039a8a0(local_d48);
      if (cVar7 != '\0') {
        local_a28 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_9a0);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_a28,local_a20,false);
        *(undefined ***)(local_a20 + (longlong)*(int *)(local_a28 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_a2c + (longlong)*(int *)(local_a28 + 4)) =
             *(int *)(local_a28 + 4) + -0x88;
        thunk_FUN_140076630(local_a20,2);
        pQVar16 = (QString *)
                  (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e38);
        uVar15 = thunk_FUN_14006fb70(&local_a28,
                                     "full frame contains no image, skip until next streamPos: ");
        pQVar17 = (QByteArray *)QString::toLocal8Bit(pQVar16);
        pcVar18 = QByteArray::data(pQVar17);
        thunk_FUN_14006fb70(uVar15,pcVar18);
        QByteArray::~QByteArray(local_e40);
        QString::~QString(local_e38);
        uVar15 = thunk_FUN_1400876c0(&local_a28,local_158);
        uVar19 = thunk_FUN_14039be80(local_d38);
        thunk_FUN_14039b070(uVar19,40000,uVar15,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                           );
        thunk_FUN_14039a8a0(local_d38);
        if (0xf < local_140) {
          uVar31 = local_158[0];
          if (0xfff < local_140 + 1) {
            if ((local_158[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar31 = *(ulonglong *)(local_158[0] - 8);
            if (local_158[0] <= uVar31) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_158[0] - uVar31 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_158[0] - uVar31) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar31);
        }
        thunk_FUN_140077af0(local_9a0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_9a0);
      }
      pQVar20 = (QImage *)QImage::QImage(local_cc0);
      QImage::operator=((QImage *)(pdVar14 + 2),pQVar20);
      pQVar20 = local_cc0;
    }
    else {
      piVar21 = (int *)thunk_FUN_140250010(*param_2 + 0x50,0);
      cVar7 = thunk_FUN_1400e9150(piVar21);
      if (cVar7 == '\0') {
        uVar15 = thunk_FUN_14039be80(local_d28);
        cVar7 = thunk_FUN_14039ba40(uVar15,40000);
        thunk_FUN_14039a8a0(local_d28);
        if (cVar7 != '\0') {
          local_938 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_8b0);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_938,local_930,false);
          *(undefined ***)(local_930 + (longlong)*(int *)(local_938 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_93c + (longlong)*(int *)(local_938 + 4)) =
               *(int *)(local_938 + 4) + -0x88;
          thunk_FUN_140076630(local_930,2);
          uVar15 = (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e30);
          uVar19 = thunk_FUN_14006fb70(&local_938,
                                       "invalid full frame image, skip until next streamPos: ");
          thunk_FUN_140078de0(uVar19,uVar15);
          QString::~QString(local_e30);
          uVar15 = thunk_FUN_1400876c0(&local_938,local_d8);
          uVar19 = thunk_FUN_14039be80(local_d18);
          thunk_FUN_14039b070(uVar19,40000,uVar15,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                             );
          thunk_FUN_14039a8a0(local_d18);
          thunk_FUN_140077b90(local_d8);
          thunk_FUN_140077af0(local_8b0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_8b0);
        }
        pQVar20 = (QImage *)QImage::QImage(local_b60);
        QImage::operator=((QImage *)(pdVar14 + 2),pQVar20);
        pQVar20 = local_b60;
      }
      else {
        puVar22 = (uchar *)QByteArray::constData((QByteArray *)(piVar21 + 4));
        pQVar25 = (QImage *)
                  QImage::QImage(local_ba0,puVar22,*piVar21,piVar21[1],piVar21[2],
                                 (_func_void_void_ptr *)0x0,(void *)0x0);
        pQVar20 = (QImage *)(pdVar14 + 2);
        QImage::operator=(pQVar20,pQVar25);
        QImage::~QImage(local_ba0);
        bVar8 = QImage::isNull(pQVar20);
        if ((((bVar8) || (iVar33 = QImage::width(pQVar20), iVar33 < 1)) ||
            (iVar33 = QImage::height(pQVar20), iVar33 < 1)) ||
           (puVar22 = QImage::constBits(pQVar20), puVar22 == (uchar *)0x0)) {
          uVar15 = thunk_FUN_14039be80(local_d08);
          cVar7 = thunk_FUN_14039ba40(uVar15,40000);
          thunk_FUN_14039a8a0(local_d08);
          if (cVar7 != '\0') {
            local_848 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_7c0);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_848,local_840,false);
            *(undefined ***)(local_840 + (longlong)*(int *)(local_848 + 4) + -8) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_84c + (longlong)*(int *)(local_848 + 4)) =
                 *(int *)(local_848 + 4) + -0x88;
            thunk_FUN_140076630(local_840,2);
            uVar15 = (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e28);
            uVar19 = thunk_FUN_14006fb70(&local_848,
                                         "corrupted full frame received, skip until next streamPos: "
                                        );
            thunk_FUN_140078de0(uVar19,uVar15);
            QString::~QString(local_e28);
            uVar15 = thunk_FUN_1400876c0(&local_848,local_f8);
            uVar19 = thunk_FUN_14039be80(local_cf8);
            thunk_FUN_14039b070(uVar19,40000,uVar15,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                               );
            thunk_FUN_14039a8a0(local_cf8);
            thunk_FUN_140077b90(local_f8);
            thunk_FUN_140077af0(local_7c0);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_7c0);
          }
          pQVar25 = (QImage *)QImage::QImage(local_c40);
          QImage::operator=(pQVar20,pQVar25);
          pQVar20 = local_c40;
        }
        else {
          pQVar25 = (QImage *)thunk_FUN_140339170(local_c20,pQVar20,1);
          QImage::operator=(pQVar20,pQVar25);
          pQVar20 = local_c20;
        }
      }
    }
  }
  else if (iVar9 == 1) {
    pQVar20 = (QImage *)(pdVar14 + 2);
    bVar8 = QImage::isNull(pQVar20);
    if (bVar8) {
      uVar15 = thunk_FUN_14039be80(local_e08);
      cVar7 = thunk_FUN_14039ba40(uVar15,40000);
      thunk_FUN_14039a8a0(local_e08);
      if (cVar7 != '\0') {
        local_668 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_5e0);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_668,local_660,false);
        *(undefined ***)(local_660 + (longlong)*(int *)(local_668 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_66c + (longlong)*(int *)(local_668 + 4)) =
             *(int *)(local_668 + 4) + -0x88;
        thunk_FUN_140076630(local_660,2);
        pQVar16 = (QString *)
                  (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e88);
        uVar15 = thunk_FUN_14006fb70(&local_668,
                                     "delta frame received with no support frame, skip until next full frame: "
                                    );
        pQVar17 = (QByteArray *)QString::toLocal8Bit(pQVar16);
        pcVar18 = QByteArray::data(pQVar17);
        thunk_FUN_14006fb70(uVar15,pcVar18);
        QByteArray::~QByteArray(local_e90);
        QString::~QString(local_e88);
        uVar15 = thunk_FUN_1400876c0(&local_668,local_1b8);
        uVar19 = thunk_FUN_14039be80(local_df8);
        thunk_FUN_14039b070(uVar19,40000,uVar15,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                           );
        thunk_FUN_14039a8a0(local_df8);
        if (0xf < local_1a0) {
          uVar31 = local_1b8[0];
          if (0xfff < local_1a0 + 1) {
            if ((local_1b8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar31 = *(ulonglong *)(local_1b8[0] - 8);
            if (local_1b8[0] <= uVar31) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_1b8[0] - uVar31 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_1b8[0] - uVar31) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar31);
        }
        thunk_FUN_140077af0(local_5e0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_5e0);
      }
      goto LAB_140336307;
    }
    iVar9 = QListData::size((QListData *)(*param_2 + 0x50));
    iVar10 = QListData::size((QListData *)(*param_2 + 0x40));
    if (iVar9 == iVar10) {
      iVar9 = QListData::size((QListData *)(*param_2 + 0x50));
      iVar10 = QListData::size((QListData *)(*param_2 + 0x48));
      if (iVar9 == iVar10) {
        iVar9 = QListData::size((QListData *)(*param_2 + 0x50));
        if (0 < iVar9) {
          do {
            piVar21 = (int *)thunk_FUN_140250010(*param_2 + 0x50,iVar33);
            cVar7 = thunk_FUN_1400e9150(piVar21);
            if (cVar7 == '\0') {
              uVar15 = thunk_FUN_14039be80(local_d88);
              cVar7 = thunk_FUN_14039ba40(uVar15,40000);
              thunk_FUN_14039a8a0(local_d88);
              if (cVar7 != '\0') {
                local_2a8 = &DAT_1405a3878;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_220);
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_2a8,local_2a0,false
                          );
                *(undefined ***)(local_2a0 + (longlong)*(int *)(local_2a8 + 4) + -8) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
                     *(int *)(local_2a8 + 4) + -0x88;
                thunk_FUN_140076630(local_2a0,2);
                pQVar16 = (QString *)
                          (**(code **)(*(longlong *)*param_2 + 0x18))
                                    ((longlong *)*param_2,local_e58);
                uVar15 = thunk_FUN_14006fb70(&local_2a8,
                                             "invalid delta subframe image, skip until next full frame: "
                                            );
                pQVar17 = (QByteArray *)QString::toLocal8Bit(pQVar16);
                pcVar18 = QByteArray::data(pQVar17);
                thunk_FUN_14006fb70(uVar15,pcVar18);
                QByteArray::~QByteArray(local_e60);
                QString::~QString(local_e58);
                uVar15 = thunk_FUN_1400876c0(&local_2a8,local_178);
                uVar19 = thunk_FUN_14039be80(local_d78);
                thunk_FUN_14039b070(uVar19,40000,uVar15,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                                   );
                thunk_FUN_14039a8a0(local_d78);
                if (0xf < local_160) {
                  uVar31 = local_178[0];
                  if (0xfff < local_160 + 1) {
                    if ((local_178[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    uVar31 = *(ulonglong *)(local_178[0] - 8);
                    if (local_178[0] <= uVar31) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    if (local_178[0] - uVar31 < 8) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    if (0x27 < local_178[0] - uVar31) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                  }
                  FUN_140002d6f(uVar31);
                }
                thunk_FUN_140077af0(local_220);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_220);
              }
              pQVar25 = (QImage *)QImage::QImage(local_c80);
              QImage::operator=(pQVar20,pQVar25);
              pQVar20 = local_c80;
              goto LAB_140336301;
            }
            puVar22 = (uchar *)QByteArray::constData((QByteArray *)(piVar21 + 4));
            uVar15 = QImage::QImage(local_bc0,puVar22,*piVar21,piVar21[1],piVar21[2],
                                    (_func_void_void_ptr *)0x0,(void *)0x0);
            thunk_FUN_140339170(local_ef0,uVar15);
            QImage::~QImage(local_bc0);
            bVar8 = QImage::isNull(local_ef0);
            if (((bVar8) || (iVar9 = QImage::width(local_ef0), iVar9 < 1)) ||
               ((iVar9 = QImage::height(local_ef0), iVar9 < 1 ||
                (puVar22 = QImage::constBits(local_ef0), puVar22 == (uchar *)0x0)))) {
              uVar15 = thunk_FUN_14039be80(local_da8);
              cVar7 = thunk_FUN_14039ba40(uVar15,40000);
              thunk_FUN_14039a8a0(local_da8);
              if (cVar7 != '\0') {
                local_398 = &DAT_1405a3878;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_310);
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_398,local_390,false
                          );
                *(undefined ***)(local_390 + (longlong)*(int *)(local_398 + 4) + -8) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_39c + (longlong)*(int *)(local_398 + 4)) =
                     *(int *)(local_398 + 4) + -0x88;
                thunk_FUN_140076630(local_390,2);
                pQVar16 = (QString *)
                          (**(code **)(*(longlong *)*param_2 + 0x18))
                                    ((longlong *)*param_2,local_e68);
                uVar15 = thunk_FUN_14006fb70(&local_398,
                                             "corrupted delta subframe received, skip until next full frame: "
                                            );
                pQVar17 = (QByteArray *)QString::toLocal8Bit(pQVar16);
                pcVar18 = QByteArray::data(pQVar17);
                thunk_FUN_14006fb70(uVar15,pcVar18);
                QByteArray::~QByteArray(local_e70);
                QString::~QString(local_e68);
                uVar15 = thunk_FUN_1400876c0(&local_398,local_198);
                uVar19 = thunk_FUN_14039be80(local_d98);
                thunk_FUN_14039b070(uVar19,40000,uVar15,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                                   );
                thunk_FUN_14039a8a0(local_d98);
                if (0xf < local_180) {
                  uVar31 = local_198[0];
                  if (0xfff < local_180 + 1) {
                    if ((local_198[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    uVar31 = *(ulonglong *)(local_198[0] - 8);
                    if (local_198[0] <= uVar31) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    if (local_198[0] - uVar31 < 8) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    if (0x27 < local_198[0] - uVar31) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                  }
                  FUN_140002d6f(uVar31);
                }
                thunk_FUN_140077af0(local_310);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_310);
              }
              pQVar25 = (QImage *)QImage::QImage(local_ca0);
              QImage::operator=(pQVar20,pQVar25);
              QImage::~QImage(local_ca0);
              pQVar20 = local_ef0;
              goto LAB_140336301;
            }
            iVar9 = QImage::width(pQVar20);
            if ((iVar9 < 1) || (iVar9 = QImage::height(pQVar20), iVar9 < 1)) {
LAB_14033556b:
              uVar15 = thunk_FUN_14039be80(local_dc8);
              cVar7 = thunk_FUN_14039ba40(uVar15,40000);
              thunk_FUN_14039a8a0(local_dc8);
              if (cVar7 != '\0') {
                local_488 = &DAT_1405a3878;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_400);
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_488,local_480,false
                          );
                *(undefined ***)(local_480 + (longlong)*(int *)(local_488 + 4) + -8) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_48c + (longlong)*(int *)(local_488 + 4)) =
                     *(int *)(local_488 + 4) + -0x88;
                thunk_FUN_140076630(local_480,2);
                uVar15 = (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e78)
                ;
                uVar19 = thunk_FUN_14006fb70(&local_488,
                                             "delta subframe is out of bounds, skip until next full frame: "
                                            );
                thunk_FUN_140078de0(uVar19,uVar15);
                QString::~QString(local_e78);
                uVar15 = thunk_FUN_1400876c0(&local_488,local_b8);
                uVar19 = thunk_FUN_14039be80(local_db8);
                thunk_FUN_14039b070(uVar19,40000,uVar15,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                                   );
                thunk_FUN_14039a8a0(local_db8);
                thunk_FUN_140077b90(local_b8);
                thunk_FUN_140077af0(local_400);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_400);
              }
              pQVar25 = (QImage *)QImage::QImage(local_b40);
              QImage::operator=(pQVar20,pQVar25);
              QImage::~QImage(local_b40);
              pQVar20 = local_ef0;
              goto LAB_140336301;
            }
            puVar23 = (uint *)thunk_FUN_1402262d0(*param_2 + 0x40,iVar33);
            uVar12 = *puVar23;
            uVar11 = QImage::width(pQVar20);
            if (uVar11 <= uVar12) goto LAB_14033556b;
            puVar23 = (uint *)thunk_FUN_1402262d0(*param_2 + 0x48,iVar33);
            uVar12 = *puVar23;
            uVar11 = QImage::height(pQVar20);
            if (uVar11 <= uVar12) goto LAB_14033556b;
            piVar21 = (int *)thunk_FUN_1402262d0(*param_2 + 0x40,iVar33);
            iVar9 = *piVar21;
            iVar10 = QImage::width(pQVar20);
            uVar12 = QImage::width(local_ef0);
            if ((uint)(iVar10 - iVar9) < uVar12) goto LAB_14033556b;
            piVar21 = (int *)thunk_FUN_1402262d0(*param_2 + 0x48,iVar33);
            iVar9 = *piVar21;
            iVar10 = QImage::height(pQVar20);
            uVar12 = QImage::height(local_ef0);
            if ((uint)(iVar10 - iVar9) < uVar12) goto LAB_14033556b;
            lVar13 = *param_2;
            puVar26 = (undefined4 *)thunk_FUN_1402262d0(lVar13 + 0x48,iVar33);
            puVar24 = (undefined4 *)thunk_FUN_1402262d0(lVar13 + 0x40,iVar33);
            cVar7 = thunk_FUN_1402522d0(pQVar20,*puVar24,*puVar26,local_ef0);
            if (cVar7 == '\0') {
              uVar15 = thunk_FUN_14039be80(local_de8);
              cVar7 = thunk_FUN_14039ba40(uVar15,40000);
              thunk_FUN_14039a8a0(local_de8);
              if (cVar7 != '\0') {
                local_578 = &DAT_1405a3878;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_4f0);
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_578,local_570,false
                          );
                *(undefined ***)(local_570 + (longlong)*(int *)(local_578 + 4) + -8) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_57c + (longlong)*(int *)(local_578 + 4)) =
                     *(int *)(local_578 + 4) + -0x88;
                thunk_FUN_140076630(local_570,2);
                uVar15 = (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e80)
                ;
                uVar19 = thunk_FUN_14006fb70(&local_578,
                                             "unable to combine delta subframe, skip until next full frame: "
                                            );
                thunk_FUN_140078de0(uVar19,uVar15);
                QString::~QString(local_e80);
                uVar15 = thunk_FUN_1400876c0(&local_578,local_98);
                uVar19 = thunk_FUN_14039be80(local_dd8);
                thunk_FUN_14039b070(uVar19,40000,uVar15,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                                   );
                thunk_FUN_14039a8a0(local_dd8);
                thunk_FUN_140077b90(local_98);
                thunk_FUN_140077af0(local_4f0);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_4f0);
              }
              pQVar25 = (QImage *)QImage::QImage(local_b80);
              QImage::operator=(pQVar20,pQVar25);
              QImage::~QImage(local_b80);
              pQVar20 = local_ef0;
              goto LAB_140336301;
            }
            QImage::~QImage(local_ef0);
            iVar33 = iVar33 + 1;
            iVar9 = QListData::size((QListData *)(*param_2 + 0x50));
          } while (iVar33 < iVar9);
        }
        goto LAB_140336307;
      }
    }
    uVar15 = thunk_FUN_14039be80(local_d68);
    cVar7 = thunk_FUN_14039ba40(uVar15,40000);
    thunk_FUN_14039a8a0(local_d68);
    if (cVar7 != '\0') {
      local_b18 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_a90);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_b18,local_b10,false);
      *(undefined ***)(local_b10 + (longlong)*(int *)(local_b18 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_b1c + (longlong)*(int *)(local_b18 + 4)) =
           *(int *)(local_b18 + 4) + -0x88;
      thunk_FUN_140076630(local_b10,2);
      pQVar16 = (QString *)
                (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e48);
      uVar15 = thunk_FUN_14006fb70(&local_b18,
                                   "invalid delta frame vectors, skip until next full frame: ");
      pQVar17 = (QByteArray *)QString::toLocal8Bit(pQVar16);
      pcVar18 = QByteArray::data(pQVar17);
      thunk_FUN_14006fb70(uVar15,pcVar18);
      QByteArray::~QByteArray(local_e50);
      QString::~QString(local_e48);
      uVar15 = thunk_FUN_1400876c0(&local_b18,local_138);
      uVar19 = thunk_FUN_14039be80(local_d58);
      thunk_FUN_14039b070(uVar19,40000,uVar15,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                         );
      thunk_FUN_14039a8a0(local_d58);
      if (0xf < local_120) {
        uVar31 = local_138[0];
        if (0xfff < local_120 + 1) {
          if ((local_138[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar31 = *(ulonglong *)(local_138[0] - 8);
          if (local_138[0] <= uVar31) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_138[0] - uVar31 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_138[0] - uVar31) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar31);
      }
      thunk_FUN_140077af0(local_a90);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_a90);
    }
    pQVar25 = (QImage *)QImage::QImage(local_c60);
    QImage::operator=(pQVar20,pQVar25);
    pQVar20 = local_c60;
  }
  else if (iVar9 == 2) {
    pQVar20 = (QImage *)QImage::QImage(local_be0);
    QImage::operator=((QImage *)(pdVar14 + 2),pQVar20);
    pQVar20 = local_be0;
  }
  else {
    uVar15 = thunk_FUN_14039be80(local_ce0);
    cVar7 = thunk_FUN_14039ba40(uVar15,40000);
    thunk_FUN_14039a8a0(local_ce0);
    if (cVar7 != '\0') {
      local_758 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_6d0);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_758,local_750,false);
      *(undefined ***)(local_750 + (longlong)*(int *)(local_758 + 4) + -8) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_75c + (longlong)*(int *)(local_758 + 4)) =
           *(int *)(local_758 + 4) + -0x88;
      thunk_FUN_140076630(local_750,2);
      pQVar16 = (QString *)
                (**(code **)(*(longlong *)*param_2 + 0x18))((longlong *)*param_2,local_e98);
      uVar15 = thunk_FUN_14006fb70(&local_758,"unknown frame status, skip until next full frame: ");
      pQVar17 = (QByteArray *)QString::toLocal8Bit(pQVar16);
      pcVar18 = QByteArray::data(pQVar17);
      thunk_FUN_14006fb70(uVar15,pcVar18);
      QByteArray::~QByteArray(local_ea0);
      QString::~QString(local_e98);
      uVar15 = thunk_FUN_1400876c0(&local_758,local_118);
      uVar19 = thunk_FUN_14039be80(local_cd0);
      thunk_FUN_14039b070(uVar19,40000,uVar15,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
                         );
      thunk_FUN_14039a8a0(local_cd0);
      if (0xf < local_100) {
        uVar31 = local_118[0];
        if (0xfff < local_100 + 1) {
          if ((local_118[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar31 = *(ulonglong *)(local_118[0] - 8);
          if (local_118[0] <= uVar31) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_118[0] - uVar31 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_118[0] - uVar31) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar31);
      }
      thunk_FUN_140077af0(local_6d0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_6d0);
    }
    pQVar20 = (QImage *)QImage::QImage(local_c00);
    QImage::operator=((QImage *)(pdVar14 + 2),pQVar20);
    pQVar20 = local_c00;
  }
LAB_140336301:
  QImage::~QImage(pQVar20);
LAB_140336307:
  local_e20 = (QDateTime *)(pdVar14 + 1);
  QDateTime::operator=(local_e20,(QDateTime *)(*param_2 + 0x30));
  local_f28 = operator_new(0x38);
  if (local_f28 == (QHashData *)0x0) {
    lVar13 = 0;
  }
  else {
    lVar13 = thunk_FUN_14032a4c0(local_f28);
  }
  local_eb0 = lVar13;
  if (lVar13 == 0) {
    puVar26 = (undefined4 *)0x0;
    local_ea8 = (undefined4 *)0x0;
  }
  else {
    puVar26 = operator_new(0x18);
    local_f28 = (QHashData *)(puVar26 + 4);
    if (local_f28 != (QHashData *)0x0) {
      *(longlong *)local_f28 = lVar13;
    }
    if (puVar26 != (undefined4 *)0x0) {
      *(undefined1 **)(puVar26 + 2) = &LAB_140021891;
      puVar26[1] = 1;
      *puVar26 = 1;
    }
    local_ea8 = puVar26;
    thunk_FUN_1400871f0(puVar26,lVar13,1);
    FUN_1400045fc(&local_eb0,lVar13);
  }
  lVar35 = *param_2;
  *(undefined8 *)(lVar13 + 8) = *(undefined8 *)(lVar35 + 8);
  QString::operator=((QString *)(lVar13 + 0x10),(QString *)(lVar35 + 0x10));
  QString::operator=((QString *)(lVar13 + 0x18),(QString *)(lVar35 + 0x18));
  *(undefined4 *)(lVar13 + 0x20) = *(undefined4 *)(lVar35 + 0x20);
  *(undefined4 *)(lVar13 + 0x24) = *(undefined4 *)(lVar35 + 0x24);
  *(undefined1 *)(lVar13 + 0x28) = *(undefined1 *)(lVar35 + 0x28);
  QDateTime::operator=((QDateTime *)(lVar13 + 0x30),(QDateTime *)(*param_2 + 0x30));
  thunk_FUN_140329180(param_1,&local_eb0);
  thunk_FUN_14032f330(puVar26);
  lVar13 = *param_2;
  plVar32 = (longlong *)(param_1 + 0x1c8);
  if (1 < *(uint *)*plVar32) {
    thunk_FUN_140330280(plVar32);
  }
  lVar13 = thunk_FUN_140331880(*plVar32,lVar13 + 8);
  if (lVar13 == 0) {
    lVar13 = *plVar32 + 8;
  }
  if (1 < *(uint *)*plVar32) {
    thunk_FUN_140330280(plVar32);
  }
  if (*plVar32 + 8 == lVar13) {
    return;
  }
  local_f18 = *(Node **)(lVar13 + 0x40);
  if (1 < *(int *)(local_f18 + 0x10) + 1U) {
    LOCK();
    *(int *)(local_f18 + 0x10) = *(int *)(local_f18 + 0x10) + 1;
    UNLOCK();
  }
  if ((((byte)local_f18[0x28] & 1) == 0) && (1 < *(uint *)(local_f18 + 0x10))) {
    thunk_FUN_14032fde0(&local_f18);
  }
  dVar3 = DAT_140bfeef8;
  dVar2 = DAT_140bf19c0;
  plVar32 = *(longlong **)(local_f18 + 8);
  iVar33 = *(int *)(local_f18 + 0x20);
  while (local_f10 = local_f18, iVar33 != 0) {
    iVar33 = iVar33 + -1;
    if ((Node *)*plVar32 != local_f18) {
      local_f10 = (Node *)*plVar32;
      break;
    }
    plVar32 = plVar32 + 1;
  }
  local_f08 = local_f18;
  local_f00 = 1;
  do {
    pNVar4 = local_f10;
    if (local_f10 == local_f18) break;
    if (local_f00 != 0) {
      plVar32 = (longlong *)(param_1 + 0x1b0);
      if (1 < *(uint *)*plVar32) {
        thunk_FUN_1403301b0(plVar32);
      }
      if (*(longlong *)(*plVar32 + 0x10) == 0) {
LAB_140336579:
        lVar35 = *plVar32 + 8;
      }
      else {
        lVar35 = 0;
        lVar13 = *(longlong *)(*plVar32 + 0x10);
        do {
          cVar7 = thunk_FUN_140250490(lVar13 + 0x18,pNVar4 + 0x10);
          if (cVar7 == '\0') {
            lVar34 = *(longlong *)(lVar13 + 8);
            lVar35 = lVar13;
          }
          else {
            lVar34 = *(longlong *)(lVar13 + 0x10);
          }
          lVar13 = lVar34;
        } while (lVar34 != 0);
        if ((lVar35 == 0) ||
           (cVar7 = thunk_FUN_140250490(pNVar4 + 0x10,lVar35 + 0x18), cVar7 != '\0'))
        goto LAB_140336579;
      }
      if (1 < *(uint *)*plVar32) {
        thunk_FUN_1403301b0(plVar32);
      }
      if (*plVar32 + 8 != lVar35) {
        local_f30 = *(QHashData **)(lVar35 + 0x40);
        pQVar36 = local_f30 + 0x10;
        if (1 < *(uint *)pQVar36 + 1) {
          LOCK();
          *(uint *)pQVar36 = *(uint *)pQVar36 + 1;
          UNLOCK();
        }
        local_f20 = local_f30;
        if ((((byte)local_f30[0x28] & 1) == 0) && (1 < *(uint *)pQVar36)) {
          thunk_FUN_14032fd50(&local_f30);
          local_f20 = local_f30;
        }
        local_f28 = local_f20 + 0x10;
        if (1 < *(uint *)local_f28 + 1) {
          LOCK();
          *(uint *)local_f28 = *(uint *)local_f28 + 1;
          UNLOCK();
        }
        local_f30 = local_f20;
        local_ed0 = local_f20;
        if ((((byte)local_f20[0x28] & 1) == 0) && (1 < *(uint *)local_f28)) {
          thunk_FUN_14032fd50(&local_ed0);
        }
        pdVar6 = local_e18;
        pQVar5 = local_e20;
        pQVar36 = local_ed0;
        plVar32 = *(longlong **)(local_ed0 + 8);
        iVar33 = *(int *)(local_ed0 + 0x20);
        while (pQVar30 = local_ed0, iVar33 != 0) {
          iVar33 = iVar33 + -1;
          if ((QHashData *)*plVar32 != local_ed0) {
            pQVar30 = (QHashData *)*plVar32;
            break;
          }
          plVar32 = plVar32 + 1;
        }
        local_eb8 = 1;
        local_ec0 = local_ed0;
        do {
          local_ec8 = (Node *)pQVar30;
          if (pQVar30 == pQVar36) break;
          uVar15 = *(undefined8 *)(pQVar30 + 0x10);
          if (local_eb8 != 0) {
            pdVar27 = (double *)thunk_FUN_1402f1fa0(uVar15);
            if (*pdVar27 == *pdVar6) {
LAB_1403366e5:
              bVar8 = QImage::isNull((QImage *)(pdVar14 + 2));
              local_ef8[0] = (uint)!bVar8;
              thunk_FUN_1402f3780(uVar15,local_ef8);
              thunk_FUN_1402f3710(uVar15,pQVar5);
              thunk_FUN_1402f34c0(uVar15,(QImage *)(pdVar14 + 2),0);
            }
            else {
              pQVar28 = (QDateTime *)thunk_FUN_1402f2060(uVar15);
              bVar8 = QDateTime::isNull(pQVar28);
              if (bVar8) goto LAB_1403366e5;
              pQVar28 = (QDateTime *)thunk_FUN_1402f2060(uVar15);
              _Var29 = QDateTime::msecsTo(pQVar28,pQVar5);
              pdVar27 = (double *)thunk_FUN_1402f1fa0(uVar15);
              if ((dVar2 / *pdVar27) * dVar3 < (double)_Var29) goto LAB_1403366e5;
            }
            local_eb8 = 0;
          }
          uVar12 = local_eb8;
          pQVar30 = (QHashData *)QHashData::nextNode((Node *)pQVar30);
          local_eb8 = uVar12 ^ 1;
          local_ec8 = (Node *)pQVar30;
        } while (local_eb8 != 0);
        pQVar30 = local_f28;
        this = local_f20;
        if (*(int *)(pQVar36 + 0x10) == 0) {
LAB_14033676b:
          QHashData::free_helper(pQVar36,thunk_FUN_14032f1a0);
        }
        else if (*(int *)(pQVar36 + 0x10) != -1) {
          LOCK();
          pQVar1 = pQVar36 + 0x10;
          *(int *)pQVar1 = *(int *)pQVar1 + -1;
          UNLOCK();
          this = local_f30;
          pQVar36 = local_ed0;
          if (*(int *)pQVar1 == 0) goto LAB_14033676b;
        }
        if (*(uint *)pQVar30 == 0) {
LAB_14033679b:
          QHashData::free_helper(this,thunk_FUN_14032f1a0);
          param_1 = local_e10;
        }
        else {
          param_1 = local_e10;
          if (*(uint *)pQVar30 != 0xffffffff) {
            LOCK();
            *(uint *)pQVar30 = *(uint *)pQVar30 - 1;
            UNLOCK();
            this = local_f30;
            if (*(uint *)pQVar30 == 0) goto LAB_14033679b;
          }
        }
      }
      local_f00 = 0;
    }
    uVar12 = local_f00;
    local_f10 = QHashData::nextNode(local_f10);
    local_f00 = uVar12 ^ 1;
  } while (local_f00 != 0);
  if (*(int *)(local_f18 + 0x10) != 0) {
    if (*(int *)(local_f18 + 0x10) == -1) {
      return;
    }
    LOCK();
    pQVar36 = (QHashData *)(local_f18 + 0x10);
    *(int *)pQVar36 = *(int *)pQVar36 + -1;
    UNLOCK();
    if (*(int *)pQVar36 != 0) {
      return;
    }
  }
  QHashData::free_helper((QHashData *)local_f18,thunk_FUN_14032f1b0);
  return;
}

// =============================================================================
// Ghidra: FUN_140324090 @ 0x140324090
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140324090(longlong param_1)

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
  local_28 = DAT_140e88880 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (*(char *)(param_1 + 0x1a8) != '\0') {
    uVar2 = thunk_FUN_14039be80(local_150);
    cVar1 = thunk_FUN_14039ba40(uVar2,20000);
    thunk_FUN_14039a8a0(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_1405a3878;
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
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14006fb70(&local_138,"info connection closed for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1a0));
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar3 = thunk_FUN_14039be80(local_150);
      local_168 = 0x87;
      thunk_FUN_14039b070(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\infoLoader.cpp"
                         );
      thunk_FUN_14039a8a0(local_150);
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
        FUN_140002d6f(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    *(undefined1 *)(param_1 + 0x1a8) = 0;
    uVar2 = thunk_FUN_1400ec3c0();
    thunk_FUN_140301380(uVar2,*(undefined8 *)(param_1 + 0x1a0));
  }
  thunk_FUN_140190b30(param_1);
  return;
}

// =============================================================================
// Ghidra: FUN_1402aa5a0 @ 0x1402aa5a0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QImage * FUN_1402aa5a0(longlong param_1,QImage *param_2,undefined8 *param_3)

{
  uint uVar1;
  char cVar2;
  bool bVar3;
  Format FVar4;
  Format FVar5;
  int iVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  longlong lVar9;
  longlong *plVar10;
  QDateTime *this;
  QImage *pQVar11;
  undefined8 *puVar12;
  QString *this_00;
  QByteArray *this_01;
  char *pcVar13;
  QHashData *pQVar14;
  int iVar15;
  longlong lVar16;
  ulonglong uVar17;
  longlong lVar18;
  longlong lVar19;
  uint uVar20;
  longlong lVar21;
  QHashData *pQVar22;
  undefined1 auStack_448 [32];
  QString *local_428;
  __int64 local_420;
  uint local_418;
  longlong local_410;
  QString local_408 [8];
  Data *local_400;
  Data *local_3f8;
  void **local_3f0;
  void **local_3e8;
  undefined4 local_3e0;
  longlong local_3d8;
  undefined8 *local_3d0;
  QImage *local_3c8;
  QHashData *local_3c0;
  Node *local_3b8;
  QHashData *local_3b0;
  uint local_3a8;
  undefined4 local_3a0;
  void *local_398;
  QByteArray local_390 [8];
  QString local_388 [8];
  undefined ***local_380;
  undefined8 local_378;
  undefined8 uStack_370;
  QImage local_368 [32];
  QImage local_348 [32];
  undefined8 local_328;
  undefined1 local_320 [16];
  undefined1 local_310 [16];
  undefined1 local_300 [16];
  undefined1 local_2f0 [20];
  int iStack_2dc;
  undefined *local_2d8;
  undefined **local_2d0;
  basic_ostream<char,std::char_traits<char>_> local_2c8 [96];
  undefined8 local_268;
  undefined4 local_260;
  basic_ios<char,std::char_traits<char>_> local_250 [100];
  int iStack_1ec;
  undefined *local_1e8;
  undefined **local_1e0;
  basic_ostream<char,std::char_traits<char>_> local_1d8 [96];
  undefined8 local_178;
  undefined4 local_170;
  basic_ios<char,std::char_traits<char>_> local_160 [104];
  QImage local_f8 [32];
  QRect local_d8 [16];
  QImage local_c8 [32];
  QImage local_a8 [32];
  ulonglong local_88 [3];
  ulonglong local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_328 = 0xfffffffffffffffe;
  local_48 = DAT_140e88880 ^ (ulonglong)auStack_448;
  uVar20 = 0;
  local_418 = 0;
  local_3d8 = param_1;
  local_3d0 = param_3;
  local_3c8 = param_2;
  if (*(int *)(*(longlong *)(param_1 + 0x20) + 4) == 0) {
    uVar7 = thunk_FUN_14039be80(local_320);
    cVar2 = thunk_FUN_14039ba40(uVar7,40000);
    thunk_FUN_14039a8a0(local_320);
    if (cVar2 != '\0') {
      local_2d8 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_250);
      local_418 = 8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_2d8,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_2d0,false);
      *(undefined ***)((longlong)&local_2d8 + (longlong)*(int *)(local_2d8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_2dc + (longlong)*(int *)(local_2d8 + 4)) =
           *(int *)(local_2d8 + 4) + -0x88;
      local_3c8 = (QImage *)&local_2d0;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_2d0);
      local_2d0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_268 = 0;
      local_260 = 4;
      thunk_FUN_14006fb70(&local_2d8,"no frame layout");
      uVar7 = thunk_FUN_1400876c0(&local_2d8,local_88);
      uVar8 = thunk_FUN_14039be80(local_310);
      local_428 = (QString *)CONCAT44(local_428._4_4_,0x13a);
      thunk_FUN_14039b070(uVar8,40000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                         );
      thunk_FUN_14039a8a0(local_310);
      if (0xf < local_70) {
        uVar17 = local_88[0];
        if (0xfff < local_70 + 1) {
          if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar17 = *(ulonglong *)(local_88[0] - 8);
          if (local_88[0] <= uVar17) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_88[0] - uVar17 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_88[0] - uVar17) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar17);
      }
      *(undefined ***)((longlong)&local_2d8 + (longlong)*(int *)(local_2d8 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_2dc + (longlong)*(int *)(local_2d8 + 4)) =
           *(int *)(local_2d8 + 4) + -0x88;
      local_2d0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_2d0);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_2d0);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_2c8);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_250);
    }
    QImage::QImage(param_2);
    return param_2;
  }
  QImage::QImage(local_368,*(int *)(param_1 + 0x18),*(int *)(param_1 + 0x1c),0xb);
  QImage::fill(local_368,0);
  uVar7 = thunk_FUN_140295a40(*param_3);
  thunk_FUN_1400757a0(&local_400,uVar7);
  thunk_FUN_1400757a0(&local_3f8,&local_400);
  local_3f0 = QListData::begin((QListData *)&local_3f8);
  local_3e8 = QListData::end((QListData *)&local_3f8);
  for (; local_3e0 = 1, local_3f0 != local_3e8; local_3f0 = local_3f0 + 1) {
    local_398 = *local_3f0;
    lVar9 = thunk_FUN_140295910(*param_3);
    if (lVar9 != 0) {
      local_3c0 = *(QHashData **)(lVar9 + 0x38);
      pQVar22 = local_3c0 + 0x10;
      if (1 < *(uint *)pQVar22 + 1) {
        LOCK();
        *(uint *)pQVar22 = *(uint *)pQVar22 + 1;
        UNLOCK();
        uVar20 = local_418;
      }
      if ((((byte)local_3c0[0x28] & 1) == 0) && (1 < *(uint *)pQVar22)) {
        thunk_FUN_140165a10(&local_3c0);
      }
      pQVar22 = local_3c0;
      plVar10 = *(longlong **)(local_3c0 + 8);
      iVar15 = *(int *)(local_3c0 + 0x20);
      while (pQVar14 = local_3c0, iVar15 != 0) {
        iVar15 = iVar15 + -1;
        if ((QHashData *)*plVar10 != local_3c0) {
          pQVar14 = (QHashData *)*plVar10;
          break;
        }
        plVar10 = plVar10 + 1;
      }
      local_3a8 = 1;
      local_3b0 = local_3c0;
      do {
        local_3b8 = (Node *)pQVar14;
        if (pQVar14 == pQVar22) break;
        if (local_3a8 != 0) {
          this = (QDateTime *)thunk_FUN_140295a70(*local_3d0);
          local_420 = QDateTime::toMSecsSinceEpoch(this);
          local_428 = (QString *)(pQVar14 + 0x10);
          thunk_FUN_140255f60(*(undefined8 *)(param_1 + 0x10),local_348,
                              *(undefined8 *)(lVar9 + 0x10),lVar9 + 0x18);
          bVar3 = QImage::isNull(local_348);
          if (!bVar3) {
            FVar4 = QImage::format(local_348);
            FVar5 = QImage::format(local_368);
            if (FVar4 == FVar5) {
              pQVar11 = (QImage *)QImage::QImage(local_a8,local_348);
              uVar20 = uVar20 | 2;
            }
            else {
              local_3a0 = 0;
              FVar4 = QImage::format(local_368);
              pQVar11 = (QImage *)QImage::convertToFormat(local_348,local_c8,FVar4,0);
              uVar20 = uVar20 | 4;
            }
            local_418 = uVar20;
            QImage::QImage(local_f8,pQVar11);
            if ((uVar20 & 4) != 0) {
              uVar20 = uVar20 & 0xfffffffb;
              local_418 = uVar20;
              QImage::~QImage(local_c8);
            }
            if ((uVar20 & 2) != 0) {
              uVar20 = uVar20 & 0xfffffffd;
              local_418 = uVar20;
              QImage::~QImage(local_a8);
            }
            local_410 = *(longlong *)(lVar9 + 0x10);
            QString::QString(local_408,(QString *)(pQVar14 + 0x10));
            puVar12 = (undefined8 *)QRect::QRect(local_d8);
            lVar18 = *(longlong *)(*(longlong *)(local_3d8 + 0x20) + 0x10);
            if (lVar18 != 0) {
              lVar21 = 0;
              lVar16 = local_410;
              do {
                if ((*(longlong *)(lVar18 + 0x18) < lVar16) ||
                   ((*(longlong *)(lVar18 + 0x18) <= lVar16 &&
                    (bVar3 = operator<((QString *)(lVar18 + 0x20),local_408), lVar16 = local_410,
                    bVar3)))) {
                  lVar19 = *(longlong *)(lVar18 + 0x10);
                }
                else {
                  lVar19 = *(longlong *)(lVar18 + 8);
                  lVar21 = lVar18;
                }
                lVar18 = lVar19;
              } while (lVar19 != 0);
              if (((lVar21 != 0) && (*(longlong *)(lVar21 + 0x18) <= lVar16)) &&
                 ((*(longlong *)(lVar21 + 0x18) < lVar16 ||
                  (bVar3 = operator<(local_408,(QString *)(lVar21 + 0x20)), !bVar3)))) {
                puVar12 = (undefined8 *)(lVar21 + 0x28);
              }
            }
            local_378 = *puVar12;
            uStack_370 = puVar12[1];
            bVar3 = QRect::isValid((QRect *)&local_378);
            if (bVar3) {
              iVar15 = QRect::y((QRect *)&local_378);
              iVar6 = QRect::x((QRect *)&local_378);
              thunk_FUN_1402522d0(local_368,iVar6,iVar15,local_f8);
            }
            else {
              uVar7 = thunk_FUN_14039be80(local_300);
              cVar2 = thunk_FUN_14039ba40(uVar7);
              thunk_FUN_14039a8a0(local_300);
              if (cVar2 != '\0') {
                local_1e8 = &DAT_1405a3878;
                std::basic_ios<char,std::char_traits<char>_>::
                basic_ios<char,std::char_traits<char>_>(local_160);
                uVar20 = uVar20 | 0x10;
                local_418 = uVar20;
                std::basic_ostream<char,std::char_traits<char>_>::
                basic_ostream<char,std::char_traits<char>_>
                          ((basic_ostream<char,std::char_traits<char>_> *)&local_1e8,
                           (basic_streambuf<char,std::char_traits<char>_> *)&local_1e0,false);
                *(undefined ***)((longlong)&local_1e8 + (longlong)*(int *)(local_1e8 + 4)) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_1ec + (longlong)*(int *)(local_1e8 + 4)) =
                     *(int *)(local_1e8 + 4) + -0x88;
                local_380 = &local_1e0;
                std::basic_streambuf<char,std::char_traits<char>_>::
                basic_streambuf<char,std::char_traits<char>_>
                          ((basic_streambuf<char,std::char_traits<char>_> *)&local_1e0);
                local_1e0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>
                            ::vftable;
                local_178 = 0;
                local_170 = 4;
                this_00 = (QString *)thunk_FUN_1402ab470(&local_410,local_388);
                uVar7 = thunk_FUN_14006fb70(&local_1e8,"invalid rect supplied for: ");
                this_01 = (QByteArray *)QString::toLocal8Bit(this_00);
                pcVar13 = QByteArray::data(this_01);
                thunk_FUN_14006fb70(uVar7,pcVar13);
                QByteArray::~QByteArray(local_390);
                QString::~QString(local_388);
                uVar7 = thunk_FUN_1400876c0(&local_1e8,local_68);
                uVar8 = thunk_FUN_14039be80(local_2f0);
                local_428 = (QString *)CONCAT44(local_428._4_4_,0x15d);
                thunk_FUN_14039b070(uVar8,30000,uVar7,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                                   );
                thunk_FUN_14039a8a0(local_2f0);
                if (0xf < local_50) {
                  uVar17 = local_68[0];
                  if (0xfff < local_50 + 1) {
                    if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    uVar17 = *(ulonglong *)(local_68[0] - 8);
                    if (local_68[0] <= uVar17) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    if (local_68[0] - uVar17 < 8) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                    if (0x27 < local_68[0] - uVar17) {
                    /* WARNING: Subroutine does not return */
                      _invalid_parameter_noinfo_noreturn();
                    }
                  }
                  FUN_140002d6f(uVar17);
                }
                *(undefined ***)((longlong)&local_1e8 + (longlong)*(int *)(local_1e8 + 4)) =
                     std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
                *(int *)((longlong)&iStack_1ec + (longlong)*(int *)(local_1e8 + 4)) =
                     *(int *)(local_1e8 + 4) + -0x88;
                local_1e0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>
                            ::vftable;
                thunk_FUN_14007bb20(&local_1e0);
                std::basic_streambuf<char,std::char_traits<char>_>::
                ~basic_streambuf<char,std::char_traits<char>_>
                          ((basic_streambuf<char,std::char_traits<char>_> *)&local_1e0);
                std::basic_ostream<char,std::char_traits<char>_>::
                ~basic_ostream<char,std::char_traits<char>_>(local_1d8);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_160);
              }
            }
            QString::~QString(local_408);
            QImage::~QImage(local_f8);
            param_1 = local_3d8;
          }
          QImage::~QImage(local_348);
          local_3a8 = 0;
        }
        uVar1 = local_3a8;
        pQVar14 = (QHashData *)QHashData::nextNode((Node *)pQVar14);
        local_3a8 = uVar1 ^ 1;
        local_3b8 = (Node *)pQVar14;
      } while (local_3a8 != 0);
      if (*(int *)(pQVar22 + 0x10) == 0) {
LAB_1402aada7:
        QHashData::free_helper(pQVar22,thunk_FUN_1401657a0);
        param_3 = local_3d0;
      }
      else {
        param_3 = local_3d0;
        if (*(int *)(pQVar22 + 0x10) != -1) {
          LOCK();
          pQVar22 = pQVar22 + 0x10;
          iVar15 = *(int *)pQVar22;
          *(int *)pQVar22 = *(int *)pQVar22 + -1;
          UNLOCK();
          pQVar22 = local_3c0;
          uVar20 = local_418;
          if (iVar15 == 1) goto LAB_1402aada7;
        }
      }
    }
  }
  local_3e0 = 1;
  if (*(int *)local_3f8 == 0) {
LAB_1402aae1c:
    QListData::dispose(local_3f8);
  }
  else if (*(int *)local_3f8 != -1) {
    LOCK();
    iVar15 = *(int *)local_3f8;
    *(int *)local_3f8 = *(int *)local_3f8 + -1;
    UNLOCK();
    if (iVar15 == 1) goto LAB_1402aae1c;
  }
  pQVar11 = local_3c8;
  QImage::QImage(local_3c8,local_368);
  if (*(int *)local_400 != 0) {
    if (*(int *)local_400 == -1) goto LAB_1402aae5a;
    LOCK();
    iVar15 = *(int *)local_400;
    *(int *)local_400 = *(int *)local_400 + -1;
    UNLOCK();
    if (iVar15 != 1) goto LAB_1402aae5a;
  }
  QListData::dispose(local_400);
LAB_1402aae5a:
  QImage::~QImage(local_368);
  return pQVar11;
}

// =============================================================================
// Ghidra: FUN_1402a9a20 @ 0x1402a9a20
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

bool FUN_1402a9a20(QImage *param_1,int *param_2)

{
  char cVar1;
  Format FVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  basic_ostream<char,std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  uchar *puVar9;
  ulonglong uVar10;
  undefined1 auStack_1b8 [32];
  undefined8 local_198;
  int local_190;
  undefined8 local_188;
  int local_180;
  int local_178;
  int local_170;
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
  ulonglong local_50 [3];
  ulonglong local_38;
  ulonglong local_30;
  
  local_58 = 0xfffffffffffffffe;
  local_30 = DAT_140e88880 ^ (ulonglong)auStack_1b8;
  local_168 = 0;
  FVar2 = QImage::format(param_1);
  if (FVar2 == 0xb) {
    if (*param_2 == 0x102) {
      if (param_2[0x14] == 0xc) {
        iVar3 = QImage::width(param_1);
        if ((param_2[1] == iVar3) && (iVar3 = QImage::height(param_1), param_2[2] == iVar3)) {
          iVar3 = QImage::height(param_1);
          iVar4 = QImage::width(param_1);
          iVar5 = QImage::bytesPerLine(param_1);
          puVar9 = QImage::bits(param_1);
          local_180 = param_2[0x12];
          local_188 = *(undefined8 *)(param_2 + 0xc);
          local_190 = param_2[0x11];
          local_198 = *(undefined8 *)(param_2 + 10);
          local_178 = iVar4;
          local_170 = iVar3;
          iVar3 = thunk_FUN_1403e5160(puVar9,iVar5,*(undefined8 *)(param_2 + 8),param_2[0x10]);
          return iVar3 == 0;
        }
        uVar6 = thunk_FUN_14039be80(local_160);
        cVar1 = thunk_FUN_14039ba40(uVar6,40000);
        thunk_FUN_14039a8a0(local_160);
        if (cVar1 == '\0') {
          return false;
        }
        local_148 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_168 = 8;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_140076630(&local_140,2);
        thunk_FUN_14006fb70(&local_148,"incompatible images");
        uVar6 = thunk_FUN_1400876c0(&local_148,local_50);
        uVar8 = thunk_FUN_14039be80(local_160);
        local_198 = CONCAT44(local_198._4_4_,0x17d);
        thunk_FUN_14039b070(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                           );
        thunk_FUN_14039a8a0(local_160);
        if (0xf < local_38) {
          uVar10 = local_50[0];
          if (0xfff < local_38 + 1) {
            if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar10 = *(ulonglong *)(local_50[0] - 8);
            if (local_50[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_50[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_50[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar10);
        }
      }
      else {
        uVar6 = thunk_FUN_14039be80(local_160);
        cVar1 = thunk_FUN_14039ba40(uVar6,40000);
        thunk_FUN_14039a8a0(local_160);
        if (cVar1 == '\0') {
          return false;
        }
        local_148 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_168 = 4;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        thunk_FUN_140076630(&local_140,2);
        pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14006fb70(&local_148,"unexpected bps: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,param_2[0x14]);
        uVar6 = thunk_FUN_1400876c0(&local_148,local_50);
        uVar8 = thunk_FUN_14039be80(local_160);
        local_198 = CONCAT44(local_198._4_4_,0x177);
        thunk_FUN_14039b070(uVar8,40000,uVar6,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                           );
        thunk_FUN_14039a8a0(local_160);
        if (0xf < local_38) {
          uVar10 = local_50[0];
          if (0xfff < local_38 + 1) {
            if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar10 = *(ulonglong *)(local_50[0] - 8);
            if (local_50[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_50[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_50[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar10);
        }
      }
    }
    else {
      uVar6 = thunk_FUN_14039be80(local_160);
      cVar1 = thunk_FUN_14039ba40(uVar6,40000);
      thunk_FUN_14039a8a0(local_160);
      if (cVar1 == '\0') {
        return false;
      }
      local_148 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
      local_168 = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
      *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
           *(int *)(local_148 + 4) + -0x88;
      thunk_FUN_140076630(&local_140,2);
      pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14006fb70(&local_148,"unexpected output format: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,*param_2);
      uVar6 = thunk_FUN_1400876c0(&local_148,local_50);
      uVar8 = thunk_FUN_14039be80(local_160);
      local_198 = CONCAT44(local_198._4_4_,0x171);
      thunk_FUN_14039b070(uVar8,40000,uVar6,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                         );
      thunk_FUN_14039a8a0(local_160);
      if (0xf < local_38) {
        uVar10 = local_50[0];
        if (0xfff < local_38 + 1) {
          if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar10 = *(ulonglong *)(local_50[0] - 8);
          if (local_50[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_50[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_50[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar10);
      }
    }
    thunk_FUN_140077af0(local_c0);
  }
  else {
    uVar6 = thunk_FUN_14039be80(local_160);
    cVar1 = thunk_FUN_14039ba40(uVar6,40000);
    thunk_FUN_14039a8a0(local_160);
    if (cVar1 == '\0') {
      return false;
    }
    local_148 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
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
    FVar2 = QImage::format(param_1);
    pbVar7 = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14006fb70(&local_148,"unexpected input format: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar7,FVar2);
    uVar6 = thunk_FUN_1400876c0(&local_148,local_50);
    uVar8 = thunk_FUN_14039be80(local_160);
    local_198 = CONCAT44(local_198._4_4_,0x16b);
    thunk_FUN_14039b070(uVar8,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                       );
    thunk_FUN_14039a8a0(local_160);
    if (0xf < local_38) {
      uVar10 = local_50[0];
      if (0xfff < local_38 + 1) {
        if ((local_50[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar10 = *(ulonglong *)(local_50[0] - 8);
        if (local_50[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_50[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_50[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar10);
    }
    *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
         *(int *)(local_148 + 4) + -0x88;
    local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_140);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_138);
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_c0);
  return false;
}

// =============================================================================
// Ghidra: FUN_1402b2060 @ 0x1402b2060
// =============================================================================
undefined8 * FUN_1402b2060(undefined8 *param_1)

{
  int iVar1;
  __int64 _Var2;
  undefined8 uVar3;
  undefined8 uVar4;
  QByteArray *this;
  char *_Buf2;
  char *_Buf1;
  QTypedArrayData<unsigned_short> *local_res18;
  QByteArray local_res20 [8];
  undefined8 in_stack_ffffffffffffffa8;
  undefined4 uVar5;
  ulonglong uVar6;
  code *local_38;
  undefined4 uStack_30;
  
  uVar5 = (undefined4)((ulonglong)in_stack_ffffffffffffffa8 >> 0x20);
  local_res18 = QString::fromAscii_helper("historyAudio4Viewer",0x13);
  thunk_FUN_1401902c0(param_1);
  *param_1 = transport::protocol::ProcessorCustom<viewer::history::player::AudioStore>::vftable;
  *(undefined2 *)(param_1 + 0xd) = 0;
  uVar4 = 0;
  param_1[0xe] = 0;
  _Var2 = QDateTime::currentMSecsSinceEpoch();
  qsrand((uint)_Var2);
  *param_1 = transport::protocol::ProcessorClient<viewer::history::player::AudioStore>::vftable;
  QString::QString((QString *)(param_1 + 0xf),(QString *)&local_res18);
  thunk_FUN_140196120(param_1 + 0x10,param_1);
  *param_1 = node::protocol::history::audio::
             AudioHistoryProcessorClient<viewer::history::player::AudioStore>::vftable;
  QString::~QString((QString *)&local_res18);
  *param_1 = viewer::history::player::AudioStore::vftable;
  param_1[0x34] = 0;
  param_1[0x35] = 0;
  param_1[0x36] = shared_null_exref;
  param_1[0x37] = 0;
  *(undefined4 *)(param_1 + 0x38) = 0;
  param_1[0x39] = shared_null_exref;
  local_res18 = (QTypedArrayData<unsigned_short> *)(param_1 + 0x3a);
  QSqlDatabase::QSqlDatabase((QSqlDatabase *)local_res18);
  QString::QString((QString *)(param_1 + 0x3b));
  QString::QString((QString *)(param_1 + 0x3c));
  QString::QString((QString *)(param_1 + 0x3d));
  local_res18 = operator_new(0x58);
  uVar3 = uVar4;
  if (local_res18 != (QTypedArrayData<unsigned_short> *)0x0) {
    uVar3 = thunk_FUN_1402c5890(local_res18,param_1,0);
  }
  param_1[0x3e] = uVar3;
  local_res18 = operator_new(0x58);
  if (local_res18 != (QTypedArrayData<unsigned_short> *)0x0) {
    uVar4 = thunk_FUN_1402c5890(local_res18,param_1,1);
  }
  param_1[0x3f] = uVar4;
  QMetaObject::normalizedType((char *)&local_res18);
  QByteArray::constData((QByteArray *)&local_res18);
  this = (QByteArray *)QMetaObject::normalizedType((char *)local_res20);
  iVar1 = *(int *)(local_res18 + 4);
  if (iVar1 == *(int *)(*(longlong *)this + 4)) {
    _Buf2 = QByteArray::constData(this);
    _Buf1 = QByteArray::constData((QByteArray *)&local_res18);
    iVar1 = memcmp(_Buf1,_Buf2,(longlong)iVar1);
    if (iVar1 == 0) goto LAB_1402b2258;
  }
  qt_assert_x("qRegisterNormalizedMetaType",
              "qRegisterNormalizedMetaType was called with a not normalized type name, please call qRegisterMetaType instead."
              ,"c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qmetatype.h",0x68e);
LAB_1402b2258:
  QByteArray::~QByteArray(local_res20);
  uVar6 = 0;
  QMetaType::registerNormalizedType
            (&local_res18,thunk_FUN_1402b3c10,thunk_FUN_1402b3bb0,0x10,CONCAT44(uVar5,7),0);
  QByteArray::~QByteArray((QByteArray *)&local_res18);
  local_38 = thunk_FUN_140126ea0;
  uStack_30 = 0;
  uVar3 = thunk_FUN_140083800();
  uVar6 = uVar6 & 0xffffffff00000000;
  thunk_FUN_1402b01b0(&local_res18,uVar3,&local_38,param_1,thunk_FUN_1402baa60,uVar6);
  QMetaObject::Connection::~Connection((Connection *)&local_res18);
  local_38 = thunk_FUN_140136100;
  uStack_30 = 0;
  uVar3 = thunk_FUN_140083800();
  uVar6 = uVar6 & 0xffffffff00000000;
  thunk_FUN_1402b02a0(&local_res18,uVar3,&local_38,param_1,thunk_FUN_1402baa60,uVar6);
  QMetaObject::Connection::~Connection((Connection *)&local_res18);
  local_38 = thunk_FUN_140126e80;
  uStack_30 = 0;
  uVar3 = thunk_FUN_140083800();
  uVar6 = uVar6 & 0xffffffff00000000;
  thunk_FUN_1402b01b0(&local_res18,uVar3,&local_38,param_1,thunk_FUN_1402baa60,uVar6);
  QMetaObject::Connection::~Connection((Connection *)&local_res18);
  uVar6 = uVar6 & 0xffffffff00000000;
  thunk_FUN_1402b0040(&local_res18,param_1,thunk_FUN_140287d80,param_1[0x3e],&LAB_14000f600,uVar6);
  QMetaObject::Connection::~Connection((Connection *)&local_res18);
  thunk_FUN_1402b0040(&local_res18,param_1,thunk_FUN_140287d80,param_1[0x3f],&LAB_14000f600,
                      uVar6 & 0xffffffff00000000);
  QMetaObject::Connection::~Connection((Connection *)&local_res18);
  thunk_FUN_1402baa60(param_1);
  return param_1;
}

// =============================================================================
// Ghidra: FUN_1402c6fb0 @ 0x1402c6fb0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1402c6fb0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  QByteArray *pQVar6;
  char *pcVar7;
  basic_ostream<char,std::char_traits<char>_> *pbVar8;
  undefined8 uVar9;
  longlong *plVar10;
  void **ppvVar11;
  ulonglong uVar12;
  undefined4 uVar13;
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
  local_40 = DAT_140e88880 ^ (ulonglong)auStack_1a8;
  local_178 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_178 & 0xffffffff00000000);
  iVar3 = thunk_FUN_1402c5f80();
  uVar5 = thunk_FUN_1402b69e0();
  local_178 = QString::fromAscii_helper("viewerAudio",0xb);
  cVar2 = thunk_FUN_1403db420(uVar5,&local_178);
  QString::~QString((QString *)&local_178);
  uVar13 = 0;
  if (cVar2 != '\0') {
    uVar5 = thunk_FUN_14039be80(local_170);
    cVar2 = thunk_FUN_14039ba40(uVar5,20000);
    thunk_FUN_14039a8a0(local_170);
    uVar13 = 0;
    if (cVar2 != '\0') {
      local_158 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      uVar13 = 1;
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
      pQVar6 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x50));
      pcVar7 = QByteArray::data(pQVar6);
      thunk_FUN_14006fb70(&local_158,pcVar7);
      QByteArray::~QByteArray((QByteArray *)&local_178);
      pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14006fb70(&local_158,"setPosition: trackIdx=");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,iVar3);
      uVar5 = thunk_FUN_1400876c0(&local_158,local_60);
      uVar9 = thunk_FUN_14039be80(local_170);
      local_188 = 0x4d;
      thunk_FUN_14039b070(uVar9,20000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioPlayer.cpp"
                         );
      thunk_FUN_14039a8a0(local_170);
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
        FUN_140002d6f(uVar12);
      }
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_150);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_148);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
  }
  if (iVar3 == -1) {
    uVar5 = 0;
  }
  else {
    iVar4 = QMediaPlaylist::currentIndex(*(QMediaPlaylist **)(param_1 + 0x28));
    if (iVar4 != iVar3) {
      QMediaPlaylist::setCurrentIndex(*(QMediaPlaylist **)(param_1 + 0x28),iVar3);
    }
    plVar10 = (longlong *)thunk_FUN_1402c5c70(param_1 + 0x18,iVar3);
    QMediaPlayer::setPosition(*(QMediaPlayer **)(param_1 + 0x20),param_2 - *plVar10);
    uVar5 = thunk_FUN_1402b69e0();
    local_178 = QString::fromAscii_helper("viewerAudio",0xb);
    cVar2 = thunk_FUN_1403db420(uVar5,&local_178);
    QString::~QString((QString *)&local_178);
    if (cVar2 != '\0') {
      uVar5 = thunk_FUN_14039be80(local_170);
      cVar2 = thunk_FUN_14039ba40(uVar5,20000);
      thunk_FUN_14039a8a0(local_170);
      if (cVar2 != '\0') {
        local_158 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        local_178 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_178._4_4_,uVar13) | 2);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        thunk_FUN_140076630(&local_150,2);
        if ((iVar3 < 0) || (iVar4 = QListData::size((QListData *)(param_1 + 0x18)), iVar4 <= iVar3))
        {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
        }
        if (1 < **(uint **)(param_1 + 0x18)) {
          thunk_FUN_14026bf60(param_1 + 0x18,(*(uint **)(param_1 + 0x18))[1]);
        }
        ppvVar11 = QListData::at((QListData *)(param_1 + 0x18),iVar3);
        plVar10 = *ppvVar11;
        pQVar6 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x50));
        pcVar7 = QByteArray::data(pQVar6);
        thunk_FUN_14006fb70(&local_158,pcVar7);
        QByteArray::~QByteArray((QByteArray *)&local_178);
        lVar1 = *plVar10;
        pbVar8 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14006fb70(&local_158,"setPosition pos=");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar8,param_2 - lVar1);
        uVar5 = thunk_FUN_1400876c0(&local_158,local_60);
        uVar9 = thunk_FUN_14039be80(local_170);
        local_188 = 0x56;
        thunk_FUN_14039b070(uVar9,20000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioPlayer.cpp"
                           );
        thunk_FUN_14039a8a0(local_170);
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
          FUN_140002d6f(uVar12);
        }
        thunk_FUN_140077af0(local_d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
      }
    }
    uVar5 = 1;
  }
  return uVar5;
}

// =============================================================================
// Ghidra: FUN_1402e7aa0 @ 0x1402e7aa0
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402e7aa0(longlong param_1)

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
  local_28 = DAT_140e88880 ^ (ulonglong)auStack_188;
  local_158 = 0;
  if (*(char *)(param_1 + 0x1a8) != '\0') {
    uVar2 = thunk_FUN_14039be80(local_150);
    cVar1 = thunk_FUN_14039ba40(uVar2,20000);
    thunk_FUN_14039a8a0(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_1405a3878;
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
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14006fb70(&local_138,"keylogger connection closed for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1a0));
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar3 = thunk_FUN_14039be80(local_150);
      local_168 = 0x45;
      thunk_FUN_14039b070(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-keylogger\\engine\\loader_82.cpp"
                         );
      thunk_FUN_14039a8a0(local_150);
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
        FUN_140002d6f(uVar4);
      }
      *(undefined ***)((longlong)&local_138 + (longlong)*(int *)(local_138 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_140 + (longlong)*(int *)(local_138 + 4) + 4) =
           *(int *)(local_138 + 4) + -0x88;
      local_130 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_130);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_130);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_128);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_b0);
    }
    thunk_FUN_140190b30(param_1);
    *(undefined1 *)(param_1 + 0x1a8) = 0;
    thunk_FUN_1402e9420(param_1,*(undefined8 *)(param_1 + 0x1a0),0);
  }
  return;
}

// =============================================================================
// Ghidra: FUN_1402df960 @ 0x1402df960
// =============================================================================
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402df960(longlong param_1,longlong param_2,longlong *param_3)

{
  longlong lVar1;
  char cVar2;
  QMapNodeBase *this;
  undefined8 uVar3;
  __int64 _Var4;
  basic_ostream<char,std::char_traits<char>_> *this_00;
  undefined8 uVar5;
  ulonglong uVar6;
  undefined1 auStack_1c8 [32];
  undefined4 local_1a8;
  undefined4 local_198;
  undefined ***local_190;
  int *local_188;
  undefined8 local_180;
  longlong *local_178;
  undefined1 local_170 [16];
  undefined1 local_160 [20];
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
  
  local_180 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_1c8;
  local_198 = 0;
  lVar1 = *(longlong *)(param_1 + 0x10);
  if (*(longlong *)(lVar1 + 0x10) == 0) {
    this = (QMapNodeBase *)(lVar1 + 8);
  }
  else {
    this = *(QMapNodeBase **)(lVar1 + 0x20);
  }
  local_178 = param_3;
  do {
    if (this == (QMapNodeBase *)(lVar1 + 8)) {
LAB_1402dfa25:
      uVar3 = thunk_FUN_14039be80(local_170);
      cVar2 = thunk_FUN_14039ba40(uVar3,30000);
      thunk_FUN_14039a8a0(local_170);
      if (cVar2 != '\0') {
        local_148 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_c0);
        local_198 = 1;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_148,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_140,false);
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        local_190 = &local_140;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
        local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_d8 = 0;
        local_d0 = 4;
        _Var4 = thunk_FUN_1402e78d0(param_2);
        this_00 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_14006fb70(&local_148,"failed to find loader for a message, node #");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(this_00,_Var4);
        uVar3 = thunk_FUN_1400876c0(&local_148,local_58);
        uVar5 = thunk_FUN_14039be80(local_160);
        local_1a8 = 0xba;
        thunk_FUN_14039b070(uVar5,30000,uVar3,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-keylogger\\engine\\provider_82.cpp"
                           );
        thunk_FUN_14039a8a0(local_160);
        if (0xf < local_40) {
          uVar6 = local_58[0];
          if (0xfff < local_40 + 1) {
            if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar6 = *(ulonglong *)(local_58[0] - 8);
            if (local_58[0] <= uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_58[0] - uVar6 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_58[0] - uVar6) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar6);
        }
        *(undefined ***)((longlong)&local_148 + (longlong)*(int *)(local_148 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_14c + (longlong)*(int *)(local_148 + 4)) =
             *(int *)(local_148 + 4) + -0x88;
        local_140 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14007bb20(&local_140);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_140);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_138);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_c0);
      }
LAB_1402dfbef:
      thunk_FUN_1402c8880(param_3[1]);
      return;
    }
    if (param_2 == *(longlong *)(this + 0x20)) {
      local_190 = (undefined ***)*param_3;
      local_188 = (int *)param_3[1];
      if (local_188 != (int *)0x0) {
        LOCK();
        *local_188 = *local_188 + 1;
        UNLOCK();
        LOCK();
        local_188[1] = local_188[1] + 1;
        UNLOCK();
      }
      thunk_FUN_1402e0bb0(param_1,*(undefined8 *)(this + 0x18));
      if (this != (QMapNodeBase *)(lVar1 + 8)) goto LAB_1402dfbef;
      goto LAB_1402dfa25;
    }
    this = QMapNodeBase::nextNode(this);
  } while( true );
}

// =============================================================================
// Ghidra: __dyn_tls_init @ 0x1404ebe0c
// =============================================================================
/* Library Function - Single Match
    __dyn_tls_init
   
   Library: Visual Studio 2015 Release */

void __dyn_tls_init(undefined8 param_1,int param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (param_2 == 2) {
    for (puVar2 = &DAT_14059d9c0; puVar2 != (undefined8 *)&DAT_14059dbd8; puVar2 = puVar2 + 1) {
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
// Ghidra: tls_callback_0 @ 0x14001221f
// =============================================================================
void tls_callback_0(undefined8 param_1,int param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  
  if (param_2 == 2) {
    for (puVar2 = &DAT_14059d9c0; puVar2 != (undefined8 *)&DAT_14059dbd8; puVar2 = puVar2 + 1) {
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
// Ghidra: tls_callback_1 @ 0x1404ebe84
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
