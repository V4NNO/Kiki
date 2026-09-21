/*
 * viewer.cpp
 * viewer.exe / KickidlerViewer - pseudocod extras cu Ghidra MCP
 * Data analizei: 2026-09-21
 *
 * IMPORTANT:
 * - Acesta este pseudocod C/C++ produs de Ghidra, nu codul-sursa original.
 * - Nu este garantat compilabil. Tipurile undefined*, numele FUN_/DAT_/LAB_,
 *   prototipurile si conventiile de apel sunt inferentele decompilatorului.
 * - Adresele sunt pastrate pentru verificare directa in proiectul Ghidra.
 * - Binarul are 53.338 functii; Qt/CRT/STL, QML, codecurile si instantiarile
 *   repetitive nu sunt reproduse integral.
 */


// =============================================================================
// STARTUP SI APLICATIE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140024a8c
// -----------------------------------------------------------------------------
void entry(void)

{
  __security_init_cookie();
  FUN_1404d5f00();
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1404d5f00
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140108c30
// -----------------------------------------------------------------------------
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
              ((basic_ostream<char,std::char_traits<char>_> *)this,
               (_func_basic_ostream<char,struct_std::char_traits<char>_>_ptr_basic_ostream<char,struct_std::char_traits<char>_>_ptr
                *)&LAB_14000c82e);
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
    qInstallMessageHandler
              ((_func_void_QtMsgType_QMessageLogContext_ptr_QString_ptr *)&LAB_140005961);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400beee0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QCoreApplication * FUN_1400beee0(QCoreApplication *param_1,undefined8 param_2,undefined8 param_3)

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
  thunk_FUN_1403d4050(param_1,param_2,param_3,1);
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1400e6a10
// -----------------------------------------------------------------------------
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
// CONTROL REMOTE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140235300
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402361c0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402361c0(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar3;
  ulonglong uVar4;
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
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_198;
  local_168 = 0;
  if (*(int *)(*(longlong *)(param_1 + 0x230) + 4) != 0) {
    uVar2 = thunk_FUN_14039be80(local_160);
    cVar1 = thunk_FUN_14039ba40(uVar2,20000);
    thunk_FUN_14039a8a0(local_160);
    if (cVar1 != '\0') {
      local_148 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_c0);
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
             thunk_FUN_14006fb70(&local_148,"control connection closed for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1c0));
      uVar2 = thunk_FUN_1400876c0(&local_148,local_58);
      uVar3 = thunk_FUN_14039be80(local_160);
      local_178 = 0x1ba;
      thunk_FUN_14039b070(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\master.cpp"
                         );
      thunk_FUN_14039a8a0(local_160);
      if (0xf < local_40) {
        uVar4 = local_58[0];
        if (0xfff < local_40 + 1) {
          if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar4 = *(ulonglong *)(local_58[0] - 8);
          if (local_58[0] <= uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_58[0] - uVar4 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_58[0] - uVar4) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar4);
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
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_c0);
    }
  }
  *(undefined8 *)(param_1 + 0x1c0) = 0;
  *(undefined8 *)(param_1 + 0x1c8) = 0;
  QString::clear((QString *)(param_1 + 0x1d0));
  QString::clear((QString *)(param_1 + 0x230));
  thunk_FUN_140190b30(param_1);
  if (*(int *)(param_1 + 0x1d8) != 0) {
    *(undefined4 *)(param_1 + 0x1d8) = 0;
    thunk_FUN_1402311a0(param_1);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140236a10
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140238b20
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x140242530
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140242530(undefined8 param_1,bool param_2)

{
  char cVar1;
  undefined8 uVar2;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar3;
  undefined ***pppuVar4;
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
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_198;
  local_168 = 0;
  uVar2 = thunk_FUN_14039be80(local_160);
  cVar1 = thunk_FUN_14039ba40(uVar2,20000);
  thunk_FUN_14039a8a0(local_160);
  if (cVar1 != '\0') {
    local_148 = &DAT_1405a3878;
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
           thunk_FUN_14006fb70(&local_148,"control master connected as server: ");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
    uVar2 = thunk_FUN_1400876c0(&local_148,local_58);
    uVar3 = thunk_FUN_14039be80(local_160);
    local_178 = 0x148;
    thunk_FUN_14039b070(uVar3,20000,uVar2,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control\\channel_31.cpp"
                       );
    thunk_FUN_14039a8a0(local_160);
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
      FUN_140002d6f(uVar5);
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
  if (param_2 == false) {
    thunk_FUN_14023a1e0(param_1,0);
    thunk_FUN_1402474c0(param_1);
  }
  else {
    thunk_FUN_1402474c0();
    pppuVar4 = operator_new(0x10);
    local_160[0] = pppuVar4;
    if (pppuVar4 == (undefined ***)0x0) {
      pppuVar4 = (undefined ***)0x0;
    }
    else {
      thunk_FUN_14018c0c0(pppuVar4);
      *pppuVar4 = grabberAgent::protocol::control::Ripeness::vftable;
      *(undefined1 *)(pppuVar4 + 1) = 1;
    }
    thunk_FUN_14023e640(param_1,pppuVar4);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140244ae0
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402460b0
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402478c0
// -----------------------------------------------------------------------------
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
// ISTORIC VIDEO
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140255580
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 *
FUN_140255580(longlong param_1,undefined8 *param_2,undefined8 param_3,QString *param_4,
             QString *param_5,TimeSpec param_6)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  QSqlError *pQVar6;
  QString *pQVar7;
  QByteArray *pQVar8;
  char *pcVar9;
  undefined8 uVar10;
  QDateTime *pQVar11;
  undefined ***pppuVar12;
  ulonglong uVar13;
  undefined ***pppuVar14;
  undefined1 auStack_1e8 [32];
  undefined4 local_1c8;
  QTypedArrayData<unsigned_short> *local_1b8;
  QSqlQuery local_1b0 [8];
  undefined4 local_1a8;
  undefined4 uStack_1a4;
  undefined4 local_198;
  undefined ***local_190;
  undefined1 local_188;
  undefined8 local_180;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  undefined8 local_88;
  QVariant local_80 [16];
  QVariant local_70 [16];
  ulonglong local_60 [3];
  ulonglong local_48;
  ulonglong local_40;
  
  local_88 = 0xfffffffffffffffe;
  local_40 = DAT_140e88880 ^ (ulonglong)auStack_1e8;
  pppuVar14 = (undefined ***)0x0;
  local_198 = 0;
  local_180 = param_3;
  uVar5 = QSqlDatabase::QSqlDatabase((QSqlDatabase *)&local_190,(QSqlDatabase *)(param_1 + 0x200));
  QSqlQuery::QSqlQuery(local_1b0,uVar5);
  local_1b8 = QString::fromAscii_helper
                        ("SELECT i.status, i.width, i.height, i.format FROM image AS i JOIN segment AS s ON (i.segment_id = s.id AND s.grabberId=?1 AND s.sessionId=?2 AND s.displayId=?3 AND s.start <= ?4 AND s.stop >= ?4) WHERE i.pos <= ?4 AND i.status != ?5 ORDER BY i.id DESC LIMIT 1"
                         ,0x103);
  bVar1 = QSqlQuery::prepare(local_1b0,(QString *)&local_1b8);
  QString::~QString((QString *)&local_1b8);
  if (bVar1) {
    pQVar7 = (QString *)thunk_FUN_1404df6e0(&local_1b8,&local_180);
    QVariant::QVariant((QVariant *)&local_1a8,pQVar7);
    QSqlQuery::addBindValue(local_1b0,&local_1a8,1);
    QVariant::~QVariant((QVariant *)&local_1a8);
    QString::~QString((QString *)&local_1b8);
    QVariant::QVariant((QVariant *)&local_1a8,param_4);
    QSqlQuery::addBindValue(local_1b0,&local_1a8,1);
    QVariant::~QVariant((QVariant *)&local_1a8);
    QVariant::QVariant((QVariant *)&local_1a8,param_5);
    QSqlQuery::addBindValue(local_1b0,&local_1a8,1);
    QVariant::~QVariant((QVariant *)&local_1a8);
    pQVar11 = (QDateTime *)QDateTime::fromMSecsSinceEpoch((__int64)&local_1b8,param_6,1);
    QVariant::QVariant((QVariant *)&local_1a8,pQVar11);
    QSqlQuery::addBindValue(local_1b0,&local_1a8,1);
    QVariant::~QVariant((QVariant *)&local_1a8);
    QDateTime::~QDateTime((QDateTime *)&local_1b8);
    QVariant::QVariant((QVariant *)&local_1a8,1);
    QSqlQuery::addBindValue(local_1b0,&local_1a8);
    QVariant::~QVariant((QVariant *)&local_1a8);
    bVar1 = QSqlQuery::exec(local_1b0);
    if (bVar1) {
      bVar1 = QSqlQuery::next(local_1b0);
      if (bVar1) {
        QSqlQuery::record(local_1b0);
        uVar5 = QSqlRecord::value((QSqlRecord *)&local_1b8,(int)&local_1a8);
        iVar3 = thunk_FUN_14024df60(uVar5);
        QVariant::~QVariant((QVariant *)&local_1a8);
        if (iVar3 == 0) {
          bVar1 = QSqlRecord::isNull((QSqlRecord *)&local_1b8,3);
          if (bVar1) {
            qt_assert("!rec.isNull(3)",
                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                      ,0x18e);
          }
          pppuVar12 = operator_new(0xc);
          local_190 = pppuVar12;
          if (pppuVar12 != (undefined ***)0x0) {
            uVar5 = QSqlRecord::value((QSqlRecord *)&local_1b8,(int)local_60);
            local_198 = 2;
            local_1a8 = thunk_FUN_14024df60(uVar5);
            uVar5 = QSqlRecord::value((QSqlRecord *)&local_1b8,(int)local_70);
            local_198 = 6;
            uStack_1a4 = thunk_FUN_14024df60(uVar5);
            uVar5 = QSqlRecord::value((QSqlRecord *)&local_1b8,(int)local_80);
            local_198 = 0xe;
            uVar4 = thunk_FUN_140104220(uVar5);
            *pppuVar12 = (undefined **)CONCAT44(uStack_1a4,local_1a8);
            *(undefined4 *)(pppuVar12 + 1) = uVar4;
            pppuVar14 = pppuVar12;
          }
          *param_2 = pppuVar14;
          thunk_FUN_14024d040(param_2,pppuVar14,local_188);
          if (pppuVar12 != (undefined ***)0x0) {
            QVariant::~QVariant(local_80);
            QVariant::~QVariant(local_70);
            QVariant::~QVariant((QVariant *)local_60);
          }
          QSqlRecord::~QSqlRecord((QSqlRecord *)&local_1b8);
          goto LAB_140255d34;
        }
        QSqlRecord::~QSqlRecord((QSqlRecord *)&local_1b8);
      }
    }
    else {
      uVar5 = thunk_FUN_14039be80(&local_1a8);
      cVar2 = thunk_FUN_14039ba40(uVar5,40000);
      thunk_FUN_14039a8a0(&local_1a8);
      if (cVar2 != '\0') {
        local_178 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        local_198 = 0x20;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
             *(int *)(local_178 + 4) + -0x88;
        thunk_FUN_140076630(&local_170,2);
        pQVar6 = (QSqlError *)QSqlQuery::lastError(local_1b0);
        pQVar7 = (QString *)QSqlError::text(pQVar6);
        uVar5 = thunk_FUN_14006fb70(&local_178,"unable to exec history select-header statement: ");
        pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar7);
        pcVar9 = QByteArray::data(pQVar8);
        thunk_FUN_14006fb70(uVar5,pcVar9);
        QByteArray::~QByteArray((QByteArray *)&local_190);
        QString::~QString((QString *)&local_1b8);
        QSqlError::~QSqlError((QSqlError *)local_60);
        uVar5 = thunk_FUN_1400876c0(&local_178,local_60);
        uVar10 = thunk_FUN_14039be80(&local_1a8);
        local_1c8 = 0x185;
        thunk_FUN_14039b070(uVar10,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                           );
        thunk_FUN_14039a8a0(&local_1a8);
        if (0xf < local_48) {
          uVar13 = local_60[0];
          if (0xfff < local_48 + 1) {
            if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar13 = *(ulonglong *)(local_60[0] - 8);
            if (local_60[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_60[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_60[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar13);
        }
        thunk_FUN_140077af0(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
    }
  }
  else {
    uVar5 = thunk_FUN_14039be80(&local_1a8);
    cVar2 = thunk_FUN_14039ba40(uVar5,40000);
    thunk_FUN_14039a8a0(&local_1a8);
    if (cVar2 != '\0') {
      local_178 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_f0);
      local_198 = 0x10;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
           *(int *)(local_178 + 4) + -0x88;
      local_190 = &local_170;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_108 = 0;
      local_100 = 4;
      pQVar6 = (QSqlError *)QSqlQuery::lastError(local_1b0);
      pQVar7 = (QString *)QSqlError::text(pQVar6);
      uVar5 = thunk_FUN_14006fb70(&local_178,"unable to prepare history select-header statement: ");
      pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar7);
      pcVar9 = QByteArray::data(pQVar8);
      thunk_FUN_14006fb70(uVar5,pcVar9);
      QByteArray::~QByteArray((QByteArray *)&local_190);
      QString::~QString((QString *)&local_1b8);
      QSqlError::~QSqlError((QSqlError *)local_60);
      uVar5 = thunk_FUN_1400876c0(&local_178,local_60);
      uVar10 = thunk_FUN_14039be80(&local_1a8);
      local_1c8 = 0x179;
      thunk_FUN_14039b070(uVar10,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                         );
      thunk_FUN_14039a8a0(&local_1a8);
      if (0xf < local_48) {
        uVar13 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar13 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar13 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar13) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar13);
      }
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_180 + (longlong)*(int *)(local_178 + 4) + 4) =
           *(int *)(local_178 + 4) + -0x88;
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_170);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_168);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_f0);
    }
  }
  *param_2 = 0;
  param_2[1] = 0;
LAB_140255d34:
  QSqlQuery::~QSqlQuery(local_1b0);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140255f60
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QImage * FUN_140255f60(longlong param_1,QImage *param_2,longlong param_3,QString *param_4,
                      QString *param_5)

{
  uint uVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  void **ppvVar6;
  longlong lVar7;
  longlong lVar8;
  int *piVar9;
  undefined8 uVar10;
  QString *this;
  QByteArray *this_00;
  char *pcVar11;
  longlong *plVar12;
  void **ppvVar13;
  uchar *puVar14;
  QImage *pQVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  undefined8 uVar18;
  basic_ostream<char,std::char_traits<char>_> *pbVar19;
  uint *puVar20;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar21;
  uint *puVar22;
  ulonglong uVar23;
  longlong lVar24;
  Data *pDVar25;
  Data *pDVar26;
  longlong *plVar27;
  QListData *pQVar28;
  QImage *this_01;
  undefined1 auStackY_d28 [32];
  Data *local_ce8;
  longlong local_ce0;
  QString local_cd8 [8];
  QString local_cd0 [8];
  int local_cc8;
  uint local_cc4;
  char local_cc0;
  int local_cb8;
  undefined4 uStack_cb4;
  uint local_cb0;
  QImage *local_ca8;
  QMutexLocker local_ca0 [8];
  longlong *local_c98;
  undefined1 local_c90 [8];
  longlong local_c88;
  undefined8 local_c80;
  undefined4 local_c78;
  Data *local_c70;
  uint *local_c68;
  longlong local_c60;
  undefined8 local_c58;
  undefined8 local_c50;
  int local_c48;
  int local_c44;
  Data *local_c40;
  Data *local_c38;
  QListData *local_c30;
  QByteArray local_c28 [8];
  QString local_c20 [8];
  QByteArray local_c18 [8];
  QListData *local_c10;
  QString local_c08 [8];
  QString local_c00 [8];
  QString local_bf8 [8];
  QString local_bf0 [8];
  QString local_be8 [8];
  QString local_be0 [8];
  QImage local_bd8 [32];
  undefined4 local_bb8 [2];
  QImage local_bb0 [32];
  undefined1 local_b90 [16];
  undefined1 local_b80 [8];
  undefined8 local_b78;
  undefined1 local_b70 [16];
  undefined1 local_b60 [16];
  undefined1 local_b50 [16];
  undefined1 local_b40 [16];
  undefined1 local_b30 [16];
  undefined1 local_b20 [16];
  undefined1 local_b10 [16];
  undefined1 local_b00 [16];
  undefined1 local_af0 [16];
  undefined1 local_ae0 [16];
  undefined1 local_ad0 [16];
  undefined1 local_ac0 [16];
  undefined1 local_ab0 [16];
  undefined1 local_aa0 [16];
  undefined1 local_a90 [16];
  undefined1 local_a80 [16];
  undefined8 local_a70;
  undefined1 local_a68 [8];
  undefined1 local_a60 [16];
  undefined4 local_a50 [2];
  QImage local_a48 [32];
  QRect local_a28 [32];
  QImage local_a08 [32];
  QRect local_9e8 [12];
  int iStack_9dc;
  undefined *local_9d8;
  basic_streambuf<char,std::char_traits<char>_> local_9d0 [128];
  basic_ios<char,std::char_traits<char>_> local_950 [104];
  undefined1 local_8e8 [136];
  basic_ios<char,std::char_traits<char>_> local_860 [104];
  undefined1 local_7f8 [136];
  basic_ios<char,std::char_traits<char>_> local_770 [104];
  undefined1 local_708 [136];
  basic_ios<char,std::char_traits<char>_> local_680 [104];
  undefined1 local_618 [136];
  basic_ios<char,std::char_traits<char>_> local_590 [104];
  undefined1 local_528 [136];
  basic_ios<char,std::char_traits<char>_> local_4a0 [104];
  undefined1 local_438 [136];
  basic_ios<char,std::char_traits<char>_> local_3b0 [104];
  undefined1 local_348 [136];
  basic_ios<char,std::char_traits<char>_> local_2c0 [104];
  undefined1 local_258 [136];
  basic_ios<char,std::char_traits<char>_> local_1d0 [104];
  ulonglong local_168 [3];
  ulonglong local_150;
  undefined1 local_148 [32];
  undefined1 local_128 [32];
  undefined1 local_108 [32];
  undefined1 local_e8 [32];
  undefined1 local_c8 [32];
  undefined1 local_a8 [32];
  undefined1 local_88 [32];
  undefined1 local_68 [32];
  ulonglong local_48;
  
  local_a70 = 0xfffffffffffffffe;
  local_48 = DAT_140e88880 ^ (ulonglong)auStackY_d28;
  local_c78 = 0;
  local_ca8 = param_2;
  local_c60 = param_1;
  QMutexLocker::QMutexLocker(local_ca0,(QBasicMutex *)(param_1 + 0x218));
  if ((*(int *)(*(longlong *)((longlong)ThreadLocalStoragePointer + (ulonglong)_tls_index * 8) +
               0x110) < DAT_140e9fc48) && (_Init_thread_header(&DAT_140e9fc48), DAT_140e9fc48 == -1)
     ) {
    FUN_140250bf0(local_c90,&DAT_140e9fc20);
    atexit(FUN_140595040);
    _Init_thread_footer(&DAT_140e9fc48);
  }
  local_ce0 = param_3;
  QString::QString(local_cd8,param_4);
  QString::QString(local_cd0,param_5);
  local_cc8 = 1;
  local_cc4 = 0xffffffff;
  local_cc0 = '\x01';
  local_c58 = 0;
  thunk_FUN_14025d670(param_1,&local_ce8,&local_ce0,&stack0x00000030);
  iVar4 = QListData::size((QListData *)&local_ce8);
  if (1 < iVar4) {
    qt_assert("frames.size() < 2",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
              ,0xf5);
  }
  iVar4 = QListData::size((QListData *)&local_ce8);
  if (iVar4 != 0) {
    bVar2 = QListData::isEmpty((QListData *)&local_ce8);
    if (bVar2) {
      qt_assert("!isEmpty()","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                0x152);
    }
    if (1 < *(uint *)local_ce8) {
      thunk_FUN_140254890(&local_ce8,*(uint *)(local_ce8 + 4));
    }
    ppvVar6 = QListData::end((QListData *)&local_ce8);
    local_cb0 = *(uint *)ppvVar6[-1];
    bVar2 = QListData::isEmpty((QListData *)&local_ce8);
    if (bVar2) {
      qt_assert("!isEmpty()","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                0x152);
    }
    if (1 < *(uint *)local_ce8) {
      thunk_FUN_140254890(&local_ce8,*(uint *)(local_ce8 + 4));
    }
    ppvVar6 = QListData::end((QListData *)&local_ce8);
    iVar4 = *(int *)((longlong)ppvVar6[-1] + 0x10);
    local_cb8 = iVar4;
    if (iVar4 != 2) {
      plVar27 = (longlong *)(local_c60 + 0x220);
      local_c98 = plVar27;
      if (*(longlong *)(*plVar27 + 0x10) != 0) {
        lVar7 = 0;
        lVar8 = *(longlong *)(*plVar27 + 0x10);
        do {
          if (*(longlong *)(lVar8 + 0x18) < local_ce0) {
LAB_14025624a:
            lVar24 = *(longlong *)(lVar8 + 0x10);
          }
          else {
            if (*(longlong *)(lVar8 + 0x18) <= local_ce0) {
              bVar2 = operator<((QString *)(lVar8 + 0x20),local_cd8);
              if (bVar2) goto LAB_14025624a;
              bVar2 = operator<(local_cd8,(QString *)(lVar8 + 0x20));
              if (!bVar2) {
                bVar2 = operator<((QString *)(lVar8 + 0x28),local_cd0);
                if ((bVar2) ||
                   ((bVar2 = operator<(local_cd0,(QString *)(lVar8 + 0x28)), !bVar2 &&
                    ((*(int *)(lVar8 + 0x30) < local_cc8 ||
                     ((*(int *)(lVar8 + 0x30) <= local_cc8 &&
                      ((*(uint *)(lVar8 + 0x34) < local_cc4 ||
                       (((*(uint *)(lVar8 + 0x34) == local_cc4 &&
                         (*(char *)(lVar8 + 0x38) != local_cc0)) && (local_cc0 != '\0'))))))))))))
                goto LAB_14025624a;
              }
            }
            lVar24 = *(longlong *)(lVar8 + 8);
            lVar7 = lVar8;
          }
          lVar8 = lVar24;
        } while (lVar24 != 0);
        if ((lVar7 != 0) && (cVar3 = thunk_FUN_140250490(&local_ce0,lVar7 + 0x18), cVar3 == '\0')) {
          if (1 < *(uint *)*plVar27) {
            thunk_FUN_140254a30(plVar27);
          }
          if (*(longlong *)(*plVar27 + 0x10) == 0) {
LAB_14025635d:
            local_bb8[0] = 0;
            QImage::QImage(local_bb0);
            if (1 < *(uint *)*plVar27) {
              thunk_FUN_140254a30(plVar27);
            }
            lVar24 = *(longlong *)(*plVar27 + 0x10);
            lVar8 = *plVar27 + 8;
            lVar7 = 0;
            if (lVar24 == 0) {
LAB_140256478:
              lVar7 = thunk_FUN_140253d20(*plVar27,&local_ce0,local_bb8,lVar8);
            }
            else {
              do {
                lVar8 = lVar24;
                if (*(longlong *)(lVar8 + 0x18) < local_ce0) {
LAB_14025643b:
                  lVar24 = *(longlong *)(lVar8 + 0x10);
                }
                else {
                  if (*(longlong *)(lVar8 + 0x18) <= local_ce0) {
                    bVar2 = operator<((QString *)(lVar8 + 0x20),local_cd8);
                    if (bVar2) goto LAB_14025643b;
                    bVar2 = operator<(local_cd8,(QString *)(lVar8 + 0x20));
                    if (!bVar2) {
                      bVar2 = operator<((QString *)(lVar8 + 0x28),local_cd0);
                      if ((bVar2) ||
                         ((bVar2 = operator<(local_cd0,(QString *)(lVar8 + 0x28)), !bVar2 &&
                          ((*(int *)(lVar8 + 0x30) < local_cc8 ||
                           ((*(int *)(lVar8 + 0x30) <= local_cc8 &&
                            ((*(uint *)(lVar8 + 0x34) < local_cc4 ||
                             (((*(uint *)(lVar8 + 0x34) == local_cc4 &&
                               (*(char *)(lVar8 + 0x38) != local_cc0)) && (local_cc0 != '\0'))))))))
                          )))) goto LAB_14025643b;
                    }
                  }
                  lVar24 = *(longlong *)(lVar8 + 8);
                  lVar7 = lVar8;
                }
              } while (lVar24 != 0);
              if ((lVar7 == 0) ||
                 (cVar3 = thunk_FUN_140250490(&local_ce0,lVar7 + 0x18), cVar3 != '\0'))
              goto LAB_140256478;
              *(undefined4 *)(lVar7 + 0x40) = local_bb8[0];
              QImage::operator=((QImage *)(lVar7 + 0x48),local_bb0);
            }
            QImage::~QImage(local_bb0);
            iVar4 = local_cb8;
          }
          else {
            lVar7 = 0;
            lVar8 = *(longlong *)(*plVar27 + 0x10);
            do {
              if (*(longlong *)(lVar8 + 0x18) < local_ce0) {
LAB_140256335:
                lVar24 = *(longlong *)(lVar8 + 0x10);
              }
              else {
                if (*(longlong *)(lVar8 + 0x18) <= local_ce0) {
                  bVar2 = operator<((QString *)(lVar8 + 0x20),local_cd8);
                  if (bVar2) goto LAB_140256335;
                  bVar2 = operator<(local_cd8,(QString *)(lVar8 + 0x20));
                  if (!bVar2) {
                    bVar2 = operator<((QString *)(lVar8 + 0x28),local_cd0);
                    if ((bVar2) ||
                       ((bVar2 = operator<(local_cd0,(QString *)(lVar8 + 0x28)), !bVar2 &&
                        ((*(int *)(lVar8 + 0x30) < local_cc8 ||
                         ((*(int *)(lVar8 + 0x30) <= local_cc8 &&
                          ((*(uint *)(lVar8 + 0x34) < local_cc4 ||
                           (((*(uint *)(lVar8 + 0x34) == local_cc4 &&
                             (*(char *)(lVar8 + 0x38) != local_cc0)) && (local_cc0 != '\0'))))))))))
                       )) goto LAB_140256335;
                  }
                }
                lVar24 = *(longlong *)(lVar8 + 8);
                lVar7 = lVar8;
              }
              lVar8 = lVar24;
            } while (lVar24 != 0);
            if ((lVar7 == 0) ||
               (cVar3 = thunk_FUN_140250490(&local_ce0,lVar7 + 0x18), cVar3 != '\0'))
            goto LAB_14025635d;
          }
          if (local_cb0 == *(uint *)(lVar7 + 0x40)) {
            lVar8 = thunk_FUN_140250130(plVar27,&local_ce0);
            QImage::QImage(param_2,(QImage *)(lVar8 + 8));
            if (*(int *)local_ce8 != 0) {
              if (*(int *)local_ce8 == -1) goto LAB_140257e1a;
              LOCK();
              *(int *)local_ce8 = *(int *)local_ce8 + -1;
              UNLOCK();
              if (*(int *)local_ce8 != 0) goto LAB_140257e1a;
            }
            pDVar26 = local_ce8 + ((longlong)*(int *)(local_ce8 + 0xc) + 2) * 8;
            iVar4 = *(int *)(local_ce8 + 8);
            while (local_ce8 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
              pDVar26 = pDVar26 + -8;
              lVar8 = *(longlong *)pDVar26;
              if (lVar8 != 0) {
                QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
                QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
                FUN_140008206(lVar8,0x28);
              }
            }
            QListData::dispose(local_ce8);
            goto LAB_140257e1a;
          }
        }
      }
      if (iVar4 != 1) goto LAB_140256cc1;
      if (*(longlong *)(*plVar27 + 0x10) != 0) {
        lVar7 = 0;
        lVar8 = *(longlong *)(*plVar27 + 0x10);
        do {
          if ((*(longlong *)(lVar8 + 0x18) < local_ce0) ||
             ((*(longlong *)(lVar8 + 0x18) <= local_ce0 &&
              ((bVar2 = operator<((QString *)(lVar8 + 0x20),local_cd8), bVar2 ||
               ((bVar2 = operator<(local_cd8,(QString *)(lVar8 + 0x20)), !bVar2 &&
                ((bVar2 = operator<((QString *)(lVar8 + 0x28),local_cd0), bVar2 ||
                 ((bVar2 = operator<(local_cd0,(QString *)(lVar8 + 0x28)), !bVar2 &&
                  ((*(int *)(lVar8 + 0x30) < local_cc8 ||
                   ((*(int *)(lVar8 + 0x30) <= local_cc8 &&
                    ((*(uint *)(lVar8 + 0x34) < local_cc4 ||
                     (((*(uint *)(lVar8 + 0x34) == local_cc4 &&
                       (*(char *)(lVar8 + 0x38) != local_cc0)) && (local_cc0 != '\0'))))))))))))))))
              )))) {
            lVar24 = *(longlong *)(lVar8 + 0x10);
          }
          else {
            lVar24 = *(longlong *)(lVar8 + 8);
            lVar7 = lVar8;
          }
          lVar8 = lVar24;
        } while (lVar24 != 0);
        if (((lVar7 != 0) && (cVar3 = thunk_FUN_140250490(&local_ce0,lVar7 + 0x18), cVar3 == '\0'))
           && (piVar9 = (int *)thunk_FUN_140250130(plVar27,&local_ce0), local_cb0 - 1 == *piVar9))
        goto LAB_140256cc1;
      }
      lVar8 = local_c60;
      uVar1 = local_cb0;
      local_c50 = 0;
      thunk_FUN_14025d670(local_c60,&local_cb8,&local_ce0,&stack0x00000030);
      iVar4 = QListData::size((QListData *)&local_cb8);
      if ((iVar4 == 1) && (uVar1 != 0)) {
        bVar2 = QListData::isEmpty((QListData *)&local_cb8);
        if (bVar2) {
          qt_assert("!isEmpty()",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x14f);
        }
        if (1 < *(uint *)CONCAT44(uStack_cb4,local_cb8)) {
          thunk_FUN_140254890(&local_cb8,((uint *)CONCAT44(uStack_cb4,local_cb8))[1]);
        }
        ppvVar6 = QListData::begin((QListData *)&local_cb8);
        if (*(uint *)*ppvVar6 < uVar1) {
          bVar2 = QListData::isEmpty((QListData *)&local_cb8);
          if (bVar2) {
            qt_assert("!isEmpty()",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x14f);
          }
          if (1 < *(uint *)CONCAT44(uStack_cb4,local_cb8)) {
            thunk_FUN_140254890(&local_cb8,((uint *)CONCAT44(uStack_cb4,local_cb8))[1]);
          }
          ppvVar6 = QListData::begin((QListData *)&local_cb8);
          local_c48 = *(int *)*ppvVar6 + 1;
          local_c44 = uVar1 - 1;
          thunk_FUN_14025d670(lVar8,&local_c70,&local_ce0,&stack0x00000030);
          uVar10 = thunk_FUN_1402503f0(&local_cb8,&local_c38,&local_c70);
          uVar10 = thunk_FUN_1402503f0(uVar10,&local_c40,&local_ce8);
          thunk_FUN_14024f980(&local_ce8,uVar10);
          if (*(int *)local_c40 == 0) {
LAB_1402567c6:
            pDVar26 = local_c40 + ((longlong)*(int *)(local_c40 + 0xc) + 2) * 8;
            iVar4 = *(int *)(local_c40 + 8);
            while (local_c40 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
              pDVar26 = pDVar26 + -8;
              lVar8 = *(longlong *)pDVar26;
              if (lVar8 != 0) {
                QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
                QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
                FUN_140008206(lVar8,0x28);
              }
            }
            QListData::dispose(local_c40);
          }
          else if (*(int *)local_c40 != -1) {
            LOCK();
            *(int *)local_c40 = *(int *)local_c40 + -1;
            UNLOCK();
            if (*(int *)local_c40 == 0) goto LAB_1402567c6;
          }
          if (*(int *)local_c38 == 0) {
LAB_14025683d:
            pDVar26 = local_c38 + ((longlong)*(int *)(local_c38 + 0xc) + 2) * 8;
            iVar4 = *(int *)(local_c38 + 8);
            while (local_c38 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
              pDVar26 = pDVar26 + -8;
              lVar8 = *(longlong *)pDVar26;
              if (lVar8 != 0) {
                QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
                QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
                FUN_140008206(lVar8,0x28);
              }
            }
            QListData::dispose(local_c38);
          }
          else if (*(int *)local_c38 != -1) {
            LOCK();
            *(int *)local_c38 = *(int *)local_c38 + -1;
            UNLOCK();
            if (*(int *)local_c38 == 0) goto LAB_14025683d;
          }
          thunk_FUN_14025d190(plVar27,&local_ce0);
          if (*(int *)local_c70 == 0) {
LAB_1402568c8:
            pDVar26 = local_c70 + ((longlong)*(int *)(local_c70 + 0xc) + 2) * 8;
            iVar4 = *(int *)(local_c70 + 8);
            while (local_c70 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
              pDVar26 = pDVar26 + -8;
              lVar8 = *(longlong *)pDVar26;
              if (lVar8 != 0) {
                QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
                QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
                FUN_140008206(lVar8,0x28);
              }
            }
            QListData::dispose(local_c70);
          }
          else if (*(int *)local_c70 != -1) {
            LOCK();
            *(int *)local_c70 = *(int *)local_c70 + -1;
            UNLOCK();
            if (*(int *)local_c70 == 0) goto LAB_1402568c8;
          }
          piVar9 = (int *)CONCAT44(uStack_cb4,local_cb8);
          if (*piVar9 == 0) {
LAB_14025694c:
            pDVar25 = (Data *)CONCAT44(uStack_cb4,local_cb8);
            pDVar26 = pDVar25 + ((longlong)*(int *)(pDVar25 + 0xc) + 2) * 8;
            iVar4 = *(int *)(pDVar25 + 8);
            while (pDVar25 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
              pDVar26 = pDVar26 + -8;
              lVar8 = *(longlong *)pDVar26;
              if (lVar8 != 0) {
                QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
                QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
                FUN_140008206(lVar8,0x28);
              }
            }
            QListData::dispose(pDVar25);
          }
          else if (*piVar9 != -1) {
            LOCK();
            *piVar9 = *piVar9 + -1;
            UNLOCK();
            if (*piVar9 == 0) goto LAB_14025694c;
          }
LAB_140256cc1:
          if (1 < *(uint *)*plVar27) {
            thunk_FUN_140254a30(plVar27);
          }
          if (*(longlong *)(*plVar27 + 0x10) == 0) {
LAB_140256da1:
            local_a50[0] = 0;
            QImage::QImage(local_a48);
            plVar12 = (longlong *)thunk_FUN_140258ff0(plVar27,local_a68,&local_ce0,local_a50);
            lVar8 = *plVar12;
            QImage::~QImage(local_a48);
          }
          else {
            lVar8 = 0;
            lVar7 = *(longlong *)(*plVar27 + 0x10);
            do {
              if ((*(longlong *)(lVar7 + 0x18) < local_ce0) ||
                 ((*(longlong *)(lVar7 + 0x18) <= local_ce0 &&
                  ((bVar2 = operator<((QString *)(lVar7 + 0x20),local_cd8), bVar2 ||
                   ((bVar2 = operator<(local_cd8,(QString *)(lVar7 + 0x20)), !bVar2 &&
                    ((bVar2 = operator<((QString *)(lVar7 + 0x28),local_cd0), bVar2 ||
                     ((bVar2 = operator<(local_cd0,(QString *)(lVar7 + 0x28)), !bVar2 &&
                      ((*(int *)(lVar7 + 0x30) < local_cc8 ||
                       ((*(int *)(lVar7 + 0x30) <= local_cc8 &&
                        ((*(uint *)(lVar7 + 0x34) < local_cc4 ||
                         (((*(uint *)(lVar7 + 0x34) == local_cc4 &&
                           (*(char *)(lVar7 + 0x38) != local_cc0)) && (local_cc0 != '\0'))))))))))))
                    )))))))) {
                lVar24 = *(longlong *)(lVar7 + 0x10);
              }
              else {
                lVar24 = *(longlong *)(lVar7 + 8);
                lVar8 = lVar7;
              }
              lVar7 = lVar24;
            } while (lVar24 != 0);
            if ((lVar8 == 0) ||
               (cVar3 = thunk_FUN_140250490(&local_ce0,lVar8 + 0x18), cVar3 != '\0'))
            goto LAB_140256da1;
          }
          this_01 = (QImage *)(lVar8 + 0x48);
          if (1 < *(uint *)local_ce8) {
            thunk_FUN_140254890(&local_ce8,*(uint *)(local_ce8 + 4));
          }
          ppvVar6 = QListData::begin((QListData *)&local_ce8);
          if (1 < *(uint *)local_ce8) {
            thunk_FUN_140254890(&local_ce8,*(uint *)(local_ce8 + 4));
          }
          ppvVar13 = QListData::end((QListData *)&local_ce8);
          for (; ppvVar6 != ppvVar13; ppvVar6 = ppvVar6 + 1) {
            puVar22 = *ppvVar6;
            local_c68 = puVar22;
            uVar10 = QByteArray::QByteArray(local_c18,(QByteArray *)(puVar22 + 8));
            uVar10 = thunk_FUN_14018d120(local_b80,uVar10,1);
            thunk_FUN_14024d520(&local_c88,uVar10);
            thunk_FUN_14007f2b0(local_b78);
            lVar8 = local_c88;
            if (local_c88 == 0) {
              uVar10 = thunk_FUN_14039be80(local_a90);
              cVar3 = thunk_FUN_14039ba40(uVar10,40000);
              thunk_FUN_14039a8a0(local_a90);
              if (cVar3 != '\0') {
                thunk_FUN_1400764b0(local_8e8,2,1);
                thunk_FUN_14006fb70(local_8e8,"unable to deserialize image");
                uVar10 = thunk_FUN_1400876c0(local_8e8,local_88);
                uVar18 = thunk_FUN_14039be80(local_a80);
                thunk_FUN_14039b070(uVar18,40000,uVar10,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                   );
                thunk_FUN_14039a8a0(local_a80);
                thunk_FUN_140077b90(local_88);
                thunk_FUN_140077af0(local_860);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_860);
              }
              thunk_FUN_14025d190(local_c98,&local_ce0);
              pQVar15 = local_ca8;
              QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
              thunk_FUN_1402541f0(local_c80);
              if (*(uint *)local_ce8 != 0) {
                if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                LOCK();
                *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                UNLOCK();
                if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
              }
              thunk_FUN_140253f50(&local_ce8,local_ce8);
              goto LAB_140257d13;
            }
            if (puVar22[4] != 0) {
              if (puVar22[4] == 1) {
                bVar2 = QImage::isNull(this_01);
                if ((((bVar2) || (iVar4 = QImage::width(this_01), iVar4 < 1)) ||
                    (iVar4 = QImage::height(this_01), iVar4 < 1)) ||
                   (puVar14 = QImage::constBits(this_01), puVar14 == (uchar *)0x0)) {
                  uVar10 = thunk_FUN_14039be80(local_ad0);
                  cVar3 = thunk_FUN_14039ba40(uVar10,40000);
                  thunk_FUN_14039a8a0(local_ad0);
                  if (cVar3 != '\0') {
                    thunk_FUN_1400764b0(local_348,2,1);
                    thunk_FUN_14006fb70(local_348,"unable to process delta frame (broken support)");
                    uVar10 = thunk_FUN_1400876c0(local_348,local_c8);
                    uVar18 = thunk_FUN_14039be80(local_ac0);
                    thunk_FUN_14039b070(uVar18,40000,uVar10,
                                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                       );
                    thunk_FUN_14039a8a0(local_ac0);
                    thunk_FUN_140077b90(local_c8);
                    thunk_FUN_140077af0(local_2c0);
                    std::basic_ios<char,std::char_traits<char>_>::
                    ~basic_ios<char,std::char_traits<char>_>(local_2c0);
                  }
                  thunk_FUN_14025d190(local_c98,&local_ce0);
                  pQVar15 = local_ca8;
                  QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
                  thunk_FUN_1402541f0(local_c80);
                  if (*(uint *)local_ce8 != 0) {
                    if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                    LOCK();
                    *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                    UNLOCK();
                    if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
                  }
                  thunk_FUN_140253f50(&local_ce8,local_ce8);
                }
                else {
                  pQVar28 = (QListData *)(lVar8 + 0x50);
                  iVar4 = QListData::size(pQVar28);
                  local_c30 = (QListData *)(lVar8 + 0x40);
                  iVar5 = QListData::size(local_c30);
                  if (iVar4 == iVar5) {
                    iVar4 = QListData::size(pQVar28);
                    local_c10 = (QListData *)(lVar8 + 0x48);
                    iVar5 = QListData::size(local_c10);
                    if (iVar4 == iVar5) {
                      iVar5 = 0;
                      iVar4 = QListData::size(pQVar28);
                      if (0 < iVar4) {
                        do {
                          piVar9 = (int *)thunk_FUN_140250010(pQVar28,iVar5);
                          cVar3 = thunk_FUN_1400e9150(piVar9);
                          if (cVar3 == '\0') {
                            uVar10 = thunk_FUN_14039be80(local_b10);
                            cVar3 = thunk_FUN_14039ba40(uVar10,40000);
                            thunk_FUN_14039a8a0(local_b10);
                            if (cVar3 != '\0') {
                              thunk_FUN_1400764b0(local_528,2,1);
                              uVar10 = thunk_FUN_1400fc890(&local_ce0,local_bf0);
                              uVar18 = thunk_FUN_14006fb70(local_528,
                                                                                                                      
                                                  "invalid delta subframe in history cache: stream="
                                                  );
                              uVar10 = thunk_FUN_140078de0(uVar18,uVar10);
                              pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                                        thunk_FUN_14006fb70(uVar10,", frameId=");
                              pbVar21 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                                  (pbVar19,*local_c68);
                              pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                                        thunk_FUN_14006fb70(pbVar21,", subframe=");
                              std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                        (pbVar19,iVar5);
                              QString::~QString(local_bf0);
                              uVar10 = thunk_FUN_1400876c0(local_528,local_108);
                              uVar18 = thunk_FUN_14039be80(local_b00);
                              thunk_FUN_14039b070(uVar18,40000,uVar10,
                                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                                 );
                              thunk_FUN_14039a8a0(local_b00);
                              thunk_FUN_140077b90(local_108);
                              thunk_FUN_140077af0(local_4a0);
                              std::basic_ios<char,std::char_traits<char>_>::
                              ~basic_ios<char,std::char_traits<char>_>(local_4a0);
                            }
                            thunk_FUN_14025d190(local_c98,&local_ce0);
                            pQVar15 = local_ca8;
                            QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
                            thunk_FUN_1402541f0(local_c80);
                            if (*(uint *)local_ce8 != 0) {
                              if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                              LOCK();
                              *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                              UNLOCK();
                              if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
                            }
                            thunk_FUN_140253f50(&local_ce8,local_ce8);
                            goto LAB_140257d13;
                          }
                          puVar14 = (uchar *)QByteArray::constData((QByteArray *)(piVar9 + 4));
                          QImage::QImage(local_bd8,puVar14,*piVar9,piVar9[1],piVar9[2],
                                         (_func_void_void_ptr *)0x0,(void *)0x0);
                          bVar2 = QImage::isNull(local_bd8);
                          if (((bVar2) || (iVar4 = QImage::width(local_bd8), iVar4 < 1)) ||
                             ((iVar4 = QImage::height(local_bd8), iVar4 < 1 ||
                              (puVar14 = QImage::constBits(local_bd8), puVar14 == (uchar *)0x0)))) {
LAB_14025743c:
                            uVar10 = thunk_FUN_14039be80(local_b30);
                            cVar3 = thunk_FUN_14039ba40(uVar10,40000);
                            thunk_FUN_14039a8a0(local_b30);
                            if (cVar3 != '\0') {
                              thunk_FUN_1400764b0(local_618,2,1);
                              uVar10 = thunk_FUN_1400fc890(&local_ce0,local_bf8);
                              puVar22 = (uint *)thunk_FUN_1402262d0(local_c88 + 0x48,iVar5);
                              puVar20 = (uint *)thunk_FUN_1402262d0(local_c88 + 0x40,iVar5);
                              uVar18 = thunk_FUN_14006fb70(local_618,
                                                                                                                      
                                                  "unable to combine delta subframe from history cache: stream="
                                                  );
                              uVar10 = thunk_FUN_140078de0(uVar18,uVar10);
                              pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                                        thunk_FUN_14006fb70(uVar10,", frameId=");
                              pbVar21 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                                  (pbVar19,*local_c68);
                              pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                                        thunk_FUN_14006fb70(pbVar21,", subframe=");
                              pbVar21 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                                  (pbVar19,iVar5);
                              pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                                        thunk_FUN_14006fb70(pbVar21,&DAT_140bfb954);
                              pbVar21 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                                  (pbVar19,*puVar20);
                              pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                                        thunk_FUN_14006fb70(pbVar21,&DAT_140bfb94c);
                              std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                        (pbVar19,*puVar22);
                              QString::~QString(local_bf8);
                              uVar10 = thunk_FUN_1400876c0(local_618,local_128);
                              uVar18 = thunk_FUN_14039be80(local_b20);
                              thunk_FUN_14039b070(uVar18,40000,uVar10,
                                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                                 );
                              thunk_FUN_14039a8a0(local_b20);
                              thunk_FUN_140077b90(local_128);
                              thunk_FUN_140077af0(local_590);
                              std::basic_ios<char,std::char_traits<char>_>::
                              ~basic_ios<char,std::char_traits<char>_>(local_590);
                            }
                            thunk_FUN_14025d190(local_c98,&local_ce0);
                            pQVar15 = local_ca8;
                            QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
                            QImage::~QImage(local_bd8);
                            thunk_FUN_1402541f0(local_c80);
                            if (*(uint *)local_ce8 != 0) {
                              if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                              LOCK();
                              *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                              UNLOCK();
                              if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
                            }
                            thunk_FUN_140253f50(&local_ce8,local_ce8);
                            goto LAB_140257d13;
                          }
                          puVar16 = (undefined4 *)thunk_FUN_1402262d0(local_c10,iVar5);
                          puVar17 = (undefined4 *)thunk_FUN_1402262d0(local_c30,iVar5);
                          cVar3 = thunk_FUN_1402522d0(this_01,*puVar17,*puVar16,local_bd8);
                          if (cVar3 == '\0') goto LAB_14025743c;
                          QImage::~QImage(local_bd8);
                          iVar5 = iVar5 + 1;
                          iVar4 = QListData::size(pQVar28);
                        } while (iVar5 < iVar4);
                      }
                      goto LAB_14025713c;
                    }
                  }
                  uVar10 = thunk_FUN_14039be80(local_af0);
                  cVar3 = thunk_FUN_14039ba40(uVar10,40000);
                  thunk_FUN_14039a8a0(local_af0);
                  if (cVar3 != '\0') {
                    thunk_FUN_1400764b0(local_438,2,1);
                    uVar10 = thunk_FUN_1400fc890(&local_ce0,local_be8);
                    uVar18 = thunk_FUN_14006fb70(local_438,
                                                 "invalid delta frame vectors in history cache: stream="
                                                );
                    uVar10 = thunk_FUN_140078de0(uVar18,uVar10);
                    pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                              thunk_FUN_14006fb70(uVar10,", frameId=");
                    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar19,*local_c68)
                    ;
                    QString::~QString(local_be8);
                    uVar10 = thunk_FUN_1400876c0(local_438,local_e8);
                    uVar18 = thunk_FUN_14039be80(local_ae0);
                    thunk_FUN_14039b070(uVar18,40000,uVar10,
                                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                       );
                    thunk_FUN_14039a8a0(local_ae0);
                    thunk_FUN_140077b90(local_e8);
                    thunk_FUN_140077af0(local_3b0);
                    std::basic_ios<char,std::char_traits<char>_>::
                    ~basic_ios<char,std::char_traits<char>_>(local_3b0);
                  }
                  thunk_FUN_14025d190(local_c98,&local_ce0);
                  pQVar15 = local_ca8;
                  QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
                  thunk_FUN_1402541f0(local_c80);
                  if (*(uint *)local_ce8 != 0) {
                    if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                    LOCK();
                    *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                    UNLOCK();
                    if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
                  }
                  thunk_FUN_140253f50(&local_ce8,local_ce8);
                }
              }
              else {
                uVar10 = thunk_FUN_14039be80(local_ab0);
                cVar3 = thunk_FUN_14039ba40(uVar10,40000);
                thunk_FUN_14039a8a0(local_ab0);
                if (cVar3 != '\0') {
                  thunk_FUN_1400764b0(local_258,2,1);
                  uVar10 = thunk_FUN_1400fc890(&local_ce0,local_be0);
                  uVar18 = thunk_FUN_14006fb70(local_258,
                                               "invalid frame status in history cache: stream=");
                  uVar10 = thunk_FUN_140078de0(uVar18,uVar10);
                  pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                            thunk_FUN_14006fb70(uVar10,", frameId=");
                  pbVar21 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                      (pbVar19,*puVar22);
                  pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                            thunk_FUN_14006fb70(pbVar21,", status=");
                  std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar19,puVar22[4]);
                  QString::~QString(local_be0);
                  uVar10 = thunk_FUN_1400876c0(local_258,local_a8);
                  uVar18 = thunk_FUN_14039be80(local_aa0);
                  thunk_FUN_14039b070(uVar18,40000,uVar10,
                                      "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                     );
                  thunk_FUN_14039a8a0(local_aa0);
                  thunk_FUN_140077b90(local_a8);
                  thunk_FUN_140077af0(local_1d0);
                  std::basic_ios<char,std::char_traits<char>_>::
                  ~basic_ios<char,std::char_traits<char>_>(local_1d0);
                }
                thunk_FUN_14025d190(local_c98,&local_ce0);
                pQVar15 = local_ca8;
                QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
                thunk_FUN_1402541f0(local_c80);
                if (*(uint *)local_ce8 != 0) {
                  if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                  LOCK();
                  *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                  UNLOCK();
                  if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
                }
                thunk_FUN_140253f50(&local_ce8,local_ce8);
              }
              goto LAB_140257d13;
            }
            pQVar28 = (QListData *)(local_c88 + 0x50);
            bVar2 = QListData::isEmpty(pQVar28);
            if (bVar2) {
LAB_1402572cc:
              uVar10 = thunk_FUN_14039be80(local_b50);
              cVar3 = thunk_FUN_14039ba40(uVar10,40000);
              thunk_FUN_14039a8a0(local_b50);
              if (cVar3 != '\0') {
                thunk_FUN_1400764b0(local_708,2,1);
                uVar10 = thunk_FUN_1400fc890(&local_ce0,local_c00);
                uVar18 = thunk_FUN_14006fb70(local_708,
                                             "invalid full frame in history cache: stream=");
                uVar10 = thunk_FUN_140078de0(uVar18,uVar10);
                pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                          thunk_FUN_14006fb70(uVar10,", frameId=");
                std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar19,*puVar22);
                QString::~QString(local_c00);
                uVar10 = thunk_FUN_1400876c0(local_708,local_68);
                uVar18 = thunk_FUN_14039be80(local_b40);
                thunk_FUN_14039b070(uVar18,40000,uVar10,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                   );
                thunk_FUN_14039a8a0(local_b40);
                thunk_FUN_140077b90(local_68);
                thunk_FUN_140077af0(local_680);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_680);
              }
              thunk_FUN_14025d190(local_c98,&local_ce0);
              pQVar15 = local_ca8;
              QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
              thunk_FUN_1402541f0(local_c80);
              if (*(uint *)local_ce8 != 0) {
                if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                LOCK();
                *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                UNLOCK();
                if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
              }
              thunk_FUN_140253f50(&local_ce8,local_ce8);
              goto LAB_140257d13;
            }
            uVar10 = thunk_FUN_1402553f0(pQVar28);
            cVar3 = thunk_FUN_1400e9150(uVar10);
            if (cVar3 == '\0') goto LAB_1402572cc;
            piVar9 = (int *)thunk_FUN_140250010(pQVar28,0);
            puVar14 = (uchar *)QByteArray::constData((QByteArray *)(piVar9 + 4));
            pQVar15 = (QImage *)
                      QImage::QImage(local_a08,puVar14,*piVar9,piVar9[1],piVar9[2],
                                     (_func_void_void_ptr *)0x0,(void *)0x0);
            QRect::QRect(local_9e8);
            pQVar15 = (QImage *)QImage::copy(pQVar15,local_a28);
            QImage::operator=(this_01,pQVar15);
            QImage::~QImage((QImage *)local_a28);
            QImage::~QImage(local_a08);
            bVar2 = QImage::isNull(this_01);
            if (((bVar2) || (iVar4 = QImage::width(this_01), iVar4 < 1)) ||
               ((iVar4 = QImage::height(this_01), iVar4 < 1 ||
                (puVar14 = QImage::constBits(this_01), puVar14 == (uchar *)0x0)))) {
              uVar10 = thunk_FUN_14039be80(local_b70);
              cVar3 = thunk_FUN_14039ba40(uVar10,40000);
              thunk_FUN_14039a8a0(local_b70);
              if (cVar3 != '\0') {
                thunk_FUN_1400764b0(local_7f8,2,1);
                uVar10 = thunk_FUN_1400fc890(&local_ce0,local_c08);
                uVar18 = thunk_FUN_14006fb70(local_7f8,
                                             "unable to create full frame from history cache: stream="
                                            );
                uVar10 = thunk_FUN_140078de0(uVar18,uVar10);
                pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                          thunk_FUN_14006fb70(uVar10,", frameId=");
                std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar19,*puVar22);
                QString::~QString(local_c08);
                uVar10 = thunk_FUN_1400876c0(local_7f8,local_148);
                uVar18 = thunk_FUN_14039be80(local_b60);
                thunk_FUN_14039b070(uVar18,40000,uVar10,
                                    "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                   );
                thunk_FUN_14039a8a0(local_b60);
                thunk_FUN_140077b90(local_148);
                thunk_FUN_140077af0(local_770);
                std::basic_ios<char,std::char_traits<char>_>::
                ~basic_ios<char,std::char_traits<char>_>(local_770);
              }
              thunk_FUN_14025d190(local_c98,&local_ce0);
              pQVar15 = local_ca8;
              QImage::QImage(local_ca8,(QImage *)&DAT_140e9fc20);
              thunk_FUN_1402541f0(local_c80);
              if (*(uint *)local_ce8 != 0) {
                if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
                LOCK();
                *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
                UNLOCK();
                if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
              }
              thunk_FUN_140253f50(&local_ce8,local_ce8);
              goto LAB_140257d13;
            }
LAB_14025713c:
            thunk_FUN_1402541f0(local_c80);
            plVar27 = local_c98;
          }
          bVar2 = QImage::isNull(this_01);
          param_2 = local_ca8;
          if ((((!bVar2) && (iVar4 = QImage::width(this_01), param_2 = local_ca8, 0 < iVar4)) &&
              (iVar4 = QImage::height(this_01), param_2 = local_ca8, 0 < iVar4)) &&
             (puVar14 = QImage::constBits(this_01), param_2 = local_ca8, puVar14 != (uchar *)0x0)) {
            puVar22 = (uint *)thunk_FUN_140250130(plVar27,&local_ce0);
            pQVar15 = local_ca8;
            *puVar22 = local_cb0;
            QImage::QImage(local_ca8,this_01);
            if (*(uint *)local_ce8 != 0) {
              if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257d13;
              LOCK();
              *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
              UNLOCK();
              if (*(uint *)local_ce8 != 0) goto LAB_140257d13;
            }
            thunk_FUN_140253f50(&local_ce8,local_ce8);
LAB_140257d13:
            QString::~QString(local_cd0);
            QString::~QString(local_cd8);
            QMutexLocker::~QMutexLocker(local_ca0);
            return pQVar15;
          }
          goto LAB_140257dc9;
        }
      }
      uVar10 = thunk_FUN_14039be80(local_a60);
      cVar3 = thunk_FUN_14039ba40(uVar10,40000);
      thunk_FUN_14039a8a0(local_a60);
      if (cVar3 != '\0') {
        local_9d8 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_950);
        local_c78 = 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_9d8,local_9d0,false);
        *(undefined ***)(local_9d0 + (longlong)*(int *)(local_9d8 + 4) + -8) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_9dc + (longlong)*(int *)(local_9d8 + 4)) =
             *(int *)(local_9d8 + 4) + -0x88;
        thunk_FUN_140076630(local_9d0,2);
        this = (QString *)thunk_FUN_1400fc890(&local_ce0,local_c20);
        uVar10 = thunk_FUN_14006fb70(&local_9d8,
                                     "unable to find support frame in history cache: stream=");
        this_00 = (QByteArray *)QString::toLocal8Bit(this);
        pcVar11 = QByteArray::data(this_00);
        thunk_FUN_14006fb70(uVar10,pcVar11);
        QByteArray::~QByteArray(local_c28);
        pbVar19 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_14006fb70(uVar10,", frameId=");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar19,uVar1);
        QString::~QString(local_c20);
        uVar10 = thunk_FUN_1400876c0(&local_9d8,local_168);
        uVar18 = thunk_FUN_14039be80(local_b90);
        thunk_FUN_14039b070(uVar18,40000,uVar10,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                           );
        thunk_FUN_14039a8a0(local_b90);
        if (0xf < local_150) {
          uVar23 = local_168[0];
          if (0xfff < local_150 + 1) {
            if ((local_168[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar23 = *(ulonglong *)(local_168[0] - 8);
            if (local_168[0] <= uVar23) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_168[0] - uVar23 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_168[0] - uVar23) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar23);
        }
        thunk_FUN_140077af0(local_950);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_950);
      }
      thunk_FUN_14025d190(plVar27,&local_ce0);
      QImage::QImage(param_2,(QImage *)&DAT_140e9fc20);
      piVar9 = (int *)CONCAT44(uStack_cb4,local_cb8);
      if (*piVar9 == 0) {
LAB_140256be7:
        pDVar25 = (Data *)CONCAT44(uStack_cb4,local_cb8);
        pDVar26 = pDVar25 + ((longlong)*(int *)(pDVar25 + 0xc) + 2) * 8;
        iVar4 = *(int *)(pDVar25 + 8);
        while (pDVar25 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
          pDVar26 = pDVar26 + -8;
          lVar8 = *(longlong *)pDVar26;
          if (lVar8 != 0) {
            QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
            QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
            FUN_140008206(lVar8,0x28);
          }
        }
        QListData::dispose(pDVar25);
      }
      else if (*piVar9 != -1) {
        LOCK();
        *piVar9 = *piVar9 + -1;
        UNLOCK();
        if (*piVar9 == 0) goto LAB_140256be7;
      }
      if (*(int *)local_ce8 != 0) {
        if (*(int *)local_ce8 == -1) goto LAB_140257e1a;
        LOCK();
        *(int *)local_ce8 = *(int *)local_ce8 + -1;
        UNLOCK();
        if (*(int *)local_ce8 != 0) goto LAB_140257e1a;
      }
      pDVar26 = local_ce8 + ((longlong)*(int *)(local_ce8 + 0xc) + 2) * 8;
      iVar4 = *(int *)(local_ce8 + 8);
      while (local_ce8 + ((longlong)iVar4 + 2) * 8 != pDVar26) {
        pDVar26 = pDVar26 + -8;
        lVar8 = *(longlong *)pDVar26;
        if (lVar8 != 0) {
          QByteArray::~QByteArray((QByteArray *)(lVar8 + 0x20));
          QDateTime::~QDateTime((QDateTime *)(lVar8 + 8));
          FUN_140008206(lVar8,0x28);
        }
      }
      QListData::dispose(local_ce8);
      goto LAB_140257e1a;
    }
  }
LAB_140257dc9:
  thunk_FUN_14025d190(local_c60 + 0x220,&local_ce0);
  QImage::QImage(param_2,(QImage *)&DAT_140e9fc20);
  if (*(uint *)local_ce8 != 0) {
    if (*(uint *)local_ce8 == 0xffffffff) goto LAB_140257e1a;
    LOCK();
    *(uint *)local_ce8 = *(uint *)local_ce8 - 1;
    UNLOCK();
    if (*(uint *)local_ce8 != 0) goto LAB_140257e1a;
  }
  thunk_FUN_140253f50(&local_ce8,local_ce8);
LAB_140257e1a:
  QString::~QString(local_cd0);
  QString::~QString(local_cd8);
  QMutexLocker::~QMutexLocker(local_ca0);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140259110
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140259110(longlong param_1,longlong param_2,QDateTime *param_3,QDateTime *param_4,
                  QListData *param_5,char param_6)

{
  void *pvVar1;
  longlong lVar2;
  QSqlDatabase *pQVar3;
  bool bVar4;
  char cVar5;
  uint uVar6;
  undefined8 uVar7;
  QSqlError *pQVar8;
  QString *pQVar9;
  QByteArray *pQVar10;
  char *pcVar11;
  undefined8 uVar12;
  QSqlQuery *pQVar13;
  QSqlRecord *this;
  void **ppvVar14;
  void **ppvVar15;
  ulonglong uVar16;
  longlong lVar17;
  uint uVar19;
  undefined1 auStack_288 [32];
  undefined4 local_268;
  QSqlQuery local_258 [8];
  undefined8 local_250;
  QTypedArrayData<unsigned_short> *local_248;
  QVariant local_240 [16];
  QSqlDatabase *local_230;
  QSqlDatabase local_228 [8];
  QVariant local_220 [16];
  undefined8 local_210;
  undefined *local_208;
  undefined **local_200;
  basic_ostream<char,std::char_traits<char>_> local_1f8 [96];
  undefined8 local_198;
  undefined4 local_190;
  basic_ios<char,std::char_traits<char>_> local_180 [104];
  QVariant local_118 [16];
  QVariant local_108 [16];
  undefined8 local_f8;
  QVariant local_f0 [16];
  QVariant local_e0 [16];
  QVariant local_d0 [16];
  QVariant local_c0 [16];
  QVariant local_b0 [16];
  QVariant local_a0 [16];
  ulonglong local_90 [3];
  ulonglong local_78;
  ulonglong local_70 [3];
  ulonglong local_58;
  ulonglong local_50;
  longlong lVar18;
  
  local_f8 = 0xfffffffffffffffe;
  local_50 = DAT_140e88880 ^ (ulonglong)auStack_288;
  uVar19 = 0;
  local_250 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_250 & 0xffffffff00000000);
  local_230 = (QSqlDatabase *)(param_1 + 0x200);
  local_210 = param_4;
  uVar7 = QSqlDatabase::QSqlDatabase(local_228,local_230);
  QSqlQuery::QSqlQuery(local_258,uVar7);
  local_250 = QString::fromAscii_helper
                        ("SELECT id FROM segment WHERE grabberId=?1 AND sessionId=?2 AND displayId=?3 AND start=?4"
                         ,0x58);
  bVar4 = QSqlQuery::prepare(local_258,(QString *)&local_250);
  QString::~QString((QString *)&local_250);
  if (bVar4) {
    pQVar9 = (QString *)thunk_FUN_1404df6e0(&local_248,param_2);
    QVariant::QVariant(local_240,pQVar9);
    QSqlQuery::addBindValue(local_258,local_240,1);
    QVariant::~QVariant(local_240);
    QString::~QString((QString *)&local_248);
    QVariant::QVariant(local_240,(QString *)(param_2 + 8));
    QSqlQuery::addBindValue(local_258,local_240,1);
    QVariant::~QVariant(local_240);
    QVariant::QVariant(local_240,(QString *)(param_2 + 0x10));
    QSqlQuery::addBindValue(local_258,local_240,1);
    QVariant::~QVariant(local_240);
    QVariant::QVariant(local_240,param_3);
    QSqlQuery::addBindValue(local_258,local_240);
    QVariant::~QVariant(local_240);
    bVar4 = QSqlQuery::exec(local_258);
    if (bVar4) {
      bVar4 = QSqlQuery::next(local_258);
      if (bVar4) {
LAB_140259cd4:
        this = (QSqlRecord *)QSqlQuery::record(local_258);
        uVar7 = QSqlRecord::value(this,(int)local_220);
        uVar6 = thunk_FUN_14024df60(uVar7);
        QVariant::~QVariant(local_220);
        QSqlRecord::~QSqlRecord((QSqlRecord *)&local_248);
        ppvVar14 = QListData::begin(param_5);
        ppvVar15 = QListData::end(param_5);
        pQVar3 = local_230;
        for (; ppvVar14 != ppvVar15; ppvVar14 = ppvVar14 + 1) {
          pvVar1 = *ppvVar14;
          QSqlDatabase::QSqlDatabase((QSqlDatabase *)local_240,pQVar3);
          QSqlQuery::QSqlQuery((QSqlQuery *)&local_230);
          lVar18 = -1;
          do {
            lVar17 = lVar18 + 1;
            lVar2 = lVar18 + 1;
            lVar18 = lVar17;
          } while ("INSERT INTO image (pos,status,width,height,format,data,segment_id) VALUES(?1,?2,?3,?4,?5,?6,?7)"
                   [lVar2] != '\0');
          local_248 = QString::fromAscii_helper
                                ("INSERT INTO image (pos,status,width,height,format,data,segment_id) VALUES(?1,?2,?3,?4,?5,?6,?7)"
                                 ,(int)lVar17);
          bVar4 = QSqlQuery::prepare((QSqlQuery *)&local_230,(QString *)&local_248);
          QString::~QString((QString *)&local_248);
          if (!bVar4) {
            uVar7 = thunk_FUN_14039be80(local_90);
            cVar5 = thunk_FUN_14039ba40(uVar7,40000);
            thunk_FUN_14039a8a0(local_90);
            if (cVar5 != '\0') {
              local_208 = &DAT_1405a3878;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_180);
              local_250 = (QTypedArrayData<unsigned_short> *)
                          (CONCAT44(local_250._4_4_,uVar19) | 0x4000);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
              *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
                   *(int *)(local_208 + 4) + -0x88;
              local_210 = (QDateTime *)&local_200;
              std::basic_streambuf<char,std::char_traits<char>_>::
              basic_streambuf<char,std::char_traits<char>_>
                        ((basic_streambuf<char,std::char_traits<char>_> *)&local_200);
              local_200 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                          vftable;
              local_198 = 0;
              local_190 = 4;
              pQVar8 = (QSqlError *)QSqlQuery::lastError((QSqlQuery *)&local_230);
              pQVar9 = (QString *)QSqlError::text(pQVar8);
              uVar7 = thunk_FUN_14006fb70(&local_208,"unable to prepare image store statement: ");
              pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
              pcVar11 = QByteArray::data(pQVar10);
              thunk_FUN_14006fb70(uVar7,pcVar11);
              QByteArray::~QByteArray((QByteArray *)local_240);
              QString::~QString((QString *)local_228);
              QSqlError::~QSqlError((QSqlError *)local_90);
              uVar7 = thunk_FUN_1400876c0(&local_208,local_70);
              uVar12 = thunk_FUN_14039be80(local_90);
              local_268 = 0x25d;
              thunk_FUN_14039b070(uVar12,40000,uVar7,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                 );
              thunk_FUN_14039a8a0(local_90);
              if (0xf < local_58) {
                uVar16 = local_70[0];
                if (0xfff < local_58 + 1) {
                  if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  uVar16 = *(ulonglong *)(local_70[0] - 8);
                  if (local_70[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (local_70[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (0x27 < local_70[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                }
                FUN_140002d6f(uVar16);
              }
              *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
                   *(int *)(local_208 + 4) + -0x88;
              local_200 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                          vftable;
              thunk_FUN_14007bb20(&local_200);
              std::basic_streambuf<char,std::char_traits<char>_>::
              ~basic_streambuf<char,std::char_traits<char>_>
                        ((basic_streambuf<char,std::char_traits<char>_> *)&local_200);
              std::basic_ostream<char,std::char_traits<char>_>::
              ~basic_ostream<char,std::char_traits<char>_>(local_1f8);
LAB_14025a470:
              std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                        (local_180);
            }
LAB_14025a47b:
            QSqlQuery::~QSqlQuery((QSqlQuery *)&local_230);
            goto LAB_14025a92d;
          }
          QVariant::QVariant(local_108,(QDateTime *)((longlong)pvVar1 + 8));
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,local_108,1);
          QVariant::~QVariant(local_108);
          QVariant::QVariant(local_118,*(uint *)((longlong)pvVar1 + 0x10));
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,local_118,1);
          QVariant::~QVariant(local_118);
          if (*(int *)((longlong)pvVar1 + 0x10) == 0) {
            uVar7 = QVariant::QVariant(local_e0,*(uint *)((longlong)pvVar1 + 0x14));
            uVar19 = uVar19 | 2;
          }
          else {
            uVar7 = QVariant::QVariant(local_f0);
            uVar19 = uVar19 | 4;
          }
          local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,uVar19);
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,uVar7,1);
          if ((uVar19 & 4) != 0) {
            uVar19 = uVar19 & 0xfffffffb;
            QVariant::~QVariant(local_f0);
          }
          if ((uVar19 & 2) != 0) {
            uVar19 = uVar19 & 0xfffffffd;
            QVariant::~QVariant(local_e0);
          }
          if (*(int *)((longlong)pvVar1 + 0x10) == 0) {
            uVar7 = QVariant::QVariant(local_c0,*(uint *)((longlong)pvVar1 + 0x18));
            uVar19 = uVar19 | 8;
          }
          else {
            uVar7 = QVariant::QVariant(local_d0);
            uVar19 = uVar19 | 0x10;
          }
          local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,uVar19);
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,uVar7,1);
          if ((uVar19 & 0x10) != 0) {
            uVar19 = uVar19 & 0xffffffef;
            QVariant::~QVariant(local_d0);
          }
          if ((uVar19 & 8) != 0) {
            uVar19 = uVar19 & 0xfffffff7;
            QVariant::~QVariant(local_c0);
          }
          if (*(int *)((longlong)pvVar1 + 0x10) == 0) {
            uVar7 = QVariant::QVariant(local_a0,*(int *)((longlong)pvVar1 + 0x1c));
            uVar19 = uVar19 | 0x20;
          }
          else {
            uVar7 = QVariant::QVariant(local_b0);
            uVar19 = uVar19 | 0x40;
          }
          local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,uVar19);
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,uVar7,1);
          if ((uVar19 & 0x40) != 0) {
            uVar19 = uVar19 & 0xffffffbf;
            QVariant::~QVariant(local_b0);
          }
          if ((uVar19 & 0x20) != 0) {
            uVar19 = uVar19 & 0xffffffdf;
            QVariant::~QVariant(local_a0);
          }
          if (*(int *)((longlong)pvVar1 + 0x10) == 2) {
            uVar7 = QVariant::QVariant((QVariant *)local_70);
            uVar19 = uVar19 | 0x100;
          }
          else {
            uVar7 = QVariant::QVariant((QVariant *)local_90,(QByteArray *)((longlong)pvVar1 + 0x20))
            ;
            uVar19 = uVar19 | 0x80;
          }
          local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,uVar19);
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,uVar7,1);
          if (uVar19 >> 8 != 0) {
            uVar19 = uVar19 & 0xfffffeff;
            QVariant::~QVariant((QVariant *)local_70);
          }
          if ((char)uVar19 < '\0') {
            uVar19 = uVar19 & 0xffffff7f;
            QVariant::~QVariant((QVariant *)local_90);
          }
          QVariant::QVariant(local_220,uVar6);
          QSqlQuery::addBindValue((QSqlQuery *)&local_230,local_220);
          QVariant::~QVariant(local_220);
          bVar4 = QSqlQuery::exec((QSqlQuery *)&local_230);
          if (!bVar4) {
            uVar7 = thunk_FUN_14039be80(local_90);
            cVar5 = thunk_FUN_14039ba40(uVar7,40000);
            thunk_FUN_14039a8a0(local_90);
            if (cVar5 != '\0') {
              local_208 = &DAT_1405a3878;
              std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                        (local_180);
              local_250 = (QTypedArrayData<unsigned_short> *)
                          (CONCAT44(local_250._4_4_,uVar19) | 0x8000);
              std::basic_ostream<char,std::char_traits<char>_>::
              basic_ostream<char,std::char_traits<char>_>
                        ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                         (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
              *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
                   std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
              *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
                   *(int *)(local_208 + 4) + -0x88;
              thunk_FUN_140076630(&local_200,2);
              pQVar8 = (QSqlError *)QSqlQuery::lastError((QSqlQuery *)&local_230);
              pQVar9 = (QString *)QSqlError::text(pQVar8);
              uVar7 = thunk_FUN_14006fb70(&local_208,"unable to exec image store statement: ");
              pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
              pcVar11 = QByteArray::data(pQVar10);
              thunk_FUN_14006fb70(uVar7,pcVar11);
              QByteArray::~QByteArray((QByteArray *)local_240);
              QString::~QString((QString *)local_228);
              QSqlError::~QSqlError((QSqlError *)local_90);
              uVar7 = thunk_FUN_1400876c0(&local_208,local_70);
              uVar12 = thunk_FUN_14039be80(local_90);
              local_268 = 0x26d;
              thunk_FUN_14039b070(uVar12,40000,uVar7,
                                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                                 );
              thunk_FUN_14039a8a0(local_90);
              if (0xf < local_58) {
                uVar16 = local_70[0];
                if (0xfff < local_58 + 1) {
                  if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  uVar16 = *(ulonglong *)(local_70[0] - 8);
                  if (local_70[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (local_70[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                  if (0x27 < local_70[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
                    _invalid_parameter_noinfo_noreturn();
                  }
                }
                FUN_140002d6f(uVar16);
              }
              thunk_FUN_140077af0(local_180);
              goto LAB_14025a470;
            }
            goto LAB_14025a47b;
          }
          QSqlQuery::~QSqlQuery((QSqlQuery *)&local_230);
        }
        if (param_6 == '\0') goto LAB_14025a92d;
        uVar7 = QSqlDatabase::QSqlDatabase((QSqlDatabase *)local_240,pQVar3);
        pQVar13 = (QSqlQuery *)QSqlQuery::QSqlQuery((QSqlQuery *)&local_248,uVar7);
        QSqlQuery::operator=(local_258,pQVar13);
        QSqlQuery::~QSqlQuery((QSqlQuery *)&local_248);
        local_248 = QString::fromAscii_helper("UPDATE segment SET stop = ?1 WHERE id = ?2",0x2a);
        bVar4 = QSqlQuery::prepare(local_258,(QString *)&local_248);
        QString::~QString((QString *)&local_248);
        if (bVar4) {
          QVariant::QVariant(local_220,local_210);
          QSqlQuery::addBindValue(local_258,local_220,1);
          QVariant::~QVariant(local_220);
          QVariant::QVariant(local_118,uVar6);
          QSqlQuery::addBindValue(local_258,local_118);
          QVariant::~QVariant(local_118);
          bVar4 = QSqlQuery::exec(local_258);
          if (bVar4) goto LAB_14025a92d;
          uVar7 = thunk_FUN_14039be80(local_90);
          cVar5 = thunk_FUN_14039ba40(uVar7,40000);
          thunk_FUN_14039a8a0(local_90);
          if (cVar5 == '\0') goto LAB_14025a92d;
          local_208 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_180);
          local_250 = (QTypedArrayData<unsigned_short> *)
                      (CONCAT44(local_250._4_4_,uVar19) | 0x20000);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
          *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
               *(int *)(local_208 + 4) + -0x88;
          thunk_FUN_140076630(&local_200,2);
          pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
          pQVar9 = (QString *)QSqlError::text(pQVar8);
          uVar7 = thunk_FUN_14006fb70(&local_208,"unable to exec segment update statement: ");
          pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
          pcVar11 = QByteArray::data(pQVar10);
          thunk_FUN_14006fb70(uVar7,pcVar11);
          QByteArray::~QByteArray((QByteArray *)&local_210);
          QString::~QString((QString *)local_240);
          QSqlError::~QSqlError((QSqlError *)local_90);
          uVar7 = thunk_FUN_1400876c0(&local_208,local_70);
          uVar12 = thunk_FUN_14039be80(local_90);
          local_268 = 0x280;
          thunk_FUN_14039b070(uVar12,40000,uVar7,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                             );
          thunk_FUN_14039a8a0(local_90);
          if (0xf < local_58) {
            uVar16 = local_70[0];
            if (0xfff < local_58 + 1) {
              if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar16 = *(ulonglong *)(local_70[0] - 8);
              if (local_70[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_70[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_70[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar16);
          }
        }
        else {
          uVar7 = thunk_FUN_14039be80(local_90);
          cVar5 = thunk_FUN_14039ba40(uVar7,40000);
          thunk_FUN_14039a8a0(local_90);
          if (cVar5 == '\0') goto LAB_14025a92d;
          local_208 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_180);
          local_250 = (QTypedArrayData<unsigned_short> *)
                      (CONCAT44(local_250._4_4_,uVar19) | 0x10000);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
          *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
               *(int *)(local_208 + 4) + -0x88;
          thunk_FUN_140076630(&local_200,2);
          pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
          pQVar9 = (QString *)QSqlError::text(pQVar8);
          uVar7 = thunk_FUN_14006fb70(&local_208,"unable to prepare segment update statement: ");
          pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
          pcVar11 = QByteArray::data(pQVar10);
          thunk_FUN_14006fb70(uVar7,pcVar11);
          QByteArray::~QByteArray((QByteArray *)local_240);
          QString::~QString((QString *)local_228);
          QSqlError::~QSqlError((QSqlError *)local_90);
          uVar7 = thunk_FUN_1400876c0(&local_208,local_70);
          uVar12 = thunk_FUN_14039be80(local_90);
          local_268 = 0x277;
          thunk_FUN_14039b070(uVar12,40000,uVar7,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                             );
          thunk_FUN_14039a8a0(local_90);
          if (0xf < local_58) {
            uVar16 = local_70[0];
            if (0xfff < local_58 + 1) {
              if ((local_70[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar16 = *(ulonglong *)(local_70[0] - 8);
              if (local_70[0] <= uVar16) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_70[0] - uVar16 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_70[0] - uVar16) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar16);
          }
        }
      }
      else {
        uVar7 = QSqlDatabase::QSqlDatabase((QSqlDatabase *)&local_248,local_230);
        pQVar13 = (QSqlQuery *)QSqlQuery::QSqlQuery((QSqlQuery *)&local_250,uVar7);
        QSqlQuery::operator=(local_258,pQVar13);
        QSqlQuery::~QSqlQuery((QSqlQuery *)&local_250);
        local_250 = QString::fromAscii_helper
                              ("INSERT INTO segment (grabberId,sessionId,displayId,start,stop) VALUES(?1,?2,?3,?4,NULL)"
                               ,0x57);
        bVar4 = QSqlQuery::prepare(local_258,(QString *)&local_250);
        QString::~QString((QString *)&local_250);
        if (bVar4) {
          pQVar9 = (QString *)thunk_FUN_1404df6e0(&local_248,param_2);
          QVariant::QVariant(local_240,pQVar9);
          QSqlQuery::addBindValue(local_258,local_240,1);
          QVariant::~QVariant(local_240);
          QString::~QString((QString *)&local_248);
          QVariant::QVariant(local_240,(QString *)(param_2 + 8));
          QSqlQuery::addBindValue(local_258,local_240,1);
          QVariant::~QVariant(local_240);
          QVariant::QVariant(local_240,(QString *)(param_2 + 0x10));
          QSqlQuery::addBindValue(local_258,local_240,1);
          QVariant::~QVariant(local_240);
          QVariant::QVariant(local_240,param_3);
          QSqlQuery::addBindValue(local_258,local_240);
          QVariant::~QVariant(local_240);
          bVar4 = QSqlQuery::exec(local_258);
          if (bVar4) {
            local_248 = QString::fromAscii_helper("select last_insert_rowid()",0x1a);
            local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,1);
            bVar4 = QSqlQuery::exec(local_258,(QString *)&local_248);
            if ((bVar4) && (bVar4 = QSqlQuery::next(local_258), bVar4)) {
              bVar4 = false;
            }
            else {
              bVar4 = true;
            }
            QString::~QString((QString *)&local_248);
            if (!bVar4) goto LAB_140259cd4;
            uVar7 = thunk_FUN_14039be80(local_220);
            cVar5 = thunk_FUN_14039ba40(uVar7,40000);
            thunk_FUN_14039a8a0(local_220);
            if (cVar5 == '\0') goto LAB_14025a92d;
            local_208 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_180);
            local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,0x2000);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
            *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
                 *(int *)(local_208 + 4) + -0x88;
            thunk_FUN_140076630(&local_200,2);
            pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
            uVar7 = QSqlError::text(pQVar8);
            uVar12 = thunk_FUN_14006fb70(&local_208,"unable to retrieve stored segment id: ");
            thunk_FUN_140078de0(uVar12,uVar7);
            QString::~QString((QString *)local_228);
            QSqlError::~QSqlError((QSqlError *)local_90);
            uVar7 = thunk_FUN_1400876c0(&local_208,local_90);
            uVar12 = thunk_FUN_14039be80(local_220);
            local_268 = 0x252;
            thunk_FUN_14039b070(uVar12,40000,uVar7,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                               );
            thunk_FUN_14039a8a0(local_220);
            thunk_FUN_140077b90(local_90);
          }
          else {
            uVar7 = thunk_FUN_14039be80(local_220);
            cVar5 = thunk_FUN_14039ba40(uVar7,40000);
            thunk_FUN_14039a8a0(local_220);
            if (cVar5 == '\0') goto LAB_14025a92d;
            local_208 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_180);
            local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,0x1000);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
            *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
                 *(int *)(local_208 + 4) + -0x88;
            thunk_FUN_140076630(&local_200,2);
            pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
            uVar7 = QSqlError::text(pQVar8);
            uVar12 = thunk_FUN_14006fb70(&local_208,"unable to exec segment store statement: ");
            thunk_FUN_140078de0(uVar12,uVar7);
            QString::~QString((QString *)local_228);
            QSqlError::~QSqlError((QSqlError *)local_90);
            uVar7 = thunk_FUN_1400876c0(&local_208,local_90);
            uVar12 = thunk_FUN_14039be80(local_220);
            local_268 = 0x24c;
            thunk_FUN_14039b070(uVar12,40000,uVar7,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                               );
            thunk_FUN_14039a8a0(local_220);
            thunk_FUN_140077b90(local_90);
          }
        }
        else {
          uVar7 = thunk_FUN_14039be80(local_220);
          cVar5 = thunk_FUN_14039ba40(uVar7,40000);
          thunk_FUN_14039a8a0(local_220);
          if (cVar5 == '\0') goto LAB_14025a92d;
          local_208 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_180);
          local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,0x800);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
          *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
               *(int *)(local_208 + 4) + -0x88;
          thunk_FUN_140076630(&local_200,2);
          pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
          pQVar9 = (QString *)QSqlError::text(pQVar8);
          uVar7 = thunk_FUN_14006fb70(&local_208,"unable to prepare segment store statement: ");
          pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
          pcVar11 = QByteArray::data(pQVar10);
          thunk_FUN_14006fb70(uVar7,pcVar11);
          QByteArray::~QByteArray((QByteArray *)local_228);
          QString::~QString((QString *)&local_248);
          QSqlError::~QSqlError((QSqlError *)local_90);
          uVar7 = thunk_FUN_1400876c0(&local_208,local_90);
          uVar12 = thunk_FUN_14039be80(local_220);
          local_268 = 0x241;
          thunk_FUN_14039b070(uVar12,40000,uVar7,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                             );
          thunk_FUN_14039a8a0(local_220);
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
            FUN_140002d6f(uVar16);
          }
        }
      }
    }
    else {
      uVar7 = thunk_FUN_14039be80(local_220);
      cVar5 = thunk_FUN_14039ba40(uVar7,40000);
      thunk_FUN_14039a8a0(local_220);
      if (cVar5 == '\0') goto LAB_14025a92d;
      local_208 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_180);
      local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,0x400);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
      *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
           *(int *)(local_208 + 4) + -0x88;
      thunk_FUN_140076630(&local_200,2);
      pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
      pQVar9 = (QString *)QSqlError::text(pQVar8);
      uVar7 = thunk_FUN_14006fb70(&local_208,"unable to exec segment select statement: ");
      pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
      pcVar11 = QByteArray::data(pQVar10);
      thunk_FUN_14006fb70(uVar7,pcVar11);
      QByteArray::~QByteArray((QByteArray *)local_228);
      QString::~QString((QString *)&local_248);
      QSqlError::~QSqlError((QSqlError *)local_90);
      uVar7 = thunk_FUN_1400876c0(&local_208,local_90);
      uVar12 = thunk_FUN_14039be80(local_220);
      local_268 = 0x238;
      thunk_FUN_14039b070(uVar12,40000,uVar7,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                         );
      thunk_FUN_14039a8a0(local_220);
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
        FUN_140002d6f(uVar16);
      }
    }
    thunk_FUN_140077af0(local_180);
  }
  else {
    uVar7 = thunk_FUN_14039be80(local_220);
    cVar5 = thunk_FUN_14039ba40(uVar7,40000);
    thunk_FUN_14039a8a0(local_220);
    if (cVar5 == '\0') goto LAB_14025a92d;
    local_208 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_180)
    ;
    local_250 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_250._4_4_,0x200);
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_208,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_200,false);
    *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
         *(int *)(local_208 + 4) + -0x88;
    local_210 = (QDateTime *)&local_200;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_200);
    local_200 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_198 = 0;
    local_190 = 4;
    pQVar8 = (QSqlError *)QSqlQuery::lastError(local_258);
    pQVar9 = (QString *)QSqlError::text(pQVar8);
    uVar7 = thunk_FUN_14006fb70(&local_208,"unable to prepare segment select statement: ");
    pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
    pcVar11 = QByteArray::data(pQVar10);
    thunk_FUN_14006fb70(uVar7,pcVar11);
    QByteArray::~QByteArray((QByteArray *)local_228);
    QString::~QString((QString *)&local_248);
    QSqlError::~QSqlError((QSqlError *)local_90);
    uVar7 = thunk_FUN_1400876c0(&local_208,local_90);
    uVar12 = thunk_FUN_14039be80(local_220);
    local_268 = 0x22d;
    thunk_FUN_14039b070(uVar12,40000,uVar7,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_220);
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
      FUN_140002d6f(uVar16);
    }
    *(undefined ***)((longlong)&local_208 + (longlong)*(int *)(local_208 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&local_210 + (longlong)*(int *)(local_208 + 4) + 4) =
         *(int *)(local_208 + 4) + -0x88;
    local_200 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_200);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_200);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_1f8);
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_180);
LAB_14025a92d:
  QSqlQuery::~QSqlQuery(local_258);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025d670
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 *
FUN_14025d670(longlong param_1,undefined8 *param_2,longlong param_3,undefined8 *param_4,char param_5
             ,char param_6,uint *param_7)

{
  Data *pDVar1;
  longlong lVar2;
  undefined4 *puVar3;
  Data *pDVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  undefined8 uVar8;
  QString *pQVar9;
  QDateTime *pQVar10;
  QSqlError *pQVar11;
  QByteArray *pQVar12;
  undefined8 uVar13;
  void **ppvVar14;
  void **ppvVar15;
  QVariant *pQVar16;
  undefined4 *puVar17;
  undefined4 uVar18;
  ulonglong uVar19;
  char *pcVar20;
  Data *pDVar21;
  undefined1 auStack_478 [32];
  undefined4 local_458;
  Data *local_448;
  Data *local_440;
  QSqlQuery local_438 [8];
  QTypedArrayData<unsigned_short> *local_430;
  undefined4 local_428;
  longlong local_420;
  QSqlRecord local_418 [8];
  undefined ***local_410;
  undefined4 local_408;
  undefined4 local_404;
  undefined4 local_400;
  QSqlDatabase local_3f8 [8];
  QString local_3f0 [8];
  QDateTime local_3e8 [8];
  QByteArray local_3e0 [8];
  QString local_3d8 [8];
  QByteArray local_3d0 [8];
  QString local_3c8 [8];
  QByteArray local_3c0 [8];
  QVariant local_3b8 [16];
  QVariant local_3a8 [16];
  QVariant local_398 [16];
  undefined8 local_388;
  undefined1 local_380 [16];
  undefined1 local_370 [16];
  undefined1 local_360 [16];
  undefined1 local_350 [16];
  QVariant local_340 [16];
  QVariant local_330 [16];
  QVariant local_320 [20];
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
  QSqlError local_128 [24];
  QSqlError local_110 [24];
  undefined8 local_f8;
  QDateTime local_f0 [8];
  undefined8 local_e8;
  undefined8 local_e0;
  QByteArray local_d8 [16];
  QVariant local_c8 [16];
  QVariant local_b8 [16];
  QVariant local_a8 [16];
  QVariant local_98 [16];
  ulonglong local_88 [3];
  ulonglong local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_388 = 0xfffffffffffffffe;
  local_48 = DAT_140e88880 ^ (ulonglong)auStack_478;
  local_428 = 0;
  local_448 = (Data *)shared_null_exref;
  uVar8 = QSqlDatabase::QSqlDatabase(local_3f8,(QSqlDatabase *)(param_1 + 0x200));
  QSqlQuery::QSqlQuery(local_438,uVar8);
  local_430 = QString::fromAscii_helper
                        ("SELECT i.id, i.status, i.data FROM image AS i JOIN segment AS s ON (i.segment_id = s.id AND s.grabberId=?1 AND s.sessionId=?2 AND s.displayId=?3 AND s.start <= ?4 AND s.stop >= ?4) WHERE "
                         ,0xbb);
  pQVar9 = (QString *)thunk_FUN_1404df6e0(local_3f0,param_3);
  QVariant::QVariant(local_c8,pQVar9);
  QString::~QString(local_3f0);
  QVariant::QVariant(local_b8,(QString *)(param_3 + 8));
  QVariant::QVariant(local_a8,(QString *)(param_3 + 0x10));
  pQVar10 = (QDateTime *)QDateTime::fromMSecsSinceEpoch((__int64)local_3e8,(TimeSpec)*param_4,1);
  QVariant::QVariant(local_98,pQVar10);
  QDateTime::~QDateTime(local_3e8);
  local_440 = (Data *)shared_null_exref;
  if (*(int *)(shared_null_exref + 4) < 4) {
    if (*(uint *)shared_null_exref < 2) {
      QListData::realloc((QListData *)&local_440,4);
    }
    else {
      thunk_FUN_1400e00d0();
    }
  }
  pQVar16 = local_c8;
  do {
    thunk_FUN_1400d2f20(&local_440,pQVar16);
    pQVar16 = pQVar16 + 0x10;
  } while (pQVar16 != (QVariant *)local_88);
  _eh_vector_destructor_iterator_(local_c8,0x10,4,~QVariant_exref);
  pDVar4 = local_440;
  if (param_5 == '\0') {
    if (param_7[1] < *param_7) {
      *param_2 = local_448;
      local_448 = (Data *)shared_null_exref;
      if (*(int *)local_440 != 0) {
        if (*(int *)local_440 == -1) goto LAB_14025d8e1;
        LOCK();
        *(int *)local_440 = *(int *)local_440 + -1;
        UNLOCK();
        if (*(int *)local_440 != 0) goto LAB_14025d8e1;
      }
      pDVar21 = local_440 + ((longlong)*(int *)(local_440 + 0xc) + 2) * 8;
      pDVar1 = local_440 + ((longlong)*(int *)(local_440 + 8) + 2) * 8;
      while (pDVar1 != pDVar21) {
        pDVar21 = pDVar21 + -8;
        pQVar16 = *(QVariant **)pDVar21;
        if (pQVar16 != (QVariant *)0x0) {
          QVariant::~QVariant(pQVar16);
          FUN_140008206(pQVar16,0x10);
        }
      }
      QListData::dispose(pDVar4);
LAB_14025d8e1:
      QString::~QString((QString *)&local_430);
      QSqlQuery::~QSqlQuery(local_438);
      pDVar4 = local_448;
      if (*(int *)local_448 != 0) {
        if (*(int *)local_448 == -1) {
          return param_2;
        }
        LOCK();
        *(int *)local_448 = *(int *)local_448 + -1;
        UNLOCK();
        if (*(int *)local_448 != 0) {
          return param_2;
        }
      }
      pDVar21 = local_448 + ((longlong)*(int *)(local_448 + 0xc) + 2) * 8;
      pDVar1 = local_448 + ((longlong)*(int *)(local_448 + 8) + 2) * 8;
      while (pDVar1 != pDVar21) {
        pDVar21 = pDVar21 + -8;
        lVar2 = *(longlong *)pDVar21;
        if (lVar2 != 0) {
          QByteArray::~QByteArray((QByteArray *)(lVar2 + 0x20));
          QDateTime::~QDateTime((QDateTime *)(lVar2 + 8));
          FUN_140008206(lVar2,0x28);
        }
      }
      QListData::dispose(pDVar4);
      return param_2;
    }
    QString::operator+=((QString *)&local_430,"i.id >= ?5 AND i.id <= ?6 ");
    QVariant::QVariant(local_398,param_7[1]);
    QVariant::QVariant(local_3a8,*param_7);
    thunk_FUN_1400d2f20(&local_440,local_3a8);
    thunk_FUN_1400d2f20(&local_440,local_398);
    QVariant::~QVariant(local_3a8);
    QVariant::~QVariant(local_398);
    pcVar20 = "ORDER BY i.id DESC";
  }
  else {
    QString::operator+=((QString *)&local_430,"i.pos <= ?4 ");
    if (param_6 != '\0') {
      QString::operator+=((QString *)&local_430,"AND i.status = ?5 ");
      QVariant::QVariant(local_3b8,0);
      thunk_FUN_1400d2f20(&local_440,local_3b8);
      QVariant::~QVariant(local_3b8);
    }
    pcVar20 = "ORDER BY i.id DESC LIMIT 1";
  }
  QString::operator+=((QString *)&local_430,pcVar20);
  bVar5 = QSqlQuery::prepare(local_438,(QString *)&local_430);
  if (bVar5) {
    if (1 < *(uint *)local_440) {
      thunk_FUN_1400e00d0(&local_440,*(uint *)(local_440 + 4));
    }
    ppvVar14 = QListData::begin((QListData *)&local_440);
    if (1 < *(uint *)local_440) {
      thunk_FUN_1400e00d0(&local_440,*(uint *)(local_440 + 4));
    }
    ppvVar15 = QListData::end((QListData *)&local_440);
    if (ppvVar14 != ppvVar15) {
      local_400 = 1;
      do {
        QSqlQuery::addBindValue(local_438,*ppvVar14);
        ppvVar14 = ppvVar14 + 1;
      } while (ppvVar14 != ppvVar15);
    }
    bVar5 = QSqlQuery::exec(local_438);
    if (bVar5) {
      bVar5 = QSqlQuery::next(local_438);
      if (bVar5) {
        do {
          QSqlQuery::record(local_438);
          local_f8 = 0;
          QDateTime::QDateTime(local_f0);
          local_e8 = 0;
          local_e0 = 0;
          QByteArray::QByteArray(local_d8);
          thunk_FUN_14025ca30(&local_448,&local_f8);
          QByteArray::~QByteArray(local_d8);
          QDateTime::~QDateTime(local_f0);
          bVar5 = QListData::isEmpty((QListData *)&local_448);
          if (bVar5) {
            qt_assert("!isEmpty()",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x14f);
          }
          if (1 < *(uint *)local_448) {
            thunk_FUN_140254890(&local_448,*(uint *)(local_448 + 4));
          }
          ppvVar14 = QListData::begin((QListData *)&local_448);
          puVar3 = *ppvVar14;
          pQVar16 = (QVariant *)QSqlRecord::value(local_418,(int)local_340);
          iVar7 = QVariant::userType(pQVar16);
          if (iVar7 == 3) {
            puVar17 = QVariant::constData(pQVar16);
            uVar18 = *puVar17;
          }
          else {
            bVar5 = QVariant::convert(pQVar16,3,&local_408);
            uVar18 = 0;
            if (bVar5) {
              uVar18 = local_408;
            }
          }
          *puVar3 = uVar18;
          QVariant::~QVariant(local_340);
          pQVar16 = (QVariant *)QSqlRecord::value(local_418,(int)local_330);
          iVar7 = QVariant::userType(pQVar16);
          if (iVar7 == 3) {
            puVar17 = QVariant::constData(pQVar16);
            uVar18 = *puVar17;
          }
          else {
            bVar5 = QVariant::convert(pQVar16,3,&local_404);
            uVar18 = 0;
            if (bVar5) {
              uVar18 = local_404;
            }
          }
          puVar3[4] = uVar18;
          QVariant::~QVariant(local_330);
          uVar8 = QSqlRecord::value(local_418,(int)local_320);
          pQVar12 = (QByteArray *)thunk_FUN_14015b440(uVar8,local_3c0);
          QByteArray::operator=((QByteArray *)(puVar3 + 8),pQVar12);
          QByteArray::~QByteArray(local_3c0);
          QVariant::~QVariant(local_320);
          thunk_FUN_14015b1c0(&local_420,puVar3 + 8);
          if (*(int *)(local_420 + 4) == 0) {
            pQVar12 = (QByteArray *)thunk_FUN_1403d6c80(&local_410,puVar3 + 8);
            QByteArray::operator=((QByteArray *)&local_420,pQVar12);
            QByteArray::~QByteArray((QByteArray *)&local_410);
          }
          QByteArray::operator=((QByteArray *)(puVar3 + 8),(QByteArray *)&local_420);
          QByteArray::~QByteArray((QByteArray *)&local_420);
          QSqlRecord::~QSqlRecord(local_418);
          bVar5 = QSqlQuery::next(local_438);
        } while (bVar5);
      }
      pDVar4 = local_440;
      *param_2 = local_448;
      local_448 = (Data *)shared_null_exref;
      if (*(int *)local_440 != 0) {
        if (*(int *)local_440 == -1) goto LAB_14025e453;
        LOCK();
        *(int *)local_440 = *(int *)local_440 + -1;
        UNLOCK();
        if (*(int *)local_440 != 0) goto LAB_14025e453;
      }
      pDVar21 = local_440 + ((longlong)*(int *)(local_440 + 0xc) + 2) * 8;
      pDVar1 = local_440 + ((longlong)*(int *)(local_440 + 8) + 2) * 8;
      while (pDVar1 != pDVar21) {
        pDVar21 = pDVar21 + -8;
        pQVar16 = *(QVariant **)pDVar21;
        if (pQVar16 != (QVariant *)0x0) {
          QVariant::~QVariant(pQVar16);
          FUN_140008206(pQVar16,0x10);
        }
      }
      QListData::dispose(pDVar4);
LAB_14025e453:
      QString::~QString((QString *)&local_430);
      QSqlQuery::~QSqlQuery(local_438);
      pDVar4 = local_448;
      if (*(int *)local_448 != 0) {
        if (*(int *)local_448 == -1) {
          return param_2;
        }
        LOCK();
        *(int *)local_448 = *(int *)local_448 + -1;
        UNLOCK();
        if (*(int *)local_448 != 0) {
          return param_2;
        }
      }
      pDVar21 = local_448 + ((longlong)*(int *)(local_448 + 0xc) + 2) * 8;
      pDVar1 = local_448 + ((longlong)*(int *)(local_448 + 8) + 2) * 8;
      while (pDVar1 != pDVar21) {
        pDVar21 = pDVar21 + -8;
        lVar2 = *(longlong *)pDVar21;
        if (lVar2 != 0) {
          QByteArray::~QByteArray((QByteArray *)(lVar2 + 0x20));
          QDateTime::~QDateTime((QDateTime *)(lVar2 + 8));
          FUN_140008206(lVar2,0x28);
        }
      }
      QListData::dispose(pDVar4);
      return param_2;
    }
    uVar8 = thunk_FUN_14039be80(local_360);
    cVar6 = thunk_FUN_14039ba40(uVar8,40000);
    thunk_FUN_14039a8a0(local_360);
    if (cVar6 != '\0') {
      local_218 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_190);
      local_428 = 4;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_218,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_210,false);
      *(undefined ***)((longlong)&local_218 + (longlong)*(int *)(local_218 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_21c + (longlong)*(int *)(local_218 + 4)) =
           *(int *)(local_218 + 4) + -0x88;
      local_410 = &local_210;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_210);
      local_210 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_1a8 = 0;
      local_1a0 = 4;
      pQVar11 = (QSqlError *)QSqlQuery::lastError(local_438);
      pQVar9 = (QString *)QSqlError::text(pQVar11);
      uVar8 = thunk_FUN_14006fb70(&local_218,"unable to exec history select-image statement: ");
      pQVar12 = (QByteArray *)QString::toLocal8Bit(pQVar9);
      pcVar20 = QByteArray::data(pQVar12);
      thunk_FUN_14006fb70(uVar8,pcVar20);
      QByteArray::~QByteArray(local_3d0);
      QString::~QString(local_3c8);
      QSqlError::~QSqlError(local_110);
      uVar8 = thunk_FUN_1400876c0(&local_218,local_68);
      uVar13 = thunk_FUN_14039be80(local_350);
      local_458 = 0x2b9;
      thunk_FUN_14039b070(uVar13,40000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                         );
      thunk_FUN_14039a8a0(local_350);
      if (0xf < local_50) {
        uVar19 = local_68[0];
        if (0xfff < local_50 + 1) {
          if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar19 = *(ulonglong *)(local_68[0] - 8);
          if (local_68[0] <= uVar19) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_68[0] - uVar19 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_68[0] - uVar19) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar19);
      }
      *(undefined ***)((longlong)&local_218 + (longlong)*(int *)(local_218 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_21c + (longlong)*(int *)(local_218 + 4)) =
           *(int *)(local_218 + 4) + -0x88;
      local_210 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_210);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_210);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_208);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_190);
    }
    pDVar4 = local_440;
    *param_2 = local_448;
    local_448 = (Data *)shared_null_exref;
    if (*(int *)local_440 != 0) {
      if (*(int *)local_440 == -1) goto LAB_14025e0e1;
      LOCK();
      *(int *)local_440 = *(int *)local_440 + -1;
      UNLOCK();
      if (*(int *)local_440 != 0) goto LAB_14025e0e1;
    }
    pDVar21 = local_440 + ((longlong)*(int *)(local_440 + 0xc) + 2) * 8;
    pDVar1 = local_440 + ((longlong)*(int *)(local_440 + 8) + 2) * 8;
    while (pDVar1 != pDVar21) {
      pDVar21 = pDVar21 + -8;
      pQVar16 = *(QVariant **)pDVar21;
      if (pQVar16 != (QVariant *)0x0) {
        QVariant::~QVariant(pQVar16);
        FUN_140008206(pQVar16,0x10);
      }
    }
    QListData::dispose(pDVar4);
LAB_14025e0e1:
    QString::~QString((QString *)&local_430);
    QSqlQuery::~QSqlQuery(local_438);
    pDVar4 = local_448;
    if (*(int *)local_448 != 0) {
      if (*(int *)local_448 == -1) {
        return param_2;
      }
      LOCK();
      *(int *)local_448 = *(int *)local_448 + -1;
      UNLOCK();
      if (*(int *)local_448 != 0) {
        return param_2;
      }
    }
    pDVar21 = local_448 + ((longlong)*(int *)(local_448 + 0xc) + 2) * 8;
    pDVar1 = local_448 + ((longlong)*(int *)(local_448 + 8) + 2) * 8;
    while (pDVar1 != pDVar21) {
      pDVar21 = pDVar21 + -8;
      lVar2 = *(longlong *)pDVar21;
      if (lVar2 != 0) {
        QByteArray::~QByteArray((QByteArray *)(lVar2 + 0x20));
        QDateTime::~QDateTime((QDateTime *)(lVar2 + 8));
        FUN_140008206(lVar2,0x28);
      }
    }
    QListData::dispose(pDVar4);
    return param_2;
  }
  uVar8 = thunk_FUN_14039be80(local_380);
  cVar6 = thunk_FUN_14039ba40(uVar8,40000);
  thunk_FUN_14039a8a0(local_380);
  if (cVar6 != '\0') {
    local_308 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_280)
    ;
    local_428 = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_308,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_300,false);
    *(undefined ***)((longlong)&local_308 + (longlong)*(int *)(local_308 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_30c + (longlong)*(int *)(local_308 + 4)) =
         *(int *)(local_308 + 4) + -0x88;
    local_410 = &local_300;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_300);
    local_300 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_298 = 0;
    local_290 = 4;
    pQVar11 = (QSqlError *)QSqlQuery::lastError(local_438);
    pQVar9 = (QString *)QSqlError::text(pQVar11);
    uVar8 = thunk_FUN_14006fb70(&local_308,"unable to prepare history select-image statement: ");
    pQVar12 = (QByteArray *)QString::toLocal8Bit(pQVar9);
    pcVar20 = QByteArray::data(pQVar12);
    thunk_FUN_14006fb70(uVar8,pcVar20);
    QByteArray::~QByteArray(local_3e0);
    QString::~QString(local_3d8);
    QSqlError::~QSqlError(local_128);
    uVar8 = thunk_FUN_1400876c0(&local_308,local_88);
    uVar13 = thunk_FUN_14039be80(local_370);
    local_458 = 0x2ae;
    thunk_FUN_14039b070(uVar13,40000,uVar8,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_370);
    if (0xf < local_70) {
      uVar19 = local_88[0];
      if (0xfff < local_70 + 1) {
        if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar19 = *(ulonglong *)(local_88[0] - 8);
        if (local_88[0] <= uVar19) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_88[0] - uVar19 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_88[0] - uVar19) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar19);
    }
    *(undefined ***)((longlong)&local_308 + (longlong)*(int *)(local_308 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_30c + (longlong)*(int *)(local_308 + 4)) =
         *(int *)(local_308 + 4) + -0x88;
    local_300 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_300);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_300);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_2f8);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_280);
  }
  pDVar4 = local_440;
  *param_2 = local_448;
  local_448 = (Data *)shared_null_exref;
  if (*(int *)local_440 != 0) {
    if (*(int *)local_440 == -1) goto LAB_14025dcc1;
    LOCK();
    *(int *)local_440 = *(int *)local_440 + -1;
    UNLOCK();
    if (*(int *)local_440 != 0) goto LAB_14025dcc1;
  }
  pDVar21 = local_440 + ((longlong)*(int *)(local_440 + 0xc) + 2) * 8;
  pDVar1 = local_440 + ((longlong)*(int *)(local_440 + 8) + 2) * 8;
  while (pDVar1 != pDVar21) {
    pDVar21 = pDVar21 + -8;
    pQVar16 = *(QVariant **)pDVar21;
    if (pQVar16 != (QVariant *)0x0) {
      QVariant::~QVariant(pQVar16);
      FUN_140008206(pQVar16,0x10);
    }
  }
  QListData::dispose(pDVar4);
LAB_14025dcc1:
  QString::~QString((QString *)&local_430);
  QSqlQuery::~QSqlQuery(local_438);
  pDVar4 = local_448;
  if (*(int *)local_448 != 0) {
    if (*(int *)local_448 == -1) {
      return param_2;
    }
    LOCK();
    *(int *)local_448 = *(int *)local_448 + -1;
    UNLOCK();
    if (*(int *)local_448 != 0) {
      return param_2;
    }
  }
  pDVar21 = local_448 + ((longlong)*(int *)(local_448 + 0xc) + 2) * 8;
  pDVar1 = local_448 + ((longlong)*(int *)(local_448 + 8) + 2) * 8;
  while (pDVar1 != pDVar21) {
    pDVar21 = pDVar21 + -8;
    lVar2 = *(longlong *)pDVar21;
    if (lVar2 != 0) {
      QByteArray::~QByteArray((QByteArray *)(lVar2 + 0x20));
      QDateTime::~QDateTime((QDateTime *)(lVar2 + 8));
      FUN_140008206(lVar2,0x28);
    }
  }
  QListData::dispose(pDVar4);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14025f120
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_14025f120(longlong *param_1,QString *param_2)

{
  QSqlDatabase *this;
  int iVar1;
  void *pvVar2;
  longlong lVar3;
  char cVar4;
  bool bVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  QSqlDatabase *pQVar8;
  QSqlError *this_00;
  QString *this_01;
  QByteArray *this_02;
  char *pcVar9;
  ulonglong uVar10;
  Data *pDVar11;
  undefined1 auStack_418 [32];
  undefined4 local_3f8;
  undefined4 local_3e8;
  Data *local_3e0;
  Data *local_3d8;
  void **local_3d0;
  void **local_3c8;
  undefined4 local_3c0;
  QTypedArrayData<unsigned_short> *local_3b8;
  QTypedArrayData<unsigned_short> *local_3b0;
  QTypedArrayData<unsigned_short> *local_3a8;
  QTypedArrayData<unsigned_short> *local_3a0;
  QTypedArrayData<unsigned_short> *local_398;
  QTypedArrayData<unsigned_short> *local_390;
  QTypedArrayData<unsigned_short> *local_388;
  QTypedArrayData<unsigned_short> *local_380;
  QTypedArrayData<unsigned_short> *local_378;
  QTypedArrayData<unsigned_short> *local_370;
  QTypedArrayData<unsigned_short> *local_368;
  QTypedArrayData<unsigned_short> *local_360;
  undefined ***local_358;
  undefined8 local_350;
  undefined8 local_348;
  undefined8 local_340;
  undefined8 local_338;
  undefined8 uStack_330;
  undefined8 local_328;
  QString local_320 [8];
  QSqlQuery local_318 [8];
  QString local_310 [8];
  QString local_308 [8];
  QString local_300 [8];
  QString local_2f8 [8];
  QString local_2f0 [8];
  QString local_2e8 [8];
  QString local_2e0 [8];
  QString local_2d8 [8];
  QString local_2d0 [8];
  QByteArray local_2c8 [8];
  QString local_2c0 [8];
  undefined1 local_2b8 [16];
  undefined1 local_2a8 [16];
  undefined1 local_298 [16];
  undefined8 local_288;
  undefined1 local_280 [20];
  int iStack_26c;
  undefined *local_268;
  undefined **local_260;
  basic_ostream<char,std::char_traits<char>_> local_258 [96];
  undefined8 local_1f8;
  undefined4 local_1f0;
  basic_ios<char,std::char_traits<char>_> local_1e0 [104];
  QSqlError local_178 [28];
  int iStack_15c;
  undefined *local_158;
  basic_streambuf<char,std::char_traits<char>_> local_150 [128];
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48 [3];
  ulonglong local_30;
  ulonglong local_28;
  
  local_288 = 0xfffffffffffffffe;
  local_28 = DAT_140e88880 ^ (ulonglong)auStack_418;
  local_3e8 = 0;
  if (*(int *)(*(longlong *)param_2 + 4) == 0) {
    uVar6 = thunk_FUN_14039be80(local_280);
    cVar4 = thunk_FUN_14039ba40(uVar6,30000);
    thunk_FUN_14039a8a0(local_280);
    if (cVar4 == '\0') {
      return;
    }
    local_268 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_1e0)
    ;
    local_3e8 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_268,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_260,false);
    *(undefined ***)((longlong)&local_268 + (longlong)*(int *)(local_268 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_26c + (longlong)*(int *)(local_268 + 4)) =
         *(int *)(local_268 + 4) + -0x88;
    local_358 = &local_260;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_260);
    local_260 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_1f8 = 0;
    local_1f0 = 4;
    thunk_FUN_14006fb70(&local_268,"no video cache file provided");
    uVar6 = thunk_FUN_1400876c0(&local_268,local_68);
    uVar7 = thunk_FUN_14039be80(local_2b8);
    local_3f8 = 0x75;
    thunk_FUN_14039b070(uVar7,30000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_2b8);
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
    return;
  }
  if ((param_1[0x3a] != 0) && (param_1[0x3b] != 0)) {
    thunk_FUN_140251830(param_1 + 0x3d,param_1 + 0x37);
    if (param_1[0x3b] < param_1[0x3a]) {
      qt_assert("_accumulateSegment._stop>=_accumulateSegment._start",
                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                ,0x7d);
    }
    local_348 = 0;
    local_340 = 0;
    local_338 = 0;
    uStack_330 = 0;
    local_328 = 0;
    local_350 = 0;
    QString::QString((QString *)&local_348);
    QString::QString((QString *)&local_340);
    local_338 = 0;
    uStack_330 = 0;
    local_328 = 0;
    thunk_FUN_14024fe80(param_1 + 0x37,&local_350);
    QString::~QString((QString *)&local_340);
    QString::~QString((QString *)&local_348);
  }
  QString::operator=((QString *)(param_1 + 0x41),param_2);
  local_3b8 = QString::fromAscii_helper("QSQLITE",7);
  pQVar8 = (QSqlDatabase *)QSqlDatabase::addDatabase(local_2d0,(QString *)&local_3b8);
  this = (QSqlDatabase *)(param_1 + 0x40);
  QSqlDatabase::operator=(this,pQVar8);
  QSqlDatabase::~QSqlDatabase((QSqlDatabase *)local_2d0);
  QString::~QString((QString *)&local_3b8);
  QSqlDatabase::setDatabaseName(this,(QString *)(param_1 + 0x41));
  bVar5 = QSqlDatabase::open(this);
  if (!bVar5) {
    uVar6 = thunk_FUN_14039be80(local_2a8);
    cVar4 = thunk_FUN_14039ba40(uVar6,40000);
    thunk_FUN_14039a8a0(local_2a8);
    if (cVar4 == '\0') {
      return;
    }
    local_158 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_d0);
    local_3e8 = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_158,local_150,false);
    *(undefined ***)(local_150 + (longlong)*(int *)(local_158 + 4) + -8) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
         *(int *)(local_158 + 4) + -0x88;
    thunk_FUN_140076630(local_150,2);
    this_00 = (QSqlError *)QSqlDatabase::lastError(this);
    this_01 = (QString *)QSqlError::text(this_00);
    uVar6 = thunk_FUN_14006fb70(&local_158,"unable to open video history database: ");
    this_02 = (QByteArray *)QString::toLocal8Bit(this_01);
    pcVar9 = QByteArray::data(this_02);
    thunk_FUN_14006fb70(uVar6,pcVar9);
    QByteArray::~QByteArray(local_2c8);
    QString::~QString(local_2c0);
    QSqlError::~QSqlError(local_178);
    uVar6 = thunk_FUN_1400876c0(&local_158,local_48);
    uVar7 = thunk_FUN_14039be80(local_298);
    local_3f8 = 0x87;
    thunk_FUN_14039b070(uVar7,40000,uVar6,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_298);
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
    thunk_FUN_140077af0(local_d0);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_d0)
    ;
    return;
  }
  local_3b0 = QString::fromAscii_helper("PRAGMA encoding=UTF-8",0x15);
  QSqlDatabase::exec(this,local_320);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_320);
  QString::~QString((QString *)&local_3b0);
  local_3a8 = QString::fromAscii_helper("PRAGMA auto_vacuum=NONE",0x17);
  QSqlDatabase::exec(this,(QString *)local_318);
  QSqlQuery::~QSqlQuery(local_318);
  QString::~QString((QString *)&local_3a8);
  local_3a0 = QString::fromAscii_helper("PRAGMA fullfsync=OFF",0x14);
  QSqlDatabase::exec(this,local_310);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_310);
  QString::~QString((QString *)&local_3a0);
  local_398 = QString::fromAscii_helper("PRAGMA synchronous=OFF",0x16);
  QSqlDatabase::exec(this,local_308);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_308);
  QString::~QString((QString *)&local_398);
  local_390 = QString::fromAscii_helper("PRAGMA journal_mode=OFF",0x17);
  QSqlDatabase::exec(this,local_300);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_300);
  QString::~QString((QString *)&local_390);
  local_388 = QString::fromAscii_helper("PRAGMA foreign_keys=ON",0x16);
  QSqlDatabase::exec(this,local_2f8);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_2f8);
  QString::~QString((QString *)&local_388);
  local_380 = QString::fromAscii_helper
                        ("CREATE TABLE IF NOT EXISTS segment (\ngrabberId TEXT,\nsessionId TEXT,\ndisplayId TEXT,\nstart TIMESTAMP,\nstop TIMESTAMP,\nid INTEGER PRIMARY KEY\n)"
                         ,0x8e);
  QSqlDatabase::exec(this,local_2f0);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_2f0);
  QString::~QString((QString *)&local_380);
  local_378 = QString::fromAscii_helper
                        ("CREATE INDEX IF NOT EXISTS segment_idx ON segment(grabberId, sessionId, displayId, start)"
                         ,0x59);
  QSqlDatabase::exec(this,local_2e8);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_2e8);
  QString::~QString((QString *)&local_378);
  local_370 = QString::fromAscii_helper
                        ("CREATE TABLE IF NOT EXISTS image (\npos TIMESTAMP,\nstatus INTEGER,\nwidth INTEGER,\nheight INTEGER,\nformat INTEGER,\ndata BLOB,\nid INTEGER PRIMARY KEY,\nsegment_id INTEGER, FOREIGN KEY (segment_id) REFERENCES segment (id) ON DELETE CASCADE\n)"
                         ,0xec);
  QSqlDatabase::exec(this,local_2e0);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_2e0);
  QString::~QString((QString *)&local_370);
  local_368 = QString::fromAscii_helper
                        ("CREATE INDEX IF NOT EXISTS image_segment_id_idx ON image(segment_id)",0x44
                        );
  QSqlDatabase::exec(this,local_2d8);
  QSqlQuery::~QSqlQuery((QSqlQuery *)local_2d8);
  QString::~QString((QString *)&local_368);
  local_360 = QString::fromAscii_helper("DELETE FROM segment WHERE stop IS NULL",0x26);
  QSqlDatabase::exec(this,(QString *)&local_358);
  QSqlQuery::~QSqlQuery((QSqlQuery *)&local_358);
  QString::~QString((QString *)&local_360);
  thunk_FUN_140252770(param_1);
  thunk_FUN_140251ba0(param_1,&local_3e0);
  thunk_FUN_14024e430(&local_3d8,&local_3e0);
  local_3d0 = QListData::begin((QListData *)&local_3d8);
  local_3c8 = QListData::end((QListData *)&local_3d8);
  for (; local_3c0 = 1, local_3d0 != local_3c8; local_3d0 = local_3d0 + 1) {
    pvVar2 = *local_3d0;
    (**(code **)(*param_1 + 0xd8))(param_1,pvVar2,(longlong)pvVar2 + 0x28,(longlong)pvVar2 + 0x30);
  }
  local_3c0 = 1;
  if (*(int *)local_3d8 != 0) {
    if (*(int *)local_3d8 == -1) goto LAB_14025fa39;
    LOCK();
    iVar1 = *(int *)local_3d8;
    *(int *)local_3d8 = *(int *)local_3d8 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_14025fa39;
  }
  thunk_FUN_14025ba40(&local_3d8,local_3d8 + ((longlong)*(int *)(local_3d8 + 8) + 2) * 8,
                      local_3d8 + ((longlong)*(int *)(local_3d8 + 0xc) + 2) * 8);
  QListData::dispose(local_3d8);
LAB_14025fa39:
  bVar5 = QListData::isEmpty((QListData *)(param_1 + 0x3d));
  if (!bVar5) {
    thunk_FUN_140260160(param_1);
  }
  if (*(int *)local_3e0 != 0) {
    if (*(int *)local_3e0 == -1) {
      return;
    }
    LOCK();
    iVar1 = *(int *)local_3e0;
    *(int *)local_3e0 = *(int *)local_3e0 + -1;
    UNLOCK();
    if (iVar1 != 1) {
      return;
    }
  }
  pDVar11 = local_3e0 + ((longlong)*(int *)(local_3e0 + 0xc) + 2) * 8;
  iVar1 = *(int *)(local_3e0 + 8);
  while (local_3e0 + ((longlong)iVar1 + 2) * 8 != pDVar11) {
    pDVar11 = pDVar11 + -8;
    lVar3 = *(longlong *)pDVar11;
    if (lVar3 != 0) {
      QDateTime::~QDateTime((QDateTime *)(lVar3 + 0x30));
      QDateTime::~QDateTime((QDateTime *)(lVar3 + 0x28));
      QString::~QString((QString *)(lVar3 + 0x10));
      QString::~QString((QString *)(lVar3 + 8));
      FUN_140008206(lVar3,0x38);
    }
  }
  QListData::dispose(local_3e0);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140282460
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140282460(longlong param_1,QString *param_2)

{
  bool bVar1;
  char cVar2;
  undefined8 uVar3;
  QByteArray *this;
  char *pcVar4;
  undefined8 uVar5;
  ulonglong uVar6;
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
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_198;
  local_168[0] = 0;
  bVar1 = operator==((QString *)(param_1 + 0x60),param_2);
  if (!bVar1) {
    QString::operator=((QString *)(param_1 + 0x60),param_2);
    thunk_FUN_1402810d0(param_1);
  }
  uVar3 = thunk_FUN_14039be80(local_160);
  cVar2 = thunk_FUN_14039ba40(uVar3,40000);
  thunk_FUN_14039a8a0(local_160);
  if (cVar2 != '\0') {
    local_148 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_c0);
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
    this = (QByteArray *)QString::toLocal8Bit(param_2);
    pcVar4 = QByteArray::data(this);
    thunk_FUN_14006fb70(&local_148,pcVar4);
    QByteArray::~QByteArray((QByteArray *)local_168);
    uVar3 = thunk_FUN_1400876c0(&local_148,local_58);
    uVar5 = thunk_FUN_14039be80(local_160);
    local_178 = 0x101;
    thunk_FUN_14039b070(uVar5,40000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoSaver.cpp"
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
  if (*(int *)(param_1 + 0x2c) != 4) {
    *(undefined4 *)(param_1 + 0x2c) = 4;
    thunk_FUN_1402816a0(param_1);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402a6850
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

longlong * FUN_1402a6850(longlong *param_1,QString *param_2)

{
  char cVar1;
  bool bVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  wchar_t *_Filename;
  FILE *pFVar5;
  QByteArray *this;
  char *pcVar6;
  ulonglong uVar7;
  undefined1 auStack_1b8 [32];
  undefined4 local_198;
  undefined4 local_188 [3];
  int iStack_17c;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  undefined8 local_88;
  longlong *local_80;
  undefined ***local_78 [3];
  ulonglong local_60;
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  local_88 = 0xfffffffffffffffe;
  local_38 = DAT_140e88880 ^ (ulonglong)auStack_1b8;
  local_188[0] = 0;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  local_80 = param_1;
  QString::QString((QString *)(param_1 + 4),param_2);
  if (*(int *)(param_1[4] + 4) == 0) {
    uVar3 = thunk_FUN_14039be80(local_78);
    cVar1 = thunk_FUN_14039ba40(uVar3,40000);
    thunk_FUN_14039a8a0(local_78);
    if (cVar1 == '\0') {
      return param_1;
    }
    local_178 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_f0);
    local_188[0] = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    local_78[0] = &local_170;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
    local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_108 = 0;
    local_100 = 4;
    thunk_FUN_14006fb70(&local_178,"no output filename");
    uVar3 = thunk_FUN_1400876c0(&local_178,local_58);
    uVar4 = thunk_FUN_14039be80(local_78);
    local_198 = 0x43;
    thunk_FUN_14039b070(uVar4,40000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                       );
    thunk_FUN_14039a8a0(local_78);
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
  }
  else {
    bVar2 = QFile::exists((QString *)(param_1 + 4));
    if (bVar2) {
      QFile::remove((QString *)(param_1 + 4));
    }
    _Filename = (wchar_t *)QString::toStdWString((QString *)(param_1 + 4));
    if (7 < *(ulonglong *)(_Filename + 0xc)) {
      _Filename = *(wchar_t **)_Filename;
    }
    pFVar5 = _wfopen(_Filename,L"wb");
    *param_1 = (longlong)pFVar5;
    if (7 < local_60) {
      thunk_FUN_1402a7f50(local_78,local_78[0]);
    }
    if (*param_1 != 0) {
      return param_1;
    }
    uVar3 = thunk_FUN_14039be80(local_78);
    cVar1 = thunk_FUN_14039ba40(uVar3,40000);
    thunk_FUN_14039a8a0(local_78);
    if (cVar1 == '\0') {
      return param_1;
    }
    local_178 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_f0);
    local_188[0] = 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
    *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
         *(int *)(local_178 + 4) + -0x88;
    thunk_FUN_140076630(&local_170,2);
    uVar3 = thunk_FUN_14006fb70(&local_178,"failed to open file \"");
    this = (QByteArray *)QString::toLocal8Bit(param_2);
    pcVar6 = QByteArray::data(this);
    thunk_FUN_14006fb70(uVar3,pcVar6);
    QByteArray::~QByteArray((QByteArray *)local_188);
    uVar3 = thunk_FUN_1400876c0(&local_178,local_58);
    uVar4 = thunk_FUN_14039be80(local_78);
    local_198 = 0x53;
    thunk_FUN_14039b070(uVar4,40000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\videoComposer.cpp"
                       );
    thunk_FUN_14039a8a0(local_78);
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
    thunk_FUN_140077af0(local_f0);
  }
  std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_f0);
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402a9a20
// -----------------------------------------------------------------------------
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
// ISTORIC AUDIO
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402b3d10
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402b3d10(longlong param_1,longlong param_2,longlong param_3,QDateTime *param_4,
                  QDateTime *param_5)

{
  bool bVar1;
  char cVar2;
  __int64 _Var3;
  __int64 _Var4;
  undefined8 uVar5;
  basic_ostream<char,std::char_traits<char>_> *pbVar6;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  undefined1 auStack_1d8 [32];
  undefined4 local_1b8;
  undefined4 local_1a8;
  longlong local_1a0;
  longlong local_198;
  __int64 local_190;
  __int64 local_188;
  longlong local_180;
  undefined8 local_178;
  int iStack_16c;
  undefined *local_168;
  basic_streambuf<char,std::char_traits<char>_> local_160 [128];
  basic_ios<char,std::char_traits<char>_> local_e0 [104];
  ulonglong local_78 [3];
  ulonglong local_60;
  ulonglong local_58;
  
  if (0 < param_2) {
    local_178 = 0xfffffffffffffffe;
    local_58 = DAT_140e88880 ^ (ulonglong)auStack_1d8;
    local_1a8 = 0;
    if ((0 < param_3) && (local_180 = param_1, bVar1 = QDateTime::operator<(param_4,param_5), bVar1)
       ) {
      _Var3 = QDateTime::toMSecsSinceEpoch(param_4);
      _Var4 = QDateTime::toMSecsSinceEpoch(param_5);
      uVar5 = thunk_FUN_1402b69e0();
      cVar2 = thunk_FUN_1402bac30(uVar5);
      if (cVar2 != '\0') {
        uVar5 = thunk_FUN_14039be80(&local_1a0);
        cVar2 = thunk_FUN_14039ba40(uVar5,20000);
        thunk_FUN_14039a8a0(&local_1a0);
        if (cVar2 != '\0') {
          local_168 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_e0);
          local_1a8 = 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_168,local_160,false);
          *(undefined ***)(local_160 + (longlong)*(int *)(local_168 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_16c + (longlong)*(int *)(local_168 + 4)) =
               *(int *)(local_168 + 4) + -0x88;
          thunk_FUN_140076630(local_160,2);
          pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70(&local_168,"AudioStore::addWantedTimeRange nodeId=");
          pbVar7 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,param_2);
          pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70(pbVar7," onlineSessionId=");
          pbVar7 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,param_3);
          uVar5 = thunk_FUN_14006fb70(pbVar7," beg=");
          uVar5 = thunk_FUN_1401197a0(uVar5,param_4);
          pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70(uVar5,&DAT_140beb29c);
          pbVar7 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,_Var3);
          uVar5 = thunk_FUN_14006fb70(pbVar7,") end=");
          uVar5 = thunk_FUN_1401197a0(uVar5,param_5);
          pbVar6 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70(uVar5,&DAT_140beb29c);
          pbVar7 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar6,_Var4);
          thunk_FUN_14006fb70(pbVar7,&DAT_1405bcb50);
          uVar5 = thunk_FUN_1400876c0(&local_168,local_78);
          uVar8 = thunk_FUN_14039be80(&local_1a0);
          local_1b8 = 0x4c;
          thunk_FUN_14039b070(uVar8,20000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioStore.cpp"
                             );
          thunk_FUN_14039a8a0(&local_1a0);
          if (0xf < local_60) {
            uVar9 = local_78[0];
            if (0xfff < local_60 + 1) {
              if ((local_78[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar9 = *(ulonglong *)(local_78[0] - 8);
              if (local_78[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_78[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_78[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar9);
          }
          thunk_FUN_140077af0(local_e0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_e0);
        }
      }
      local_1a0 = param_2;
      local_198 = param_3;
      local_190 = _Var3;
      local_188 = _Var4;
      thunk_FUN_1402b40c0(local_180 + 0x1b0,&local_1a0);
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402b60d0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402b60d0(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  undefined8 uVar3;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar4;
  ulonglong uVar5;
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
  if (*(longlong *)(param_1 + 0x1b8) != 0) {
    thunk_FUN_1402b40c0(param_1 + 0x1b0,*(undefined8 *)(param_1 + 0x1b8));
    lVar1 = *(longlong *)(param_1 + 0x1b8);
    if (lVar1 != 0) {
      *(undefined8 *)(param_1 + 0x1b8) = 0;
      FUN_140008206(lVar1,0x20);
    }
  }
  uVar3 = thunk_FUN_14039be80(local_150);
  cVar2 = thunk_FUN_14039ba40(uVar3,20000);
  thunk_FUN_14039a8a0(local_150);
  if (cVar2 != '\0') {
    local_138 = &DAT_1405a3878;
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
           thunk_FUN_14006fb70(&local_138,"history audio connection closed for node #");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,*(__int64 *)(param_1 + 0x1a0))
    ;
    uVar3 = thunk_FUN_1400876c0(&local_138,local_48);
    uVar4 = thunk_FUN_14039be80(local_150);
    local_168 = 0x154;
    thunk_FUN_14039b070(uVar4,20000,uVar3,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_150);
    if (0xf < local_30) {
      uVar5 = local_48[0];
      if (0xfff < local_30 + 1) {
        if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar5 = *(ulonglong *)(local_48[0] - 8);
        if (local_48[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_48[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_48[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar5);
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
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>(local_b0)
    ;
  }
  *(undefined8 *)(param_1 + 0x1a0) = 0;
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402b7bc0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402b7bc0(longlong param_1,longlong *param_2)

{
  QListData *pQVar1;
  QString *pQVar2;
  longlong lVar3;
  void *pvVar4;
  void *pvVar5;
  void *pvVar6;
  Data *pDVar7;
  char cVar8;
  bool bVar9;
  int iVar10;
  undefined8 uVar11;
  basic_ostream<char,std::char_traits<char>_> *pbVar12;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar13;
  undefined8 uVar14;
  longlong lVar15;
  longlong lVar16;
  void **ppvVar17;
  QByteArray *this;
  char *pcVar18;
  ulonglong uVar19;
  QString *pQVar20;
  uint uVar21;
  undefined1 auStack_448 [32];
  undefined4 local_428;
  uint local_418;
  code *local_410;
  Data *local_408;
  undefined ***local_400;
  undefined1 local_3f8 [8];
  undefined1 local_3f0 [8];
  QByteArray local_3e8 [8];
  undefined8 local_3e0;
  undefined1 local_3d8 [16];
  undefined1 local_3c8 [16];
  undefined1 local_3b8 [16];
  undefined1 local_3a8 [16];
  undefined1 local_398 [16];
  undefined1 local_388 [12];
  int iStack_37c;
  undefined *local_378;
  undefined **local_370;
  basic_ostream<char,std::char_traits<char>_> local_368 [96];
  undefined8 local_308;
  undefined4 local_300;
  basic_ios<char,std::char_traits<char>_> local_2f0 [100];
  int iStack_28c;
  undefined *local_288;
  undefined **local_280;
  basic_ostream<char,std::char_traits<char>_> local_278 [96];
  undefined8 local_218;
  undefined4 local_210;
  basic_ios<char,std::char_traits<char>_> local_200 [100];
  int iStack_19c;
  undefined *local_198;
  basic_streambuf<char,std::char_traits<char>_> local_190 [128];
  basic_ios<char,std::char_traits<char>_> local_110 [104];
  ulonglong local_a8 [3];
  ulonglong local_90;
  ulonglong local_88 [3];
  ulonglong local_70;
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_3e0 = 0xfffffffffffffffe;
  local_48 = DAT_140e88880 ^ (ulonglong)auStack_448;
  uVar21 = 0;
  local_418 = 0;
  lVar16 = *param_2;
  uVar11 = thunk_FUN_14039be80(local_3d8);
  cVar8 = thunk_FUN_14039ba40(uVar11,20000);
  thunk_FUN_14039a8a0(local_3d8);
  if (cVar8 != '\0') {
    local_378 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_2f0)
    ;
    uVar21 = 1;
    local_418 = 1;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_378,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_370,false);
    *(undefined ***)((longlong)&local_378 + (longlong)*(int *)(local_378 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_37c + (longlong)*(int *)(local_378 + 4)) =
         *(int *)(local_378 + 4) + -0x88;
    local_400 = &local_370;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_370);
    local_370 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_308 = 0;
    local_300 = 4;
    iVar10 = QListData::size((QListData *)(lVar16 + 0x28));
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14006fb70(&local_378,"AudioStore: StreamFrames id=");
    pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar12,*(__int64 *)(lVar16 + 8));
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14006fb70(pbVar13," timeUs=")
    ;
    pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar12,*(__int64 *)(lVar16 + 0x10));
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14006fb70(pbVar13," _endTimeUs=");
    pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar12,*(__int64 *)(lVar16 + 0x18));
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14006fb70(pbVar13," _nextTimeUs=");
    pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar12,*(__int64 *)(lVar16 + 0x20));
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)thunk_FUN_14006fb70(pbVar13," cnt=");
    std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar12,iVar10);
    uVar11 = thunk_FUN_1400876c0(&local_378,local_a8);
    uVar14 = thunk_FUN_14039be80(local_3c8);
    local_428 = 0xb3;
    thunk_FUN_14039b070(uVar14,20000,uVar11,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_3c8);
    if (0xf < local_90) {
      uVar19 = local_a8[0];
      if (0xfff < local_90 + 1) {
        if ((local_a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar19 = *(ulonglong *)(local_a8[0] - 8);
        if (local_a8[0] <= uVar19) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_a8[0] - uVar19 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_a8[0] - uVar19) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar19);
    }
    *(undefined ***)((longlong)&local_378 + (longlong)*(int *)(local_378 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_37c + (longlong)*(int *)(local_378 + 4)) =
         *(int *)(local_378 + 4) + -0x88;
    local_370 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_370);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_370);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_368);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_2f0);
  }
  if (*(longlong *)(param_1 + 0x1b8) != 0) {
    pQVar1 = (QListData *)(param_1 + 0x1c8);
    bVar9 = QListData::isEmpty(pQVar1);
    if (((((!bVar9) &&
          (lVar15 = thunk_FUN_1402b56a0(pQVar1),
          *(longlong *)(lVar15 + 0x10) == *(longlong *)(lVar16 + 8))) &&
         (*(longlong *)(lVar15 + 0x20) == *(longlong *)(lVar16 + 0x10))) &&
        (*(longlong *)(lVar15 + 0x28) == *(longlong *)(lVar16 + 0x18))) &&
       ((*(longlong *)(lVar16 + 0x20) == 0 ||
        (*(longlong *)(lVar16 + 0x10) < *(longlong *)(lVar16 + 0x20))))) {
      lVar15 = thunk_FUN_1402b56a0(pQVar1);
      bVar9 = QListData::isEmpty((QListData *)(lVar16 + 0x28));
      if (bVar9) {
        uVar11 = thunk_FUN_14039be80(local_3b8);
        cVar8 = thunk_FUN_14039ba40(uVar11,20000);
        thunk_FUN_14039a8a0(local_3b8);
        if (cVar8 != '\0') {
          local_198 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_110);
          local_418 = uVar21 | 4;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_198,local_190,false);
          *(undefined ***)(local_190 + (longlong)*(int *)(local_198 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_19c + (longlong)*(int *)(local_198 + 4)) =
               *(int *)(local_198 + 4) + -0x88;
          thunk_FUN_140076630(local_190,2);
          pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_14006fb70(&local_198,"AudioStore: audio stream descriptionId=");
          pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                              (pbVar12,*(__int64 *)(lVar16 + 8));
          thunk_FUN_14006fb70(pbVar13," no audio data");
          uVar11 = thunk_FUN_1400876c0(&local_198,local_88);
          uVar14 = thunk_FUN_14039be80(local_3a8);
          local_428 = 0xc2;
          thunk_FUN_14039b070(uVar14,20000,uVar11,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioStore.cpp"
                             );
          thunk_FUN_14039a8a0(local_3a8);
          if (0xf < local_70) {
            uVar19 = local_88[0];
            if (0xfff < local_70 + 1) {
              if ((local_88[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar19 = *(ulonglong *)(local_88[0] - 8);
              if (local_88[0] <= uVar19) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_88[0] - uVar19 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_88[0] - uVar19) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar19);
          }
          thunk_FUN_140077af0(local_110);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_110);
        }
      }
      else {
        thunk_FUN_1402c3da0(param_1 + 0x1d0,*(undefined4 *)(param_1 + 0x1c0),lVar15,lVar16 + 0x38);
        lVar3 = *(longlong *)(lVar16 + 0x20);
        if ((lVar3 != 0) && (lVar3 < *(longlong *)(lVar16 + 0x18))) {
          *(longlong *)(lVar15 + 0x20) = lVar3;
          thunk_FUN_1402b8ca0(param_1,lVar15);
          return;
        }
      }
      lVar16 = thunk_FUN_1402b2e50(param_1 + 0x1b8);
      thunk_FUN_1402c4c90(param_1 + 0x1d0,*(undefined4 *)(param_1 + 0x1c0),
                          *(undefined8 *)(lVar16 + 0x18));
      thunk_FUN_1402b89a0(pQVar1);
      bVar9 = QListData::isEmpty(pQVar1);
      if (!bVar9) {
        uVar11 = thunk_FUN_1402b56a0(pQVar1);
        thunk_FUN_1402b8ca0(param_1,uVar11);
        return;
      }
      local_408 = (Data *)shared_null_exref;
      local_410 = shared_null_exref;
      cVar8 = thunk_FUN_1402bec50(param_1 + 0x1d0,1,&local_408,&local_410);
      if (cVar8 != '\0') {
        thunk_FUN_1402c60c0(*(undefined8 *)(param_1 + 0x1f8),&local_408,&local_410);
        uVar11 = thunk_FUN_14024e160(local_3f8,&local_410);
        thunk_FUN_140287c90(param_1,uVar11);
      }
      cVar8 = thunk_FUN_1402bec50(param_1 + 0x1d0,0,&local_408,&local_410);
      if (cVar8 != '\0') {
        thunk_FUN_1402c60c0(*(undefined8 *)(param_1 + 0x1f0),&local_408,&local_410);
        uVar11 = thunk_FUN_14024e160(local_3f0,&local_410);
        thunk_FUN_140287c90(param_1,uVar11);
      }
      lVar16 = *(longlong *)(param_1 + 0x1b8);
      if (lVar16 != 0) {
        *(undefined8 *)(param_1 + 0x1b8) = 0;
        FUN_140008206(lVar16,0x20);
      }
      thunk_FUN_1402b8af0(param_1);
      thunk_FUN_14024f290(&local_410);
      pDVar7 = local_408;
      if (*(int *)local_408 != 0) {
        if (*(int *)local_408 == -1) {
          return;
        }
        LOCK();
        iVar10 = *(int *)local_408;
        *(int *)local_408 = *(int *)local_408 + -1;
        UNLOCK();
        if (iVar10 != 1) {
          return;
        }
      }
      pQVar20 = (QString *)(local_408 + ((longlong)*(int *)(local_408 + 0xc) + 2) * 8);
      pQVar2 = (QString *)(local_408 + ((longlong)*(int *)(local_408 + 8) + 2) * 8);
      while (pQVar2 != pQVar20) {
        pQVar20 = pQVar20 + -8;
        QString::~QString(pQVar20);
      }
      QListData::dispose(pDVar7);
      return;
    }
  }
  uVar11 = thunk_FUN_14039be80(local_398);
  cVar8 = thunk_FUN_14039ba40(uVar11,30000);
  thunk_FUN_14039a8a0(local_398);
  if (cVar8 != '\0') {
    local_288 = &DAT_1405a3878;
    std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>(local_200)
    ;
    local_418 = uVar21 | 2;
    std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
              ((basic_ostream<char,std::char_traits<char>_> *)&local_288,
               (basic_streambuf<char,std::char_traits<char>_> *)&local_280,false);
    *(undefined ***)((longlong)&local_288 + (longlong)*(int *)(local_288 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_28c + (longlong)*(int *)(local_288 + 4)) =
         *(int *)(local_288 + 4) + -0x88;
    local_400 = &local_280;
    std::basic_streambuf<char,std::char_traits<char>_>::
    basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_280);
    local_280 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    local_218 = 0;
    local_210 = 4;
    pQVar1 = (QListData *)(param_1 + 0x1c8);
    bVar9 = QListData::isEmpty(pQVar1);
    if (bVar9) {
      qt_assert("!isEmpty()","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                0x14f);
    }
    if (1 < **(uint **)pQVar1) {
      thunk_FUN_1402b4f40(pQVar1,(*(uint **)pQVar1)[1]);
    }
    ppvVar17 = QListData::begin(pQVar1);
    pQVar20 = (QString *)thunk_FUN_1402ba860(*ppvVar17,&local_400);
    bVar9 = QListData::isEmpty(pQVar1);
    if (bVar9) {
      qt_assert("!isEmpty()","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                0x14f);
    }
    if (1 < **(uint **)pQVar1) {
      thunk_FUN_1402b4f40(pQVar1,(*(uint **)pQVar1)[1]);
    }
    ppvVar17 = QListData::begin(pQVar1);
    pvVar4 = *ppvVar17;
    bVar9 = QListData::isEmpty(pQVar1);
    if (bVar9) {
      qt_assert("!isEmpty()","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                0x14f);
    }
    if (1 < **(uint **)pQVar1) {
      thunk_FUN_1402b4f40(pQVar1,(*(uint **)pQVar1)[1]);
    }
    ppvVar17 = QListData::begin(pQVar1);
    pvVar5 = *ppvVar17;
    bVar9 = QListData::isEmpty(pQVar1);
    if (bVar9) {
      qt_assert("!isEmpty()","c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                0x14f);
    }
    if (1 < **(uint **)pQVar1) {
      thunk_FUN_1402b4f40(pQVar1,(*(uint **)pQVar1)[1]);
    }
    ppvVar17 = QListData::begin(pQVar1);
    pvVar6 = *ppvVar17;
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14006fb70(&local_288,"AudioStore: received wrong stream for item: startUs=")
    ;
    pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar12,*(__int64 *)((longlong)pvVar6 + 0x20));
    pbVar12 = (basic_ostream<char,std::char_traits<char>_> *)
              thunk_FUN_14006fb70(pbVar13," _stopUs=");
    pbVar13 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                        (pbVar12,*(__int64 *)((longlong)pvVar5 + 0x28));
    uVar11 = thunk_FUN_14006fb70(pbVar13," _streamBegin=");
    uVar11 = thunk_FUN_1401197a0(uVar11,(longlong)pvVar4 + 0x18);
    uVar11 = thunk_FUN_14006fb70(uVar11," mark=");
    this = (QByteArray *)QString::toLocal8Bit(pQVar20);
    pcVar18 = QByteArray::data(this);
    thunk_FUN_14006fb70(uVar11,pcVar18);
    QByteArray::~QByteArray(local_3e8);
    QString::~QString((QString *)&local_400);
    uVar11 = thunk_FUN_1400876c0(&local_288,local_68);
    uVar14 = thunk_FUN_14039be80(local_388);
    local_428 = 0xba;
    thunk_FUN_14039b070(uVar14,30000,uVar11,
                        "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioStore.cpp"
                       );
    thunk_FUN_14039a8a0(local_388);
    if (0xf < local_50) {
      uVar19 = local_68[0];
      if (0xfff < local_50 + 1) {
        if ((local_68[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        uVar19 = *(ulonglong *)(local_68[0] - 8);
        if (local_68[0] <= uVar19) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (local_68[0] - uVar19 < 8) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
        if (0x27 < local_68[0] - uVar19) {
                    /* WARNING: Subroutine does not return */
          _invalid_parameter_noinfo_noreturn();
        }
      }
      FUN_140002d6f(uVar19);
    }
    *(undefined ***)((longlong)&local_288 + (longlong)*(int *)(local_288 + 4)) =
         std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    *(int *)((longlong)&iStack_28c + (longlong)*(int *)(local_288 + 4)) =
         *(int *)(local_288 + 4) + -0x88;
    local_280 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
    thunk_FUN_14007bb20(&local_280);
    std::basic_streambuf<char,std::char_traits<char>_>::
    ~basic_streambuf<char,std::char_traits<char>_>
              ((basic_streambuf<char,std::char_traits<char>_> *)&local_280);
    std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
              (local_278);
    std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
              (local_200);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402b97e0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402b97e0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  char cVar2;
  bool bVar3;
  int iVar4;
  QDateTime *pQVar5;
  QDateTime *this;
  __int64 _Var6;
  void **ppvVar7;
  undefined8 uVar8;
  QByteArray *pQVar9;
  char *pcVar10;
  basic_ostream<char,std::char_traits<char>_> *this_00;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar11;
  undefined8 uVar12;
  void **ppvVar13;
  void **ppvVar14;
  ulonglong uVar15;
  longlong lVar16;
  int iVar18;
  undefined1 auStack_268 [32];
  void **local_248;
  uint local_240;
  int *local_238;
  void **local_230;
  longlong *local_228;
  uint local_218;
  longlong local_210;
  undefined ***local_208;
  int local_200;
  QDateTime local_1f8 [8];
  QDateTime local_1f0 [8];
  void **local_1e8;
  QTypedArrayData<unsigned_short> *local_1e0;
  longlong local_1d8;
  QString *local_1d0;
  QByteArray local_1c8 [8];
  QByteArray local_1c0 [8];
  QString local_1b8 [8];
  undefined8 local_1b0;
  undefined1 local_1a8 [16];
  undefined1 local_198 [12];
  int iStack_18c;
  undefined *local_188;
  undefined **local_180;
  basic_ostream<char,std::char_traits<char>_> local_178 [96];
  undefined8 local_118;
  undefined4 local_110;
  basic_ios<char,std::char_traits<char>_> local_100 [104];
  int local_98;
  undefined4 local_94;
  undefined4 local_90;
  ulonglong local_88;
  undefined8 uStack_80;
  void *local_78;
  ulonglong local_70;
  undefined8 uStack_68;
  void *local_60;
  ulonglong local_58;
  longlong local_50;
  longlong local_48;
  ulonglong local_40;
  longlong lVar17;
  
  local_1b0 = 0xfffffffffffffffe;
  local_40 = DAT_140e88880 ^ (ulonglong)auStack_268;
  local_218 = 0;
  local_1d8 = param_1;
  local_200 = QListData::size((QListData *)(param_2 + 0x20));
  iVar18 = 0;
  if (0 < local_200) {
    do {
      if ((iVar18 < 0) || (iVar4 = QListData::size((QListData *)(param_2 + 0x30)), iVar4 <= iVar18))
      {
        qt_assert_x("QList<T>::operator[]","index out of range",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
      }
      pQVar5 = (QDateTime *)QListData::at((QListData *)(param_2 + 0x30),iVar18);
      if ((iVar18 < 0) || (iVar4 = QListData::size((QListData *)(param_2 + 0x28)), iVar4 <= iVar18))
      {
        qt_assert_x("QList<T>::operator[]","index out of range",
                    "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
      }
      this = (QDateTime *)QListData::at((QListData *)(param_2 + 0x28),iVar18);
      _Var6 = QDateTime::msecsTo(this,pQVar5);
      if (999 < _Var6) {
        local_88 = *(ulonglong *)(param_1 + 0x1a0);
        uStack_80 = *(undefined8 *)(param_2 + 8);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x20)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar7 = QListData::at((QListData *)(param_2 + 0x20),iVar18);
        local_78 = *ppvVar7;
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x48)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar7 = QListData::at((QListData *)(param_2 + 0x48),iVar18);
        local_98 = (int)*(char *)ppvVar7;
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x50)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar7 = QListData::at((QListData *)(param_2 + 0x50),iVar18);
        local_94 = *(undefined4 *)ppvVar7;
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x40)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar7 = QListData::at((QListData *)(param_2 + 0x40),iVar18);
        local_90 = *(undefined4 *)ppvVar7;
        thunk_FUN_1402c2890(param_1 + 0x1d0,&local_210,&local_88,&local_98);
        if (*(int *)(local_210 + 4) == 0) {
          QString::~QString((QString *)&local_210);
          return;
        }
        uVar8 = thunk_FUN_1402b69e0();
        lVar17 = -1;
        do {
          lVar16 = lVar17 + 1;
          lVar1 = lVar17 + 1;
          lVar17 = lVar16;
        } while ("viewerAudio"[lVar1] != '\0');
        local_1e0 = QString::fromAscii_helper("viewerAudio",(int)lVar16);
        cVar2 = thunk_FUN_1403db420(uVar8,&local_1e0);
        QString::~QString((QString *)&local_1e0);
        if (cVar2 != '\0') {
          uVar8 = thunk_FUN_14039be80(local_1a8);
          cVar2 = thunk_FUN_14039ba40(uVar8,20000);
          thunk_FUN_14039a8a0(local_1a8);
          if (cVar2 != '\0') {
            local_188 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_100);
            local_218 = local_218 | 1;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_188,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_180,false);
            *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
                 *(int *)(local_188 + 4) + -0x88;
            local_208 = &local_180;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
            local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_118 = 0;
            local_110 = 4;
            local_1d0 = (QString *)thunk_FUN_1402ba860(&local_88,local_1b8);
            if ((iVar18 < 0) ||
               (iVar4 = QListData::size((QListData *)(param_2 + 0x20)), iVar4 <= iVar18)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x217);
            }
            local_208 = QListData::at((QListData *)(param_2 + 0x20),iVar18);
            if ((iVar18 < 0) ||
               (iVar4 = QListData::size((QListData *)(param_2 + 0x30)), iVar4 <= iVar18)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x217);
            }
            local_1e8 = QListData::at((QListData *)(param_2 + 0x30),iVar18);
            if ((iVar18 < 0) ||
               (iVar4 = QListData::size((QListData *)(param_2 + 0x28)), iVar4 <= iVar18)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x217);
            }
            ppvVar7 = QListData::at((QListData *)(param_2 + 0x28),iVar18);
            uVar8 = thunk_FUN_14006fb70(&local_188,"Store AudioStreamInfo: marker=");
            pQVar9 = (QByteArray *)QString::toLocal8Bit(local_1d0);
            pcVar10 = QByteArray::data(pQVar9);
            thunk_FUN_14006fb70(uVar8,pcVar10);
            QByteArray::~QByteArray(local_1c8);
            uVar8 = thunk_FUN_14006fb70(uVar8,&DAT_140be6064);
            uVar8 = thunk_FUN_1401197a0(uVar8,ppvVar7);
            uVar8 = thunk_FUN_14006fb70(uVar8,&DAT_140bfeca0);
            uVar8 = thunk_FUN_1401197a0(uVar8,local_1e8);
            this_00 = (basic_ostream<char,std::char_traits<char>_> *)
                      thunk_FUN_14006fb70(uVar8,"] streamDescription=");
            pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<
                                (this_00,(__int64)*local_208);
            uVar8 = thunk_FUN_14006fb70(pbVar11," file_name=");
            pQVar9 = (QByteArray *)QString::toLocal8Bit((QString *)&local_210);
            pcVar10 = QByteArray::data(pQVar9);
            thunk_FUN_14006fb70(uVar8,pcVar10);
            QByteArray::~QByteArray(local_1c0);
            QString::~QString(local_1b8);
            uVar8 = thunk_FUN_1400876c0(&local_188,&local_70);
            uVar12 = thunk_FUN_14039be80(local_198);
            local_248 = (void **)CONCAT44(local_248._4_4_,0x88);
            thunk_FUN_14039b070(uVar12,20000,uVar8,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioStore.cpp"
                               );
            thunk_FUN_14039a8a0(local_198);
            if (0xf < local_58) {
              uVar15 = local_70;
              if (0xfff < local_58 + 1) {
                if ((local_70 & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar15 = *(ulonglong *)(local_70 - 8);
                if (local_70 <= uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_70 - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_70 - uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(uVar15);
            }
            *(undefined ***)((longlong)&local_188 + (longlong)*(int *)(local_188 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_18c + (longlong)*(int *)(local_188 + 4)) =
                 *(int *)(local_188 + 4) + -0x88;
            local_180 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14007bb20(&local_180);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_180);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_178);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_100);
          }
        }
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x58)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        local_1e8 = QListData::at((QListData *)(param_2 + 0x58),iVar18);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x38)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        local_208 = QListData::at((QListData *)(param_2 + 0x38),iVar18);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x30)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar7 = QListData::at((QListData *)(param_2 + 0x30),iVar18);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x28)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar13 = QListData::at((QListData *)(param_2 + 0x28),iVar18);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x20)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        ppvVar14 = QListData::at((QListData *)(param_2 + 0x20),iVar18);
        local_228 = &local_210;
        local_230 = local_1e8;
        local_238 = &local_98;
        local_240 = (uint)*(byte *)local_208;
        local_248 = ppvVar7;
        thunk_FUN_1402c06f0(local_1d8 + 0x1d0,*(undefined4 *)(local_1d8 + 0x1c0),*ppvVar14,ppvVar13)
        ;
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x28)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        pQVar5 = (QDateTime *)QListData::at((QListData *)(param_2 + 0x28),iVar18);
        bVar3 = QDateTime::operator<(pQVar5,(QDateTime *)(param_2 + 0x10));
        if (bVar3) {
          pQVar5 = (QDateTime *)(param_2 + 0x10);
        }
        QDateTime::QDateTime(local_1f0,pQVar5);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x30)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        pQVar5 = (QDateTime *)QListData::at((QListData *)(param_2 + 0x30),iVar18);
        bVar3 = QDateTime::operator<((QDateTime *)(param_2 + 0x18),pQVar5);
        if (bVar3) {
          pQVar5 = (QDateTime *)(param_2 + 0x18);
        }
        QDateTime::QDateTime(local_1f8,pQVar5);
        QDateTime::setTimeSpec(local_1f0,1);
        QDateTime::setTimeSpec(local_1f8,1);
        if ((iVar18 < 0) ||
           (iVar4 = QListData::size((QListData *)(param_2 + 0x28)), iVar4 <= iVar18)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x217);
        }
        pQVar5 = (QDateTime *)QListData::at((QListData *)(param_2 + 0x28),iVar18);
        local_70 = local_88;
        uStack_68 = uStack_80;
        local_60 = local_78;
        QDateTime::QDateTime((QDateTime *)&local_58,pQVar5);
        _Var6 = QDateTime::msecsTo(pQVar5,local_1f0);
        local_50 = _Var6 * 1000;
        _Var6 = QDateTime::msecsTo(pQVar5,local_1f8);
        param_1 = local_1d8;
        local_48 = _Var6 * 1000;
        thunk_FUN_1402b4170(local_1d8 + 0x1c8,&local_70);
        QDateTime::~QDateTime((QDateTime *)&local_58);
        QDateTime::~QDateTime(local_1f8);
        QDateTime::~QDateTime(local_1f0);
        QString::~QString((QString *)&local_210);
      }
      iVar18 = iVar18 + 1;
    } while (iVar18 < local_200);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402c6590
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402c6590(longlong param_1,longlong param_2)

{
  char cVar1;
  undefined8 uVar2;
  QByteArray *pQVar3;
  char *pcVar4;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar5;
  __int64 _Var6;
  ulonglong uVar7;
  ulonglong uVar8;
  uint uVar9;
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
  uVar2 = thunk_FUN_1402b69e0();
  local_178 = QString::fromAscii_helper("viewerAudio",0xb);
  cVar1 = thunk_FUN_1403db420(uVar2,&local_178);
  QString::~QString((QString *)&local_178);
  uVar9 = 0;
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14039be80(local_170);
    cVar1 = thunk_FUN_14039ba40(uVar2,20000);
    thunk_FUN_14039a8a0(local_170);
    uVar9 = 0;
    if (cVar1 != '\0') {
      local_158 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      uVar9 = 1;
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
      pQVar3 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x50));
      pcVar4 = QByteArray::data(pQVar3);
      thunk_FUN_14006fb70(&local_158,pcVar4);
      QByteArray::~QByteArray((QByteArray *)&local_178);
      this = (basic_ostream<char,std::char_traits<char>_> *)
             thunk_FUN_14006fb70(&local_158,"playAudio pos=");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,param_2);
      uVar2 = thunk_FUN_1400876c0(&local_158,local_60);
      uVar5 = thunk_FUN_14039be80(local_170);
      local_188 = 0x78;
      thunk_FUN_14039b070(uVar5,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioPlayer.cpp"
                         );
      thunk_FUN_14039a8a0(local_170);
      if (0xf < local_48) {
        uVar7 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar7);
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
  _Var6 = QDateTime::currentMSecsSinceEpoch();
  uVar7 = (_Var6 - *(longlong *)(param_1 + 0x40)) - (param_2 - *(longlong *)(param_1 + 0x38));
  uVar8 = (longlong)uVar7 >> 0x3f;
  if ((longlong)((uVar7 ^ uVar8) - uVar8) < 0xc9) {
LAB_1402c6a1c:
    if (*(char *)(param_1 + 0x48) != '\0') goto LAB_1402c6c0a;
  }
  else {
    uVar2 = thunk_FUN_1402b69e0();
    local_178 = QString::fromAscii_helper("viewerAudio",0xb);
    cVar1 = thunk_FUN_1403db420(uVar2,&local_178);
    QString::~QString((QString *)&local_178);
    if (cVar1 != '\0') {
      uVar2 = thunk_FUN_14039be80(local_170);
      cVar1 = thunk_FUN_14039ba40(uVar2,20000);
      thunk_FUN_14039a8a0(local_170);
      if (cVar1 != '\0') {
        local_158 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        local_178 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_178._4_4_,uVar9) | 2);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        thunk_FUN_140076630(&local_150,2);
        pQVar3 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x50));
        pcVar4 = QByteArray::data(pQVar3);
        thunk_FUN_14006fb70(&local_158,pcVar4);
        QByteArray::~QByteArray((QByteArray *)&local_178);
        thunk_FUN_14006fb70(&local_158,"timeJump");
        uVar2 = thunk_FUN_1400876c0(&local_158,local_60);
        uVar5 = thunk_FUN_14039be80(local_170);
        local_188 = 0x86;
        thunk_FUN_14039b070(uVar5,20000,uVar2,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioPlayer.cpp"
                           );
        thunk_FUN_14039a8a0(local_170);
        if (0xf < local_48) {
          uVar7 = local_60[0];
          if (0xfff < local_48 + 1) {
            if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar7 = *(ulonglong *)(local_60[0] - 8);
            if (local_60[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_60[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_60[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar7);
        }
        thunk_FUN_140077af0(local_d0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        uVar9 = uVar9 | 2;
      }
    }
    if (*(char *)(param_1 + 0x48) != '\0') {
      thunk_FUN_1402c7a00(param_1);
      goto LAB_1402c6a1c;
    }
  }
  uVar2 = thunk_FUN_1402b69e0();
  local_178 = QString::fromAscii_helper("viewerAudio",0xb);
  cVar1 = thunk_FUN_1403db420(uVar2,&local_178);
  QString::~QString((QString *)&local_178);
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14039be80(local_170);
    cVar1 = thunk_FUN_14039ba40(uVar2,20000);
    thunk_FUN_14039a8a0(local_170);
    if (cVar1 != '\0') {
      local_158 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_d0);
      local_178 = (QTypedArrayData<unsigned_short> *)(CONCAT44(local_178._4_4_,uVar9) | 4);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
      *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
           *(int *)(local_158 + 4) + -0x88;
      thunk_FUN_140076630(&local_150,2);
      pQVar3 = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x50));
      pcVar4 = QByteArray::data(pQVar3);
      thunk_FUN_14006fb70(&local_158,pcVar4);
      QByteArray::~QByteArray((QByteArray *)&local_178);
      thunk_FUN_14006fb70(&local_158,"!_isPlaying");
      uVar2 = thunk_FUN_1400876c0(&local_158,local_60);
      uVar5 = thunk_FUN_14039be80(local_170);
      local_188 = 0x8d;
      thunk_FUN_14039b070(uVar5,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioPlayer.cpp"
                         );
      thunk_FUN_14039a8a0(local_170);
      if (0xf < local_48) {
        uVar7 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar7 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar7 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar7) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar7);
      }
      thunk_FUN_140077af0(local_d0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_d0);
    }
  }
  cVar1 = thunk_FUN_1402c6fb0(param_1,param_2);
  if (cVar1 != '\0') {
    thunk_FUN_1402c7630(param_1);
  }
LAB_1402c6c0a:
  QTimer::start(*(QTimer **)(param_1 + 0x30));
  *(__int64 *)(param_1 + 0x40) = _Var6;
  *(longlong *)(param_1 + 0x38) = param_2;
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402c7a00
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402c7a00(QObject *param_1)

{
  char cVar1;
  undefined8 uVar2;
  QByteArray *this;
  char *pcVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  undefined1 auStackY_188 [32];
  QTypedArrayData<unsigned_short> *local_158;
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
  local_28 = DAT_140e88880 ^ (ulonglong)auStackY_188;
  local_158 = (QTypedArrayData<unsigned_short> *)((ulonglong)local_158 & 0xffffffff00000000);
  uVar2 = thunk_FUN_1402b69e0();
  local_158 = QString::fromAscii_helper("viewerAudio",0xb);
  cVar1 = thunk_FUN_1403db420(uVar2,&local_158);
  QString::~QString((QString *)&local_158);
  if (cVar1 != '\0') {
    uVar2 = thunk_FUN_14039be80(local_150);
    cVar1 = thunk_FUN_14039ba40(uVar2,20000);
    thunk_FUN_14039a8a0(local_150);
    if (cVar1 != '\0') {
      local_138 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_b0);
      local_158 = (QTypedArrayData<unsigned_short> *)CONCAT44(local_158._4_4_,1);
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
      this = (QByteArray *)QString::toLocal8Bit((QString *)(param_1 + 0x50));
      pcVar3 = QByteArray::data(this);
      thunk_FUN_14006fb70(&local_138,pcVar3);
      QByteArray::~QByteArray((QByteArray *)&local_158);
      thunk_FUN_14006fb70(&local_138,"stopPlaying");
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar4 = thunk_FUN_14039be80(local_150);
      thunk_FUN_14039b070(uVar4,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-history\\player\\audioPlayer.cpp"
                         );
      thunk_FUN_14039a8a0(local_150);
      if (0xf < local_30) {
        uVar5 = local_48[0];
        if (0xfff < local_30 + 1) {
          if ((local_48[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar5 = *(ulonglong *)(local_48[0] - 8);
          if (local_48[0] <= uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_48[0] - uVar5 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_48[0] - uVar5) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar5);
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
  }
  param_1[0x48] = (QObject)0x0;
  QMediaPlayer::stop(*(QMediaPlayer **)(param_1 + 0x20));
  local_158 = (QTypedArrayData<unsigned_short> *)thunk_FUN_1402c61f0;
  local_150[0] = (undefined ***)stateChanged_exref;
  QObject::disconnectImpl
            (*(QObject **)(param_1 + 0x20),local_150,param_1,&local_158,
             (QMetaObject *)staticMetaObject_exref);
  return;
}

// =============================================================================
// KEYLOGGER
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402cd760
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Removing unreachable block (ram,0x0001402cd9d9) */
/* WARNING: Removing unreachable block (ram,0x0001402cd9ec) */
/* WARNING: Removing unreachable block (ram,0x0001402cd9f2) */
/* WARNING: Removing unreachable block (ram,0x0001402cd9fe) */
/* WARNING: Removing unreachable block (ram,0x0001402cda08) */
/* WARNING: Removing unreachable block (ram,0x0001402cda22) */
/* WARNING: Removing unreachable block (ram,0x0001402cda29) */
/* WARNING: Removing unreachable block (ram,0x0001402cda43) */

void FUN_1402cd760(QObject *param_1,QListData *param_2,QListData *param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  char cVar5;
  void **ppvVar6;
  void **ppvVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  basic_ostream<char,std::char_traits<char>_> *this;
  undefined8 uVar12;
  void *pvVar13;
  QObject *pQVar14;
  longlong lVar15;
  ulonglong uVar16;
  QObject *pQVar17;
  undefined1 auStackY_268 [32];
  code *local_1f8;
  QObject *local_1f0;
  QListData *local_1e8;
  QListData *local_1e0;
  QMutexLocker local_1d8 [8];
  code *local_1d0;
  QMutexLocker local_1c8 [8];
  Connection local_1c0 [8];
  undefined8 local_1b8;
  QListData *local_1b0;
  QListData *local_1a8;
  undefined1 local_1a0 [16];
  undefined1 local_190 [24];
  undefined4 local_178;
  undefined4 uStack_174;
  undefined4 uStack_170;
  undefined4 local_168;
  undefined4 uStack_164;
  undefined4 uStack_160;
  int iStack_15c;
  undefined *local_158;
  undefined **local_150;
  basic_ostream<char,std::char_traits<char>_> local_148 [96];
  undefined8 local_e8;
  undefined4 local_e0;
  basic_ios<char,std::char_traits<char>_> local_d0 [104];
  ulonglong local_68 [3];
  ulonglong local_50;
  ulonglong local_48;
  
  local_1b8 = 0xfffffffffffffffe;
  local_48 = DAT_140e88880 ^ (ulonglong)auStackY_268;
  pQVar14 = param_1 + 0x70;
  iVar3 = *(int *)(*(longlong *)pQVar14 + 4);
  local_1f0 = param_1;
  local_1e8 = param_3;
  local_1e0 = param_2;
  local_1b0 = param_2;
  local_1a8 = param_3;
  if (1 < **(uint **)param_2) {
    thunk_FUN_140080470(param_2,(*(uint **)param_2)[1]);
  }
  ppvVar6 = QListData::begin(param_2);
  if (1 < **(uint **)param_2) {
    thunk_FUN_140080470(param_2,(*(uint **)param_2)[1]);
  }
  ppvVar7 = QListData::end(param_2);
  for (; ppvVar6 != ppvVar7; ppvVar6 = ppvVar6 + 1) {
    thunk_FUN_1402ce620(pQVar14,ppvVar6);
  }
  if (1 < **(uint **)param_3) {
    thunk_FUN_140080470(param_3,(*(uint **)param_3)[1]);
  }
  ppvVar6 = QListData::begin(param_3);
  if (1 < **(uint **)param_3) {
    thunk_FUN_140080470(param_3,(*(uint **)param_3)[1]);
  }
  ppvVar7 = QListData::end(param_3);
  pQVar17 = DAT_140ea00b0;
  for (; DAT_140ea00b0 = pQVar17, ppvVar6 != ppvVar7; ppvVar6 = ppvVar6 + 1) {
    if (*(longlong *)(*(longlong *)pQVar14 + 0x10) == 0) {
LAB_1402cd8c9:
      lVar15 = 0;
    }
    else {
      lVar8 = *(longlong *)(*(longlong *)pQVar14 + 0x10);
      lVar15 = 0;
      do {
        if (*(longlong *)(lVar8 + 0x18) < (longlong)*ppvVar6) {
          lVar9 = *(longlong *)(lVar8 + 0x10);
        }
        else {
          lVar9 = *(longlong *)(lVar8 + 8);
          lVar15 = lVar8;
        }
        lVar8 = lVar9;
      } while (lVar9 != 0);
      if ((lVar15 == 0) || ((longlong)*ppvVar6 < *(longlong *)(lVar15 + 0x18))) goto LAB_1402cd8c9;
    }
    if (lVar15 == 0) {
      local_1f8 = shared_null_exref;
      puVar10 = (undefined8 *)thunk_FUN_1402cb720(pQVar14,ppvVar6);
      piVar4 = (int *)puVar10[1];
      puVar10[1] = 0;
      *puVar10 = 0;
      if (piVar4 != (int *)0x0) {
        LOCK();
        piVar1 = piVar4 + 1;
        iVar2 = *piVar1;
        *piVar1 = *piVar1 + -1;
        UNLOCK();
        if (iVar2 == 1) {
          (**(code **)(piVar4 + 2))(piVar4);
        }
        LOCK();
        iVar2 = *piVar4;
        *piVar4 = *piVar4 + -1;
        UNLOCK();
        if (iVar2 == 1) {
          if (*piVar4 != 0) {
            qt_assert("!weakref.load()",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qsharedpointer_impl.h"
                      ,0x98);
          }
          if (0 < piVar4[1]) {
            qt_assert("strongref.load() <= 0",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qsharedpointer_impl.h"
                      ,0x98);
          }
          FUN_140002d6f(piVar4);
        }
      }
      *(undefined1 *)(puVar10 + 2) = 0;
      thunk_FUN_1402cb300(puVar10 + 3,&local_1f8);
      if (*(int *)local_1f8 == 0) {
LAB_1402cd9c4:
        thunk_FUN_1402cc590(&local_1f8,local_1f8);
      }
      else if (*(int *)local_1f8 != -1) {
        LOCK();
        iVar2 = *(int *)local_1f8;
        *(int *)local_1f8 = *(int *)local_1f8 + -1;
        UNLOCK();
        if (iVar2 == 1) goto LAB_1402cd9c4;
      }
      param_1 = local_1f0;
      thunk_FUN_1402ce1a0(local_1f0,*ppvVar6);
    }
    else {
      uVar11 = thunk_FUN_14039be80(local_1a0);
      cVar5 = thunk_FUN_14039ba40(uVar11,30000);
      thunk_FUN_14039a8a0(local_1a0);
      if (cVar5 != '\0') {
        local_158 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_d0);
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_158,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_150,false);
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
        local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_e8 = 0;
        local_e0 = 4;
        this = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14006fb70(&local_158,"noticed about already know loader, node #");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(this,(__int64)*ppvVar6);
        uVar11 = thunk_FUN_1400876c0(&local_158,local_68);
        uVar12 = thunk_FUN_14039be80(local_190);
        thunk_FUN_14039b070(uVar12,30000,uVar11,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-keylogger\\selector_82.cpp"
                           );
        thunk_FUN_14039a8a0(local_190);
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
          FUN_140002d6f(uVar16);
        }
        *(undefined ***)((longlong)&local_158 + (longlong)*(int *)(local_158 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_15c + (longlong)*(int *)(local_158 + 4)) =
             *(int *)(local_158 + 4) + -0x88;
        local_150 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14007bb20(&local_150);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_150);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_148);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_d0);
      }
    }
    param_3 = local_1e8;
    param_2 = local_1e0;
    pQVar17 = DAT_140ea00b0;
  }
  if ((*(int *)(*(longlong *)pQVar14 + 4) == 0) != (iVar3 == 0)) {
    if (*(int *)(*(longlong *)pQVar14 + 4) == 0) {
      local_178 = 0x400104a1;
      uStack_174 = 1;
      uStack_170 = 0;
      if (pQVar17 == (QObject *)0x0) {
        QMutexLocker::QMutexLocker(local_1d8,(QBasicMutex *)&DAT_140ea00c0);
        pQVar14 = DAT_140ea00b0;
        if ((DAT_140ea00b0 == (QObject *)0x0) &&
           (pvVar13 = operator_new(0x20), pQVar14 = (QObject *)0x0, pvVar13 != (void *)0x0)) {
          pQVar14 = (QObject *)thunk_FUN_1402de030(pvVar13,0);
        }
        DAT_140ea00b0 = pQVar14;
        pQVar17 = DAT_140ea00b0;
        QMutexLocker::~QMutexLocker(local_1d8);
      }
      local_1d0 = thunk_FUN_1402cd670;
      QObject::disconnectImpl
                (pQVar17,(void **)&local_178,param_1,&local_1d0,(QMetaObject *)&DAT_140e87b40);
    }
    else {
      local_168 = 0x400104a1;
      uStack_164 = 1;
      uStack_160 = 0;
      if (pQVar17 == (QObject *)0x0) {
        QMutexLocker::QMutexLocker(local_1c8,(QBasicMutex *)&DAT_140ea00c0);
        pQVar14 = DAT_140ea00b0;
        if ((DAT_140ea00b0 == (QObject *)0x0) &&
           (pvVar13 = operator_new(0x20), pQVar14 = (QObject *)0x0, pvVar13 != (void *)0x0)) {
          pQVar14 = (QObject *)thunk_FUN_1402de030(pvVar13,0);
        }
        DAT_140ea00b0 = pQVar14;
        pQVar17 = DAT_140ea00b0;
        QMutexLocker::~QMutexLocker(local_1c8);
      }
      thunk_FUN_1402c9910(local_1c0,pQVar17,&local_168,param_1);
      QMetaObject::Connection::~Connection(local_1c0);
    }
  }
  thunk_FUN_1402cdfd0(param_1);
  piVar4 = *(int **)param_2;
  if (*piVar4 != 0) {
    if (*piVar4 == -1) goto LAB_1402cddd3;
    LOCK();
    iVar3 = *piVar4;
    *piVar4 = *piVar4 + -1;
    UNLOCK();
    if (iVar3 != 1) goto LAB_1402cddd3;
  }
  QListData::dispose(*(Data **)param_2);
LAB_1402cddd3:
  piVar4 = *(int **)param_3;
  if (*piVar4 != 0) {
    if (*piVar4 == -1) {
      return;
    }
    LOCK();
    iVar3 = *piVar4;
    *piVar4 = *piVar4 + -1;
    UNLOCK();
    if (iVar3 != 1) {
      return;
    }
  }
  QListData::dispose(*(Data **)param_3);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402df960
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402e7aa0
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402e7db0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1402e7db0(longlong param_1)

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
  if (*(char *)(param_1 + 0x1a8) == '\0') {
    thunk_FUN_140190bc0();
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
             thunk_FUN_14006fb70(&local_138,"keylogger connection opened for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1a0));
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar3 = thunk_FUN_14039be80(local_150);
      local_168 = 0x3b;
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
    *(undefined1 *)(param_1 + 0x1a8) = 1;
    thunk_FUN_1402e9420(param_1,*(undefined8 *)(param_1 + 0x1a0),1);
  }
  return;
}

// =============================================================================
// ONLINE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402edac0
// -----------------------------------------------------------------------------
void FUN_1402edac0(QObject *param_1)

{
  int iVar1;
  int *piVar2;
  undefined8 uVar3;
  
  *(undefined ***)param_1 = viewer::online::Grabber::vftable;
  QObject::disconnect(param_1,(char *)0x0,(QObject *)0x0,(char *)0x0);
  uVar3 = thunk_FUN_1400ec3c0();
  thunk_FUN_140309c80(uVar3,param_1);
  if (*(longlong *)(param_1 + 0x28) != 0) {
    qt_assert("!_dataProvider",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\grabber.cpp",0x28);
  }
  QString::~QString((QString *)(param_1 + 0x30));
  piVar2 = *(int **)(param_1 + 0x10);
  if (*piVar2 != 0) {
    if (*piVar2 == -1) goto LAB_1402edb50;
    LOCK();
    iVar1 = *piVar2;
    *piVar2 = *piVar2 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_1402edb50;
  }
  QListData::dispose(*(Data **)(param_1 + 0x10));
LAB_1402edb50:
                    /* WARNING: Could not recover jumptable at 0x0001402edb58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  QObject::~QObject(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402ee330
// -----------------------------------------------------------------------------
void FUN_1402ee330(longlong param_1,longlong param_2)

{
  longlong lVar1;
  
  lVar1 = *(longlong *)(param_1 + 0x28);
  if (param_2 == 0) {
    if (lVar1 != 0) goto LAB_1402ee34e;
  }
  else if (lVar1 == 0) goto LAB_1402ee34e;
  if (param_2 != lVar1) {
    qt_assert("(!dataProvider && _dataProvider) || (dataProvider && !_dataProvider) || (dataProvider == _dataProvider)"
              ,"R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\grabber.cpp",0x2d);
    *(longlong *)(param_1 + 0x28) = param_2;
    return;
  }
LAB_1402ee34e:
  *(longlong *)(param_1 + 0x28) = param_2;
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402f04a0
// -----------------------------------------------------------------------------
void FUN_1402f04a0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  
  lVar1 = *(longlong *)(param_1 + 0x28);
  if (param_2 == 0) {
    if (lVar1 != 0) goto LAB_1402f04be;
  }
  else if (lVar1 == 0) goto LAB_1402f04be;
  if (param_2 != lVar1) {
    qt_assert("(!dataProvider && _dataProvider) || (dataProvider && !_dataProvider) || (dataProvider == _dataProvider)"
              ,"R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\session.cpp",0x2a);
    *(longlong *)(param_1 + 0x28) = param_2;
    return;
  }
LAB_1402f04be:
  *(longlong *)(param_1 + 0x28) = param_2;
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402f1fc0
// -----------------------------------------------------------------------------
QImage * FUN_1402f1fc0(longlong param_1,QImage *param_2)

{
  bool bVar1;
  QImage *pQVar2;
  
  bVar1 = QImage::isNull((QImage *)&DAT_140ea02b8);
  if (bVar1) {
    qt_assert("!_imageNull.isNull()",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\monitor.cpp",0x134);
  }
  bVar1 = QImage::isNull((QImage *)(param_1 + 0x58));
  pQVar2 = (QImage *)&DAT_140ea02b8;
  if (!bVar1) {
    pQVar2 = (QImage *)(param_1 + 0x58);
  }
  QImage::QImage(param_2,pQVar2);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1402f2590
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QString * FUN_1402f2590(longlong param_1,QString *param_2,undefined8 param_3,QString *param_4)

{
  char cVar1;
  bool bVar2;
  FileError FVar3;
  QFileInfo *this;
  QString *pQVar4;
  LPCWSTR lpFileName;
  HANDLE hObject;
  undefined8 uVar5;
  QByteArray *pQVar6;
  char *pcVar7;
  undefined8 uVar8;
  QString *pQVar9;
  basic_ostream<char,std::char_traits<char>_> *pbVar10;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar11;
  QString *pQVar12;
  QImage *this_00;
  __int64 _Var13;
  ulonglong uVar14;
  undefined1 auStackY_218 [32];
  QString local_1d8 [8];
  QString local_1d0 [8];
  QFile local_1c8 [16];
  undefined4 local_1b8;
  QString local_1b0 [8];
  QString local_1a8 [8];
  QDir local_1a0 [8];
  QChar local_198 [8];
  QFileInfo local_190 [16];
  QString local_180 [4];
  int iStack_17c;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  undefined8 local_88;
  undefined ***local_80 [2];
  undefined8 local_70;
  ulonglong local_68;
  ulonglong local_60 [3];
  ulonglong local_48;
  ulonglong local_40;
  
  local_88 = 0xfffffffffffffffe;
  local_40 = DAT_140e88880 ^ (ulonglong)auStackY_218;
  local_1b8 = 0;
  QFile::QFile(local_1c8,param_4);
  this = (QFileInfo *)QFileInfo::QFileInfo(local_190,param_4);
  QFileInfo::path(this);
  pQVar4 = (QString *)QDir::toNativeSeparators(local_1a8);
  QDir::QDir(local_1a0,pQVar4);
  QString::~QString(local_1a8);
  QString::~QString(local_180);
  QFileInfo::~QFileInfo(local_190);
  pQVar4 = (QString *)QDir::path(local_1a0);
  lpFileName = (LPCWSTR)QString::toStdWString(pQVar4);
  if (7 < *(ulonglong *)(lpFileName + 0xc)) {
    lpFileName = *(LPCWSTR *)lpFileName;
  }
  hObject = CreateFileW(lpFileName,0x40000000,1,(LPSECURITY_ATTRIBUTES)0x0,3,0x2000000,(HANDLE)0x0);
  if (7 < local_68) {
    thunk_FUN_1402a7f50(local_80,local_80[0]);
  }
  local_68 = 7;
  local_70 = 0;
  local_80[0] = (undefined ***)((ulonglong)local_80[0] & 0xffffffffffff0000);
  QString::~QString(local_1d8);
  if (hObject == (HANDLE)0xffffffffffffffff) {
    uVar5 = thunk_FUN_14039be80(local_80);
    cVar1 = thunk_FUN_14039ba40(uVar5,40000);
    thunk_FUN_14039a8a0(local_80);
    if (cVar1 != '\0') {
      local_178 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_f0);
      local_1b8 = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
           *(int *)(local_178 + 4) + -0x88;
      local_80[0] = &local_170;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
      local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_108 = 0;
      local_100 = 4;
      pQVar4 = (QString *)QFile::fileName(local_1c8);
      uVar5 = thunk_FUN_14006fb70(&local_178,"Error save screenshoot file ");
      pQVar6 = (QByteArray *)QString::toLocal8Bit(pQVar4);
      pcVar7 = QByteArray::data(pQVar6);
      thunk_FUN_14006fb70(uVar5,pcVar7);
      QByteArray::~QByteArray((QByteArray *)local_1d8);
      QString::~QString(local_1d0);
      uVar5 = thunk_FUN_1400876c0(&local_178,local_60);
      uVar8 = thunk_FUN_14039be80(local_80);
      thunk_FUN_14039b070(uVar8,40000,uVar5,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\monitor.cpp"
                         );
      thunk_FUN_14039a8a0(local_80);
      if (0xf < local_48) {
        uVar14 = local_60[0];
        if (0xfff < local_48 + 1) {
          if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar14 = *(ulonglong *)(local_60[0] - 8);
          if (local_60[0] <= uVar14) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_60[0] - uVar14 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_60[0] - uVar14) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar14);
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
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_f0);
    }
    uVar5 = QFile::fileName(local_1c8);
    pQVar4 = (QString *)thunk_FUN_1402f43a0(local_1d0,"Error save screenshoot file %1",0,0xffffffff)
    ;
    QChar::QChar(local_198,0x20);
    QString::arg(pQVar4,param_2,uVar5,0);
    QString::~QString(local_1d0);
    QString::~QString(local_1d8);
  }
  else {
    CloseHandle(hObject);
    bVar2 = QFile::open(local_1c8,3);
    if (bVar2) {
      QByteArray::QByteArray((QByteArray *)local_1a8);
      QBuffer::QBuffer((QBuffer *)local_190,(QByteArray *)local_1a8,(QObject *)0x0);
      QBuffer::open((QBuffer *)local_190,2);
      this_00 = (QImage *)thunk_FUN_1402f17a0(param_1 + 200,param_3);
      QImage::save(this_00,(QIODevice *)local_190,"png",-1);
      QBuffer::close((QBuffer *)local_190);
      thunk_FUN_1402f2480(param_1 + 200,param_3);
      _Var13 = QIODevice::write((QIODevice *)local_1c8,(QByteArray *)local_1a8);
      if (_Var13 == -1) {
        QFileDevice::close((QFileDevice *)local_1c8);
        uVar5 = thunk_FUN_14039be80(local_80);
        cVar1 = thunk_FUN_14039ba40(uVar5,40000);
        thunk_FUN_14039a8a0(local_80);
        if (cVar1 != '\0') {
          local_178 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_f0);
          local_1b8 = 8;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
          *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
               *(int *)(local_178 + 4) + -0x88;
          thunk_FUN_140076630(&local_170,2);
          pQVar4 = (QString *)QIODevice::errorString((QIODevice *)local_1c8);
          pQVar9 = (QString *)QFile::fileName(local_1c8);
          uVar5 = thunk_FUN_14006fb70(&local_178,"Failed to write file ");
          pQVar6 = (QByteArray *)QString::toLocal8Bit(pQVar9);
          pcVar7 = QByteArray::data(pQVar6);
          thunk_FUN_14006fb70(uVar5,pcVar7);
          QByteArray::~QByteArray((QByteArray *)local_80);
          FVar3 = QFileDevice::error((QFileDevice *)local_1c8);
          pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                    thunk_FUN_14006fb70(uVar5,&DAT_1405bc63c);
          pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,FVar3);
          uVar5 = thunk_FUN_14006fb70(pbVar11,&DAT_1405a1f6c);
          pQVar6 = (QByteArray *)QString::toLocal8Bit(pQVar4);
          pcVar7 = QByteArray::data(pQVar6);
          thunk_FUN_14006fb70(uVar5,pcVar7);
          QByteArray::~QByteArray((QByteArray *)local_1b0);
          QString::~QString(local_1d0);
          QString::~QString(local_1d8);
          uVar5 = thunk_FUN_1400876c0(&local_178,local_60);
          uVar8 = thunk_FUN_14039be80(local_80);
          thunk_FUN_14039b070(uVar8,40000,uVar5,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\monitor.cpp"
                             );
          thunk_FUN_14039a8a0(local_80);
          if (0xf < local_48) {
            uVar14 = local_60[0];
            if (0xfff < local_48 + 1) {
              if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar14 = *(ulonglong *)(local_60[0] - 8);
              if (local_60[0] <= uVar14) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_60[0] - uVar14 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_60[0] - uVar14) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar14);
          }
          thunk_FUN_140077af0(local_f0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_f0);
        }
        QIODevice::errorString((QIODevice *)local_1c8);
        FVar3 = QFileDevice::error((QFileDevice *)local_1c8);
        pQVar4 = (QString *)QString::number((int)local_1d0,FVar3);
        pQVar9 = (QString *)QFile::fileName(local_1c8);
        pQVar12 = (QString *)
                  thunk_FUN_1402f43a0(local_80,"Failed to write file %1 with error %2: %3",0,
                                      0xffffffff);
        QString::arg(pQVar12,param_2,pQVar9,pQVar4);
        QString::~QString((QString *)local_80);
        QString::~QString(local_1b0);
        QString::~QString(local_1d0);
        QString::~QString(local_1d8);
      }
      else {
        QFileDevice::close((QFileDevice *)local_1c8);
        QString::QString(param_2);
      }
      QBuffer::~QBuffer((QBuffer *)local_190);
      QByteArray::~QByteArray((QByteArray *)local_1a8);
    }
    else {
      thunk_FUN_1402f2480(param_1 + 200,param_3);
      uVar5 = thunk_FUN_14039be80(local_80);
      cVar1 = thunk_FUN_14039ba40(uVar5,40000);
      thunk_FUN_14039a8a0(local_80);
      if (cVar1 != '\0') {
        local_178 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_f0);
        local_1b8 = 4;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
        *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
             *(int *)(local_178 + 4) + -0x88;
        thunk_FUN_140076630(&local_170,2);
        pQVar4 = (QString *)QIODevice::errorString((QIODevice *)local_1c8);
        pQVar9 = (QString *)QFile::fileName(local_1c8);
        uVar5 = thunk_FUN_14006fb70(&local_178,"Failed to open file ");
        pQVar6 = (QByteArray *)QString::toLocal8Bit(pQVar9);
        pcVar7 = QByteArray::data(pQVar6);
        thunk_FUN_14006fb70(uVar5,pcVar7);
        QByteArray::~QByteArray((QByteArray *)local_1d0);
        FVar3 = QFileDevice::error((QFileDevice *)local_1c8);
        pbVar10 = (basic_ostream<char,std::char_traits<char>_> *)
                  thunk_FUN_14006fb70(uVar5," in write mode with error ");
        pbVar11 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar10,FVar3);
        uVar5 = thunk_FUN_14006fb70(pbVar11,&DAT_1405a1f6c);
        pQVar6 = (QByteArray *)QString::toLocal8Bit(pQVar4);
        pcVar7 = QByteArray::data(pQVar6);
        thunk_FUN_14006fb70(uVar5,pcVar7);
        QByteArray::~QByteArray((QByteArray *)local_1d8);
        QString::~QString(local_1b0);
        QString::~QString((QString *)local_80);
        uVar5 = thunk_FUN_1400876c0(&local_178,local_60);
        uVar8 = thunk_FUN_14039be80(local_80);
        thunk_FUN_14039b070(uVar8,40000,uVar5,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\monitor.cpp"
                           );
        thunk_FUN_14039a8a0(local_80);
        if (0xf < local_48) {
          uVar14 = local_60[0];
          if (0xfff < local_48 + 1) {
            if ((local_60[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar14 = *(ulonglong *)(local_60[0] - 8);
            if (local_60[0] <= uVar14) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_60[0] - uVar14 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_60[0] - uVar14) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar14);
        }
        thunk_FUN_140077af0(local_f0);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_f0);
      }
      QIODevice::errorString((QIODevice *)local_1c8);
      FVar3 = QFileDevice::error((QFileDevice *)local_1c8);
      pQVar4 = (QString *)QString::number((int)local_1d0,FVar3);
      pQVar9 = (QString *)QFile::fileName(local_1c8);
      pQVar12 = (QString *)
                thunk_FUN_1402f43a0(local_80,
                                    "Failed to open file %1 in write mode with error %2: %3",0,
                                    0xffffffff);
      QString::arg(pQVar12,param_2,pQVar9,pQVar4);
      QString::~QString((QString *)local_80);
      QString::~QString(local_1b0);
      QString::~QString(local_1d0);
      QString::~QString(local_1d8);
    }
  }
  QDir::~QDir(local_1a0);
  QFile::~QFile(local_1c8);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14032b270
// -----------------------------------------------------------------------------
void FUN_14032b270(undefined8 *param_1)

{
  QHashData *pQVar1;
  uint uVar2;
  int *piVar3;
  QHashData *this;
  longlong lVar4;
  int iVar5;
  QMapDataBase *pQVar6;
  ulonglong *puVar7;
  Node *pNVar8;
  longlong *plVar9;
  QMapDataBase *local_60;
  QMapNodeBase *local_58;
  QHashData *local_40;
  
  *param_1 = viewer::online::engine::VideoLoader::vftable;
  thunk_FUN_14018ec00();
  if (*(int *)(param_1[0x36] + 4) != 0) {
    qt_assert("_trackedMonitors.isEmpty()",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
              ,0x31);
  }
  piVar3 = (int *)param_1[0x36];
  if (*piVar3 == 0) {
    pQVar6 = QMapDataBase::createData();
    if (*(longlong *)(param_1[0x36] + 0x10) != 0) {
      puVar7 = (ulonglong *)thunk_FUN_14032e640(*(longlong *)(param_1[0x36] + 0x10),pQVar6);
      *(ulonglong **)(pQVar6 + 0x10) = puVar7;
      *puVar7 = *puVar7 & 3;
      *puVar7 = *puVar7 | (ulonglong)(pQVar6 + 8);
      QMapDataBase::recalcMostLeftNode(pQVar6);
    }
  }
  else {
    if (*piVar3 != -1) {
      LOCK();
      *piVar3 = *piVar3 + 1;
      UNLOCK();
    }
    pQVar6 = (QMapDataBase *)param_1[0x36];
  }
  if (*(int *)pQVar6 == 0) {
    local_60 = QMapDataBase::createData();
    if (*(longlong *)(pQVar6 + 0x10) != 0) {
      puVar7 = (ulonglong *)thunk_FUN_14032e640(*(longlong *)(pQVar6 + 0x10),local_60);
      *(ulonglong **)(local_60 + 0x10) = puVar7;
      *puVar7 = *puVar7 & 3;
      *puVar7 = *puVar7 | (ulonglong)(local_60 + 8);
      QMapDataBase::recalcMostLeftNode(local_60);
    }
  }
  else {
    local_60 = pQVar6;
    if (*(int *)pQVar6 != -1) {
      LOCK();
      *(int *)pQVar6 = *(int *)pQVar6 + 1;
      UNLOCK();
    }
  }
  if (*(longlong *)(local_60 + 0x10) == 0) {
    local_58 = (QMapNodeBase *)(local_60 + 8);
  }
  else {
    local_58 = *(QMapNodeBase **)(local_60 + 0x20);
  }
  for (; local_58 != (QMapNodeBase *)(local_60 + 8); local_58 = QMapNodeBase::nextNode(local_58)) {
    this = *(QHashData **)(local_58 + 0x40);
    pQVar1 = this + 0x10;
    if (1 < *(uint *)pQVar1 + 1) {
      LOCK();
      *(uint *)pQVar1 = *(uint *)pQVar1 + 1;
      UNLOCK();
    }
    local_40 = this;
    if ((((byte)this[0x28] & 1) == 0) && (1 < *(uint *)pQVar1)) {
      local_40 = QHashData::detach_helper
                           (this,(_func_void_Node_ptr_void_ptr *)&LAB_14001afaf,
                            (_func_void_Node_ptr *)&LAB_14000eb8d,0x18,8);
      if (*(uint *)pQVar1 == 0) {
LAB_14032b436:
        QHashData::free_helper(this,(_func_void_Node_ptr *)&LAB_14000eb8d);
      }
      else if (*(uint *)pQVar1 != 0xffffffff) {
        LOCK();
        uVar2 = *(uint *)pQVar1;
        *(uint *)pQVar1 = *(uint *)pQVar1 - 1;
        UNLOCK();
        if (uVar2 == 1) goto LAB_14032b436;
      }
    }
    plVar9 = *(longlong **)(local_40 + 8);
    iVar5 = *(int *)(local_40 + 0x20);
    while (pNVar8 = (Node *)local_40, iVar5 != 0) {
      iVar5 = iVar5 + -1;
      if ((QHashData *)*plVar9 != local_40) {
        pNVar8 = (Node *)*plVar9;
        break;
      }
      plVar9 = plVar9 + 1;
    }
    for (; pNVar8 != (Node *)local_40; pNVar8 = QHashData::nextNode(pNVar8)) {
      thunk_FUN_140339cb0(param_1,*(undefined8 *)(pNVar8 + 0x10));
    }
    if (*(int *)(local_40 + 0x10) == 0) {
LAB_14032b4d8:
      QHashData::free_helper(local_40,(_func_void_Node_ptr *)&LAB_14000eb8d);
    }
    else if (*(int *)(local_40 + 0x10) != -1) {
      LOCK();
      pQVar1 = local_40 + 0x10;
      iVar5 = *(int *)pQVar1;
      *(int *)pQVar1 = *(int *)pQVar1 + -1;
      UNLOCK();
      if (iVar5 == 1) goto LAB_14032b4d8;
    }
  }
  if (*(int *)local_60 == 0) {
LAB_14032b532:
    lVar4 = *(longlong *)(local_60 + 0x10);
    if (lVar4 != 0) {
      QString::~QString((QString *)(lVar4 + 0x28));
      QString::~QString((QString *)(lVar4 + 0x20));
      iVar5 = *(int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
      if (iVar5 == 0) {
LAB_14032b573:
        QHashData::free_helper(*(QHashData **)(lVar4 + 0x40),(_func_void_Node_ptr *)&LAB_14000eb8d);
      }
      else if (iVar5 != -1) {
        LOCK();
        piVar3 = (int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
        iVar5 = *piVar3;
        *piVar3 = *piVar3 + -1;
        UNLOCK();
        if (iVar5 == 1) goto LAB_14032b573;
      }
      if (*(longlong *)(lVar4 + 8) != 0) {
        thunk_FUN_14032fb90();
      }
      if (*(longlong *)(lVar4 + 0x10) != 0) {
        thunk_FUN_14032fb90();
      }
      QMapDataBase::freeTree(local_60,*(QMapNodeBase **)(local_60 + 0x10),8);
    }
    QMapDataBase::freeData(local_60);
  }
  else if (*(int *)local_60 != -1) {
    LOCK();
    iVar5 = *(int *)local_60;
    *(int *)local_60 = *(int *)local_60 + -1;
    UNLOCK();
    if (iVar5 == 1) goto LAB_14032b532;
  }
  if (*(int *)pQVar6 == 0) {
LAB_14032b5e0:
    lVar4 = *(longlong *)(pQVar6 + 0x10);
    if (lVar4 != 0) {
      QString::~QString((QString *)(lVar4 + 0x28));
      QString::~QString((QString *)(lVar4 + 0x20));
      iVar5 = *(int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
      if (iVar5 == 0) {
LAB_14032b61c:
        QHashData::free_helper(*(QHashData **)(lVar4 + 0x40),(_func_void_Node_ptr *)&LAB_14000eb8d);
      }
      else if (iVar5 != -1) {
        LOCK();
        piVar3 = (int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
        iVar5 = *piVar3;
        *piVar3 = *piVar3 + -1;
        UNLOCK();
        if (iVar5 == 1) goto LAB_14032b61c;
      }
      if (*(longlong *)(lVar4 + 8) != 0) {
        thunk_FUN_14032fb90();
      }
      if (*(longlong *)(lVar4 + 0x10) != 0) {
        thunk_FUN_14032fb90();
      }
      QMapDataBase::freeTree(pQVar6,*(QMapNodeBase **)(pQVar6 + 0x10),8);
    }
    QMapDataBase::freeData(pQVar6);
  }
  else if (*(int *)pQVar6 != -1) {
    LOCK();
    iVar5 = *(int *)pQVar6;
    *(int *)pQVar6 = *(int *)pQVar6 + -1;
    UNLOCK();
    if (iVar5 == 1) goto LAB_14032b5e0;
  }
  QTimer::~QTimer((QTimer *)(param_1 + 0x3a));
  piVar3 = (int *)param_1[0x39];
  if (*piVar3 == 0) {
LAB_14032b69e:
    pQVar6 = (QMapDataBase *)param_1[0x39];
    lVar4 = *(longlong *)(pQVar6 + 0x10);
    if (lVar4 != 0) {
      QString::~QString((QString *)(lVar4 + 0x28));
      QString::~QString((QString *)(lVar4 + 0x20));
      iVar5 = *(int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
      if (iVar5 == 0) {
LAB_14032b6e1:
        QHashData::free_helper(*(QHashData **)(lVar4 + 0x40),(_func_void_Node_ptr *)&LAB_140007072);
      }
      else if (iVar5 != -1) {
        LOCK();
        piVar3 = (int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
        iVar5 = *piVar3;
        *piVar3 = *piVar3 + -1;
        UNLOCK();
        if (iVar5 == 1) goto LAB_14032b6e1;
      }
      if (*(longlong *)(lVar4 + 8) != 0) {
        thunk_FUN_14032fc20();
      }
      if (*(longlong *)(lVar4 + 0x10) != 0) {
        thunk_FUN_14032fc20();
      }
      QMapDataBase::freeTree(pQVar6,*(QMapNodeBase **)(pQVar6 + 0x10),8);
    }
    QMapDataBase::freeData(pQVar6);
  }
  else if (*piVar3 != -1) {
    LOCK();
    iVar5 = *piVar3;
    *piVar3 = *piVar3 + -1;
    UNLOCK();
    if (iVar5 == 1) goto LAB_14032b69e;
  }
  piVar3 = (int *)param_1[0x38];
  if (*piVar3 == 0) {
LAB_14032b751:
    pQVar6 = (QMapDataBase *)param_1[0x38];
    lVar4 = *(longlong *)(pQVar6 + 0x10);
    if (lVar4 != 0) {
      QString::~QString((QString *)(lVar4 + 0x28));
      QString::~QString((QString *)(lVar4 + 0x20));
      QString::~QString((QString *)(lVar4 + 0x50));
      QString::~QString((QString *)(lVar4 + 0x48));
      if (*(longlong *)(lVar4 + 8) != 0) {
        thunk_FUN_14032fa20();
      }
      if (*(longlong *)(lVar4 + 0x10) != 0) {
        thunk_FUN_14032fa20();
      }
      QMapDataBase::freeTree(pQVar6,*(QMapNodeBase **)(pQVar6 + 0x10),8);
    }
    QMapDataBase::freeData(pQVar6);
  }
  else if (*piVar3 != -1) {
    LOCK();
    iVar5 = *piVar3;
    *piVar3 = *piVar3 + -1;
    UNLOCK();
    if (iVar5 == 1) goto LAB_14032b751;
  }
  piVar3 = (int *)param_1[0x37];
  if (*piVar3 == 0) {
LAB_14032b7eb:
    pQVar6 = (QMapDataBase *)param_1[0x37];
    lVar4 = *(longlong *)(pQVar6 + 0x10);
    if (lVar4 != 0) {
      QString::~QString((QString *)(lVar4 + 0x28));
      QString::~QString((QString *)(lVar4 + 0x20));
      QImage::~QImage((QImage *)(lVar4 + 0x50));
      QDateTime::~QDateTime((QDateTime *)(lVar4 + 0x48));
      if (*(longlong *)(lVar4 + 8) != 0) {
        thunk_FUN_14032fa90();
      }
      if (*(longlong *)(lVar4 + 0x10) != 0) {
        thunk_FUN_14032fa90();
      }
      QMapDataBase::freeTree(pQVar6,*(QMapNodeBase **)(pQVar6 + 0x10),8);
    }
    QMapDataBase::freeData(pQVar6);
  }
  else if (*piVar3 != -1) {
    LOCK();
    iVar5 = *piVar3;
    *piVar3 = *piVar3 + -1;
    UNLOCK();
    if (iVar5 == 1) goto LAB_14032b7eb;
  }
  piVar3 = (int *)param_1[0x36];
  if (*piVar3 != 0) {
    if (*piVar3 == -1) goto LAB_14032b919;
    LOCK();
    iVar5 = *piVar3;
    *piVar3 = *piVar3 + -1;
    UNLOCK();
    if (iVar5 != 1) goto LAB_14032b919;
  }
  pQVar6 = (QMapDataBase *)param_1[0x36];
  lVar4 = *(longlong *)(pQVar6 + 0x10);
  if (lVar4 != 0) {
    QString::~QString((QString *)(lVar4 + 0x28));
    QString::~QString((QString *)(lVar4 + 0x20));
    iVar5 = *(int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
    if (iVar5 == 0) {
LAB_14032b8cb:
      QHashData::free_helper(*(QHashData **)(lVar4 + 0x40),(_func_void_Node_ptr *)&LAB_14000eb8d);
    }
    else if (iVar5 != -1) {
      LOCK();
      piVar3 = (int *)(*(longlong *)(lVar4 + 0x40) + 0x10);
      iVar5 = *piVar3;
      *piVar3 = *piVar3 + -1;
      UNLOCK();
      if (iVar5 == 1) goto LAB_14032b8cb;
    }
    if (*(longlong *)(lVar4 + 8) != 0) {
      thunk_FUN_14032fb90();
    }
    if (*(longlong *)(lVar4 + 0x10) != 0) {
      thunk_FUN_14032fb90();
    }
    QMapDataBase::freeTree(pQVar6,*(QMapNodeBase **)(pQVar6 + 0x10),8);
  }
  QMapDataBase::freeData(pQVar6);
LAB_14032b919:
  thunk_FUN_1401965b0(param_1 + 0x10);
  QString::~QString((QString *)(param_1 + 0xf));
  plVar9 = (longlong *)param_1[0xe];
  if (plVar9 != (longlong *)0x0) {
    (**(code **)(*plVar9 + 0x18))(plVar9,1);
  }
  thunk_FUN_140190390(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140332c60
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140332c60(longlong param_1)

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
             thunk_FUN_14006fb70(&local_138,"video connection closed for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1a0));
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar3 = thunk_FUN_14039be80(local_150);
      local_168 = 0x141;
      thunk_FUN_14039b070(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-online\\engine\\videoLoader.cpp"
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
  }
  thunk_FUN_14032de00(param_1 + 0x1b8);
  thunk_FUN_14032dd50(param_1 + 0x1c0);
  thunk_FUN_14032deb0(param_1 + 0x1c8);
  thunk_FUN_140190b30(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140334cd0
// -----------------------------------------------------------------------------
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
          QHashData::free_helper(pQVar36,(_func_void_Node_ptr *)&LAB_14000eb8d);
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
          QHashData::free_helper(this,(_func_void_Node_ptr *)&LAB_14000eb8d);
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
  QHashData::free_helper((QHashData *)local_f18,(_func_void_Node_ptr *)&LAB_140007072);
  return;
}

// =============================================================================
// ORGANIZATIE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14033ed80
// -----------------------------------------------------------------------------
undefined8 FUN_14033ed80(longlong param_1,undefined8 param_2)

{
  if (*(longlong *)(param_1 + 0x18) == 0) {
    qt_assert("_engine",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\index_54.cpp",
              0x32);
  }
  thunk_FUN_14034ea60(*(undefined8 *)(param_1 + 0x18),param_2);
  return param_2;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1403485f0
// -----------------------------------------------------------------------------
void FUN_1403485f0(longlong param_1,undefined8 param_2,int param_3,int param_4)

{
  char *pcVar1;
  int iVar2;
  longlong local_30;
  undefined8 uStack_28;
  longlong local_20;
  undefined8 uStack_18;
  
  if (param_3 == 10) {
    local_20 = 0;
    uStack_18 = 0;
    thunk_FUN_140358f70(param_1 + 0x20,&local_30,param_2,&local_20,0xfffffffffffffffe);
    thunk_FUN_140348dd0(uStack_18);
    if (local_30 != 0) {
      if (param_4 == 10) {
        thunk_FUN_1403610b0();
      }
      else if ((param_4 == 0x14) || (param_4 == 0x1e)) {
        qt_assert("!\"bad oek\"",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                  ,0x331);
      }
      else {
        qt_assert("!\"unknown oek\"",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                  ,0x334);
      }
    }
    thunk_FUN_140348dd0(uStack_28);
    goto LAB_1403487e1;
  }
  if (param_3 == 0x14) {
    local_20 = 0;
    uStack_18 = 0;
    thunk_FUN_140358fe0(param_1 + 0x30,&local_30,param_2,&local_20);
    thunk_FUN_140348ea0(uStack_18);
    if (local_30 != 0) {
      if (param_4 == 10) {
        thunk_FUN_14035b600();
      }
      else if ((param_4 == 0x14) || (param_4 == 0x1e)) {
        qt_assert("!\"bad oek\"",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                  ,0x346);
      }
      else {
        qt_assert("!\"unknown oek\"",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                  ,0x349);
      }
    }
    thunk_FUN_140348ea0(uStack_28);
    goto LAB_1403487e1;
  }
  if (param_3 != 0x1e) {
    qt_assert("!\"unknown oek\"",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
              ,0x365);
    goto LAB_1403487e1;
  }
  local_30 = 0;
  uStack_28 = 0;
  thunk_FUN_140359050(param_1 + 0x38,&local_20,param_2,&local_30);
  thunk_FUN_140349040(uStack_28);
  if (local_20 != 0) {
    if (param_4 == 10) {
LAB_14034869c:
      iVar2 = 0x35b;
      pcVar1 = "!\"bad oek\"";
    }
    else {
      if (param_4 == 0x14) {
        thunk_FUN_140368700();
        goto LAB_1403486b7;
      }
      if (param_4 == 0x1e) goto LAB_14034869c;
      iVar2 = 0x35e;
      pcVar1 = "!\"unknown oek\"";
    }
    qt_assert(pcVar1,
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
              ,iVar2);
  }
LAB_1403486b7:
  thunk_FUN_140349040(uStack_18);
LAB_1403487e1:
  thunk_FUN_140368b20(param_1 + -0x18);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1403514a0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1403514a0(longlong param_1,int param_2,QListData *param_3)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  void **ppvVar7;
  longlong lVar8;
  QString *pQVar9;
  QByteArray *pQVar10;
  char *pcVar11;
  ulonglong uVar12;
  undefined ***pppuVar13;
  longlong lVar14;
  longlong lVar15;
  int iVar16;
  undefined1 auStack_1d8 [32];
  undefined4 local_1b8;
  undefined4 local_1a8;
  longlong local_1a0;
  QByteArray local_198 [8];
  undefined1 local_190 [20];
  int iStack_17c;
  undefined *local_178;
  undefined **local_170;
  basic_ostream<char,std::char_traits<char>_> local_168 [96];
  undefined8 local_108;
  undefined4 local_100;
  basic_ios<char,std::char_traits<char>_> local_f0 [104];
  undefined8 local_88;
  undefined ***local_80 [3];
  ulonglong local_68;
  ulonglong local_60 [3];
  ulonglong local_48;
  ulonglong local_40;
  
  local_88 = 0xfffffffffffffffe;
  local_40 = DAT_140e88880 ^ (ulonglong)auStack_1d8;
  local_1a8 = 0;
  local_1a0 = param_1;
  if (param_2 == 10) {
    iVar16 = 0;
    iVar3 = QListData::size(param_3);
    if (0 < iVar3) {
      do {
        if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
        }
        if (1 < **(uint **)param_3) {
          thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
        }
        pQVar9 = (QString *)QListData::at(param_3,iVar16);
        lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x10);
        if (lVar8 == 0) {
LAB_140351df4:
          if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
            qt_assert_x("QList<T>::operator[]","index out of range",
                        "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b
                       );
          }
          if (1 < **(uint **)param_3) {
            thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
          }
          lVar8 = local_1a0;
          pQVar9 = (QString *)QListData::at(param_3,iVar16);
          bVar2 = operator==(pQVar9,(QString *)(lVar8 + 0x18));
          if (bVar2) goto LAB_1403520fa;
          uVar4 = thunk_FUN_14039be80(local_60);
          cVar1 = thunk_FUN_14039ba40(uVar4,30000);
          thunk_FUN_14039a8a0(local_60);
          if (cVar1 != '\0') {
            local_178 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_f0);
            local_1a8 = 1;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
            *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
                 *(int *)(local_178 + 4) + -0x88;
            local_80[0] = &local_170;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
            local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_108 = 0;
            local_100 = 4;
            if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x21b);
            }
            if (1 < **(uint **)param_3) {
              thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
            }
            pQVar9 = (QString *)QListData::at(param_3,iVar16);
            uVar4 = thunk_FUN_14006fb70(&local_178,"invalid id filtered, ");
            pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
            pcVar11 = QByteArray::data(pQVar10);
            thunk_FUN_14006fb70(uVar4,pcVar11);
            QByteArray::~QByteArray(local_198);
            pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70(uVar4,&DAT_1405a3ec4);
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,10);
            uVar4 = thunk_FUN_1400876c0(&local_178,local_80);
            uVar6 = thunk_FUN_14039be80(local_190);
            local_1b8 = 0x2ac;
            thunk_FUN_14039b070(uVar6,30000,uVar4,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                               );
            thunk_FUN_14039a8a0(local_190);
            if (0xf < local_68) {
              pppuVar13 = local_80[0];
              if (0xfff < local_68 + 1) {
                if (((ulonglong)local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                pppuVar13 = (undefined ***)local_80[0][-1];
                if (local_80[0] <= pppuVar13) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if ((ulonglong)((longlong)local_80[0] - (longlong)pppuVar13) < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < (ulonglong)((longlong)local_80[0] - (longlong)pppuVar13)) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(pppuVar13);
            }
            *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
                 *(int *)(local_178 + 4) + -0x88;
            local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14007bb20(&local_170);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_168);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_f0);
          }
          if ((-1 < iVar16) && (iVar3 = QListData::size(param_3), iVar16 < iVar3)) {
            if (1 < **(uint **)param_3) {
              thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
            }
            pQVar9 = (QString *)QListData::at(param_3,iVar16);
            QString::~QString(pQVar9);
            QListData::remove(param_3,iVar16);
          }
        }
        else {
          lVar14 = 0;
          do {
            bVar2 = operator<((QString *)(lVar8 + 0x18),pQVar9);
            if (bVar2) {
              lVar15 = *(longlong *)(lVar8 + 0x10);
            }
            else {
              lVar15 = *(longlong *)(lVar8 + 8);
              lVar14 = lVar8;
            }
            lVar8 = lVar15;
          } while (lVar15 != 0);
          if ((lVar14 == 0) || (bVar2 = operator<(pQVar9,(QString *)(lVar14 + 0x18)), bVar2))
          goto LAB_140351df4;
LAB_1403520fa:
          iVar16 = iVar16 + 1;
        }
        iVar3 = QListData::size(param_3);
        param_1 = local_1a0;
      } while (iVar16 < iVar3);
    }
  }
  else if (param_2 == 0x14) {
    iVar16 = 0;
    iVar3 = QListData::size(param_3);
    if (0 < iVar3) {
      do {
        if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
        }
        if (1 < **(uint **)param_3) {
          thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
        }
        ppvVar7 = QListData::at(param_3,iVar16);
        lVar8 = thunk_FUN_14034e7d0(*(undefined8 *)(param_1 + 0x38),ppvVar7);
        if (lVar8 == 0) {
          uVar4 = thunk_FUN_14039be80(local_60);
          cVar1 = thunk_FUN_14039ba40(uVar4,30000);
          thunk_FUN_14039a8a0(local_60);
          if (cVar1 != '\0') {
            local_178 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_f0);
            local_1a8 = 2;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
            *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
                 *(int *)(local_178 + 4) + -0x88;
            local_80[0] = &local_170;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
            local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_108 = 0;
            local_100 = 4;
            if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x21b);
            }
            if (1 < **(uint **)param_3) {
              thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
            }
            pQVar9 = (QString *)QListData::at(param_3,iVar16);
            uVar4 = thunk_FUN_14006fb70(&local_178,"invalid id filtered, ");
            pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
            pcVar11 = QByteArray::data(pQVar10);
            thunk_FUN_14006fb70(uVar4,pcVar11);
            QByteArray::~QByteArray(local_198);
            pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70(uVar4,&DAT_1405a3ec4);
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,0x14);
            uVar4 = thunk_FUN_1400876c0(&local_178,local_80);
            uVar6 = thunk_FUN_14039be80(local_190);
            local_1b8 = 0x2ba;
            thunk_FUN_14039b070(uVar6,30000,uVar4,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                               );
            thunk_FUN_14039a8a0(local_190);
            if (0xf < local_68) {
              pppuVar13 = local_80[0];
              if (0xfff < local_68 + 1) {
                if (((ulonglong)local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                pppuVar13 = (undefined ***)local_80[0][-1];
                if (local_80[0] <= pppuVar13) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if ((ulonglong)((longlong)local_80[0] - (longlong)pppuVar13) < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < (ulonglong)((longlong)local_80[0] - (longlong)pppuVar13)) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(pppuVar13);
            }
            *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
                 *(int *)(local_178 + 4) + -0x88;
            local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            thunk_FUN_14007bb20(&local_170);
            std::basic_streambuf<char,std::char_traits<char>_>::
            ~basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
            std::basic_ostream<char,std::char_traits<char>_>::
            ~basic_ostream<char,std::char_traits<char>_>(local_168);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_f0);
          }
          param_1 = local_1a0;
          if ((-1 < iVar16) &&
             (iVar3 = QListData::size(param_3), param_1 = local_1a0, iVar16 < iVar3)) {
            if (1 < **(uint **)param_3) {
              thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
            }
            pQVar9 = (QString *)QListData::at(param_3,iVar16);
            QString::~QString(pQVar9);
            QListData::remove(param_3,iVar16);
            param_1 = local_1a0;
          }
        }
        else {
          iVar16 = iVar16 + 1;
        }
        iVar3 = QListData::size(param_3);
      } while (iVar16 < iVar3);
    }
  }
  else if (param_2 == 0x1e) {
    iVar16 = 0;
    iVar3 = QListData::size(param_3);
    if (0 < iVar3) {
      do {
        if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
          qt_assert_x("QList<T>::operator[]","index out of range",
                      "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",0x21b);
        }
        if (1 < **(uint **)param_3) {
          thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
        }
        ppvVar7 = QListData::at(param_3,iVar16);
        lVar8 = thunk_FUN_14034e870(*(undefined8 *)(param_1 + 0x40),ppvVar7);
        if (lVar8 == 0) {
          uVar4 = thunk_FUN_14039be80(local_190);
          cVar1 = thunk_FUN_14039ba40(uVar4,30000);
          thunk_FUN_14039a8a0(local_190);
          if (cVar1 != '\0') {
            local_178 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_f0);
            local_1a8 = 4;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
            *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
                 std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                 vftable;
            *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
                 *(int *)(local_178 + 4) + -0x88;
            local_80[0] = &local_170;
            std::basic_streambuf<char,std::char_traits<char>_>::
            basic_streambuf<char,std::char_traits<char>_>
                      ((basic_streambuf<char,std::char_traits<char>_> *)&local_170);
            local_170 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                        vftable;
            local_108 = 0;
            local_100 = 4;
            if ((iVar16 < 0) || (iVar3 = QListData::size(param_3), iVar3 <= iVar16)) {
              qt_assert_x("QList<T>::operator[]","index out of range",
                          "c:\\kickidler\\builder\\64-shared\\qt-5.6.2\\include\\QtCore/qlist.h",
                          0x21b);
            }
            if (1 < **(uint **)param_3) {
              thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
            }
            pQVar9 = (QString *)QListData::at(param_3,iVar16);
            uVar4 = thunk_FUN_14006fb70(&local_178,"invalid id filtered, ");
            pQVar10 = (QByteArray *)QString::toLocal8Bit(pQVar9);
            pcVar11 = QByteArray::data(pQVar10);
            thunk_FUN_14006fb70(uVar4,pcVar11);
            QByteArray::~QByteArray(local_198);
            pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70(uVar4,&DAT_1405a3ec4);
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,0x1e);
            uVar4 = thunk_FUN_1400876c0(&local_178,local_80);
            uVar6 = thunk_FUN_14039be80(local_60);
            local_1b8 = 0x2c8;
            thunk_FUN_14039b070(uVar6,30000,uVar4,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                               );
            thunk_FUN_14039a8a0(local_60);
            if (0xf < local_68) {
              pppuVar13 = local_80[0];
              if (0xfff < local_68 + 1) {
                if (((ulonglong)local_80[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                pppuVar13 = (undefined ***)local_80[0][-1];
                if (local_80[0] <= pppuVar13) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if ((ulonglong)((longlong)local_80[0] - (longlong)pppuVar13) < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < (ulonglong)((longlong)local_80[0] - (longlong)pppuVar13)) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(pppuVar13);
            }
            thunk_FUN_140077af0(local_f0);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_f0);
          }
          param_1 = local_1a0;
          if ((-1 < iVar16) &&
             (iVar3 = QListData::size(param_3), param_1 = local_1a0, iVar16 < iVar3)) {
            if (1 < **(uint **)param_3) {
              thunk_FUN_140080270(param_3,(*(uint **)param_3)[1]);
            }
            pQVar9 = (QString *)QListData::at(param_3,iVar16);
            QString::~QString(pQVar9);
            QListData::remove(param_3,iVar16);
            param_1 = local_1a0;
          }
        }
        else {
          iVar16 = iVar16 + 1;
        }
        iVar3 = QListData::size(param_3);
      } while (iVar16 < iVar3);
    }
  }
  else {
    qt_assert("!\"unknown oek\"",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
              ,0x2ce);
    uVar4 = thunk_FUN_14039be80(local_80);
    cVar1 = thunk_FUN_14039ba40(uVar4,40000);
    thunk_FUN_14039a8a0(local_80);
    if (cVar1 != '\0') {
      local_178 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_f0);
      local_1a8 = 8;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_178,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_170,false);
      *(undefined ***)((longlong)&local_178 + (longlong)*(int *)(local_178 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_17c + (longlong)*(int *)(local_178 + 4)) =
           *(int *)(local_178 + 4) + -0x88;
      thunk_FUN_140076630(&local_170,2);
      pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14006fb70(&local_178,"invalid oek detected, ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,param_2);
      uVar4 = thunk_FUN_1400876c0(&local_178,local_60);
      uVar6 = thunk_FUN_14039be80(local_80);
      local_1b8 = 0x2cf;
      thunk_FUN_14039b070(uVar6,40000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                         );
      thunk_FUN_14039a8a0(local_80);
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
      thunk_FUN_140077af0(local_f0);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_f0);
    }
    thunk_FUN_14007c7c0(param_3);
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1403580f0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined8 FUN_1403580f0(longlong param_1,int param_2,undefined8 param_3)

{
  int iVar1;
  char cVar2;
  bool bVar3;
  undefined8 uVar4;
  basic_ostream<char,std::char_traits<char>_> *pbVar5;
  undefined8 uVar6;
  longlong lVar7;
  QByteArray *pQVar8;
  char *pcVar9;
  ulonglong uVar10;
  longlong lVar11;
  QString *pQVar12;
  longlong lVar13;
  undefined1 auStack_5c8 [32];
  undefined4 local_5a8;
  undefined4 local_598;
  undefined ***local_590;
  Data *local_588;
  QString *local_580;
  QString *local_578;
  undefined4 local_570;
  QListData local_568 [8];
  QString *local_560;
  QString *local_558;
  undefined4 local_550;
  QListData local_548 [8];
  QString *local_540;
  QString *local_538;
  undefined4 local_530;
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
  basic_streambuf<char,std::char_traits<char>_> local_1b0 [128];
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
  local_48 = DAT_140e88880 ^ (ulonglong)auStack_5c8;
  local_598 = 0;
  if (param_2 != 10) {
    if (param_2 != 0x14) {
      if (param_2 != 0x1e) {
        qt_assert("!\"unknown oek\"",
                  "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                  ,0x297);
        uVar4 = thunk_FUN_14039be80(local_510);
        cVar2 = thunk_FUN_14039ba40(uVar4,40000);
        thunk_FUN_14039a8a0(local_510);
        if (cVar2 != '\0') {
          local_1b8 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_130);
          local_598 = 8;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_1b8,local_1b0,false);
          *(undefined ***)(local_1b0 + (longlong)*(int *)(local_1b8 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_1bc + (longlong)*(int *)(local_1b8 + 4)) =
               *(int *)(local_1b8 + 4) + -0x88;
          thunk_FUN_140076630(local_1b0,2);
          pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70(&local_1b8,"invalid oek detected, ");
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,param_2);
          uVar4 = thunk_FUN_1400876c0(&local_1b8,local_c8);
          uVar6 = thunk_FUN_14039be80(local_500);
          local_5a8 = 0x298;
          thunk_FUN_14039b070(uVar6,40000,uVar4,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                             );
          thunk_FUN_14039a8a0(local_500);
          if (0xf < local_b0) {
            uVar10 = local_c8[0];
            if (0xfff < local_b0 + 1) {
              if ((local_c8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar10 = *(ulonglong *)(local_c8[0] - 8);
              if (local_c8[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_c8[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_c8[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar10);
          }
          thunk_FUN_140077af0(local_130);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_130);
        }
        return 0;
      }
      thunk_FUN_1400755c0(local_568,param_3);
      local_560 = (QString *)QListData::begin(local_568);
      local_558 = (QString *)QListData::end(local_568);
      while( true ) {
        pQVar12 = local_560;
        local_550 = 1;
        if (local_560 == local_558) {
          local_550 = 1;
          thunk_FUN_1400775d0(local_568);
          return 1;
        }
        lVar7 = thunk_FUN_14034e870(*(undefined8 *)(param_1 + 0x40),local_560);
        if (lVar7 == 0) break;
        local_560 = local_560 + 8;
      }
      uVar4 = thunk_FUN_14039be80(local_4f0);
      cVar2 = thunk_FUN_14039ba40(uVar4,30000);
      thunk_FUN_14039a8a0(local_4f0);
      if (cVar2 != '\0') {
        local_488 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_400);
        local_598 = 4;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_488,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_480,false);
        *(undefined ***)((longlong)&local_488 + (longlong)*(int *)(local_488 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_48c + (longlong)*(int *)(local_488 + 4)) =
             *(int *)(local_488 + 4) + -0x88;
        local_590 = &local_480;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_480);
        local_480 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        local_418 = 0;
        local_410 = 4;
        uVar4 = thunk_FUN_14006fb70(&local_488,"invalid id detected, ");
        pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar12);
        pcVar9 = QByteArray::data(pQVar8);
        thunk_FUN_14006fb70(uVar4,pcVar9);
        QByteArray::~QByteArray(local_528);
        pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14006fb70(uVar4,&DAT_1405a3ec4);
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,0x1e);
        uVar4 = thunk_FUN_1400876c0(&local_488,local_a8);
        uVar6 = thunk_FUN_14039be80(local_4e0);
        local_5a8 = 0x291;
        thunk_FUN_14039b070(uVar6,30000,uVar4,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                           );
        thunk_FUN_14039a8a0(local_4e0);
        if (0xf < local_90) {
          uVar10 = local_a8[0];
          if (0xfff < local_90 + 1) {
            if ((local_a8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar10 = *(ulonglong *)(local_a8[0] - 8);
            if (local_a8[0] <= uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_a8[0] - uVar10 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_a8[0] - uVar10) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar10);
        }
        *(undefined ***)((longlong)&local_488 + (longlong)*(int *)(local_488 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_48c + (longlong)*(int *)(local_488 + 4)) =
             *(int *)(local_488 + 4) + -0x88;
        local_480 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable
        ;
        thunk_FUN_14007bb20(&local_480);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_480);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_478);
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_400);
      }
      thunk_FUN_1400775d0(local_568);
      return 0;
    }
    thunk_FUN_1400755c0(local_548,param_3);
    local_540 = (QString *)QListData::begin(local_548);
    local_538 = (QString *)QListData::end(local_548);
    while( true ) {
      pQVar12 = local_540;
      local_530 = 1;
      if (local_540 == local_538) {
        local_530 = 1;
        thunk_FUN_1400775d0(local_548);
        return 1;
      }
      lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x10);
      lVar13 = 0;
      if (lVar7 == 0) break;
      do {
        bVar3 = operator<((QString *)(lVar7 + 0x18),pQVar12);
        if (bVar3) {
          lVar11 = *(longlong *)(lVar7 + 0x10);
        }
        else {
          lVar11 = *(longlong *)(lVar7 + 8);
          lVar13 = lVar7;
        }
        lVar7 = lVar11;
      } while (lVar11 != 0);
      if ((lVar13 == 0) || (bVar3 = operator<(pQVar12,(QString *)(lVar13 + 0x18)), bVar3)) break;
      local_540 = local_540 + 8;
    }
    uVar4 = thunk_FUN_14039be80(local_4d0);
    cVar2 = thunk_FUN_14039ba40(uVar4,30000);
    thunk_FUN_14039a8a0(local_4d0);
    if (cVar2 != '\0') {
      local_398 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_310);
      local_598 = 2;
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_398,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_390,false);
      *(undefined ***)((longlong)&local_398 + (longlong)*(int *)(local_398 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_39c + (longlong)*(int *)(local_398 + 4)) =
           *(int *)(local_398 + 4) + -0x88;
      local_590 = &local_390;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_390);
      local_390 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_328 = 0;
      local_320 = 4;
      uVar4 = thunk_FUN_14006fb70(&local_398,"invalid id detected, ");
      pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar12);
      pcVar9 = QByteArray::data(pQVar8);
      thunk_FUN_14006fb70(uVar4,pcVar9);
      QByteArray::~QByteArray(local_520);
      pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14006fb70(uVar4,&DAT_1405a3ec4);
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,0x14);
      uVar4 = thunk_FUN_1400876c0(&local_398,local_88);
      uVar6 = thunk_FUN_14039be80(local_4c0);
      local_5a8 = 0x287;
      thunk_FUN_14039b070(uVar6,30000,uVar4,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                         );
      thunk_FUN_14039a8a0(local_4c0);
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
      *(undefined ***)((longlong)&local_398 + (longlong)*(int *)(local_398 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_39c + (longlong)*(int *)(local_398 + 4)) =
           *(int *)(local_398 + 4) + -0x88;
      local_390 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_390);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_390);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_388);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_310);
    }
    thunk_FUN_1400775d0(local_548);
    return 0;
  }
  thunk_FUN_1400755c0(&local_588,param_3);
  local_580 = (QString *)QListData::begin((QListData *)&local_588);
  local_578 = (QString *)QListData::end((QListData *)&local_588);
  do {
    pQVar12 = local_580;
    local_570 = 1;
    if (local_580 == local_578) {
      local_570 = 1;
      if (*(int *)local_588 != 0) {
        if (*(int *)local_588 == -1) {
          return 1;
        }
        LOCK();
        iVar1 = *(int *)local_588;
        *(int *)local_588 = *(int *)local_588 + -1;
        UNLOCK();
        if (iVar1 != 1) {
          return 1;
        }
      }
      pQVar12 = (QString *)(local_588 + ((longlong)*(int *)(local_588 + 0xc) + 2) * 8);
      iVar1 = *(int *)(local_588 + 8);
      while ((QString *)(local_588 + ((longlong)iVar1 + 2) * 8) != pQVar12) {
        pQVar12 = pQVar12 + -8;
        QString::~QString(pQVar12);
      }
      QListData::dispose(local_588);
      return 1;
    }
    lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x28) + 0x10);
    lVar13 = 0;
    if (lVar7 == 0) {
LAB_14035896b:
      bVar3 = operator==(pQVar12,(QString *)(param_1 + 0x18));
      if (!bVar3) {
        uVar4 = thunk_FUN_14039be80(local_4b0);
        cVar2 = thunk_FUN_14039ba40(uVar4,30000);
        thunk_FUN_14039a8a0(local_4b0);
        if (cVar2 != '\0') {
          local_2a8 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_220);
          local_598 = 1;
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_2a8,
                     (basic_streambuf<char,std::char_traits<char>_> *)&local_2a0,false);
          *(undefined ***)((longlong)&local_2a8 + (longlong)*(int *)(local_2a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
               *(int *)(local_2a8 + 4) + -0x88;
          local_590 = &local_2a0;
          std::basic_streambuf<char,std::char_traits<char>_>::
          basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_2a0);
          local_2a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          local_238 = 0;
          local_230 = 4;
          uVar4 = thunk_FUN_14006fb70(&local_2a8,"invalid id detected, ");
          pQVar8 = (QByteArray *)QString::toLocal8Bit(pQVar12);
          pcVar9 = QByteArray::data(pQVar8);
          thunk_FUN_14006fb70(uVar4,pcVar9);
          QByteArray::~QByteArray((QByteArray *)&local_590);
          pbVar5 = (basic_ostream<char,std::char_traits<char>_> *)
                   thunk_FUN_14006fb70(uVar4,&DAT_1405a3ec4);
          std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar5,10);
          uVar4 = thunk_FUN_1400876c0(&local_2a8,local_68);
          uVar6 = thunk_FUN_14039be80(local_4a0);
          local_5a8 = 0x27d;
          thunk_FUN_14039b070(uVar6,30000,uVar4,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\index_55.cpp"
                             );
          thunk_FUN_14039a8a0(local_4a0);
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
          *(undefined ***)((longlong)&local_2a8 + (longlong)*(int *)(local_2a8 + 4)) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&iStack_2ac + (longlong)*(int *)(local_2a8 + 4)) =
               *(int *)(local_2a8 + 4) + -0x88;
          local_2a0 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                      vftable;
          thunk_FUN_14007bb20(&local_2a0);
          std::basic_streambuf<char,std::char_traits<char>_>::
          ~basic_streambuf<char,std::char_traits<char>_>
                    ((basic_streambuf<char,std::char_traits<char>_> *)&local_2a0);
          std::basic_ostream<char,std::char_traits<char>_>::
          ~basic_ostream<char,std::char_traits<char>_>(local_298);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_220);
        }
        thunk_FUN_1400775d0(&local_588);
        return 0;
      }
    }
    else {
      do {
        bVar3 = operator<((QString *)(lVar7 + 0x18),pQVar12);
        if (bVar3) {
          lVar11 = *(longlong *)(lVar7 + 0x10);
        }
        else {
          lVar11 = *(longlong *)(lVar7 + 8);
          lVar13 = lVar7;
        }
        lVar7 = lVar11;
      } while (lVar11 != 0);
      if ((lVar13 == 0) || (bVar3 = operator<(pQVar12,(QString *)(lVar13 + 0x18)), bVar3))
      goto LAB_14035896b;
    }
    local_580 = local_580 + 8;
  } while( true );
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140359d00
// -----------------------------------------------------------------------------
void FUN_140359d00(QObject *param_1)

{
  int iVar1;
  int *piVar2;
  Data *pDVar3;
  QVariant *pQVar4;
  undefined8 uVar5;
  Data *pDVar6;
  Data *local_res8;
  Data *local_50;
  void **local_48;
  void **local_40;
  int local_38;
  
  *(undefined ***)param_1 = viewer::organization::engine::Employee::vftable;
  if (*(int *)(*(longlong *)(param_1 + 0x10) + 0x14) != 0) {
    qt_assert("_bridges.isEmpty()",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\employee_55.cpp"
              ,0x30);
  }
  uVar5 = thunk_FUN_14035cda0(param_1 + 0x10,&local_res8);
  thunk_FUN_1403595c0(&local_50,uVar5);
  local_48 = QListData::begin((QListData *)&local_50);
  local_40 = QListData::end((QListData *)&local_50);
  local_38 = 1;
  if (*(int *)local_res8 == 0) {
LAB_140359dc6:
    QListData::dispose(local_res8);
  }
  else if (*(int *)local_res8 != -1) {
    LOCK();
    iVar1 = *(int *)local_res8;
    *(int *)local_res8 = *(int *)local_res8 + -1;
    UNLOCK();
    if (iVar1 == 1) goto LAB_140359dc6;
  }
  if (local_38 != 0) {
    for (; local_48 != local_40; local_48 = local_48 + 1) {
      if (local_38 != 0) {
        thunk_FUN_14033c0d0(*local_48,0);
        thunk_FUN_14035ae80(param_1);
      }
      local_38 = 1;
    }
  }
  if (*(int *)local_50 == 0) {
LAB_140359e44:
    QListData::dispose(local_50);
  }
  else if (*(int *)local_50 != -1) {
    LOCK();
    iVar1 = *(int *)local_50;
    *(int *)local_50 = *(int *)local_50 + -1;
    UNLOCK();
    if (iVar1 == 1) goto LAB_140359e44;
  }
  QString::~QString((QString *)(param_1 + 0x58));
  QDateTime::~QDateTime((QDateTime *)(param_1 + 0x48));
  piVar2 = *(int **)(param_1 + 0x40);
  if (*piVar2 == 0) {
LAB_140359e7c:
    pDVar3 = *(Data **)(param_1 + 0x40);
    pDVar6 = pDVar3 + ((longlong)*(int *)(pDVar3 + 0xc) + 2) * 8;
    iVar1 = *(int *)(pDVar3 + 8);
    while (pDVar3 + ((longlong)iVar1 + 2) * 8 != pDVar6) {
      pDVar6 = pDVar6 + -8;
      pQVar4 = *(QVariant **)pDVar6;
      if (pQVar4 != (QVariant *)0x0) {
        QVariant::~QVariant(pQVar4);
        FUN_140008206(pQVar4,0x10);
      }
    }
    QListData::dispose(pDVar3);
  }
  else if (*piVar2 != -1) {
    LOCK();
    iVar1 = *piVar2;
    *piVar2 = *piVar2 + -1;
    UNLOCK();
    if (iVar1 == 1) goto LAB_140359e7c;
  }
  piVar2 = *(int **)(param_1 + 0x38);
  if (*piVar2 == 0) {
LAB_140359eee:
    pDVar3 = *(Data **)(param_1 + 0x38);
    pDVar6 = pDVar3 + ((longlong)*(int *)(pDVar3 + 0xc) + 2) * 8;
    iVar1 = *(int *)(pDVar3 + 8);
    while (pDVar3 + ((longlong)iVar1 + 2) * 8 != pDVar6) {
      pDVar6 = pDVar6 + -8;
      pQVar4 = *(QVariant **)pDVar6;
      if (pQVar4 != (QVariant *)0x0) {
        QVariant::~QVariant(pQVar4);
        FUN_140008206(pQVar4,0x10);
      }
    }
    QListData::dispose(pDVar3);
  }
  else if (*piVar2 != -1) {
    LOCK();
    iVar1 = *piVar2;
    *piVar2 = *piVar2 + -1;
    UNLOCK();
    if (iVar1 == 1) goto LAB_140359eee;
  }
  QString::~QString((QString *)(param_1 + 0x28));
  QString::~QString((QString *)(param_1 + 0x20));
  QString::~QString((QString *)(param_1 + 0x18));
  iVar1 = *(int *)(*(longlong *)(param_1 + 0x10) + 0x10);
  if (iVar1 != 0) {
    if (iVar1 == -1) goto LAB_140359f92;
    LOCK();
    piVar2 = (int *)(*(longlong *)(param_1 + 0x10) + 0x10);
    iVar1 = *piVar2;
    *piVar2 = *piVar2 + -1;
    UNLOCK();
    if (iVar1 != 1) goto LAB_140359f92;
  }
  QHashData::free_helper(*(QHashData **)(param_1 + 0x10),(_func_void_Node_ptr *)&LAB_1400123f5);
LAB_140359f92:
                    /* WARNING: Could not recover jumptable at 0x000140359fad. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  QObject::~QObject(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1403602f0
// -----------------------------------------------------------------------------
QObject * FUN_1403602f0(QObject *param_1,undefined8 param_2,QString *param_3)

{
  QObject::QObject(param_1,(QObject *)0x0);
  *(undefined ***)param_1 = viewer::organization::engine::Department::vftable;
  *(code **)(param_1 + 0x10) = shared_null_exref;
  QString::QString((QString *)(param_1 + 0x18));
  param_1[0x20] = (QObject)0x0;
  *(undefined8 *)(param_1 + 0x28) = param_2;
  QString::QString((QString *)(param_1 + 0x30),param_3);
  if (*(longlong *)(param_1 + 0x28) == 0) {
    qt_assert("_index",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\department_55.cpp"
              ,0x22);
  }
  return param_1;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x140366d80
// -----------------------------------------------------------------------------
QObject * FUN_140366d80(QObject *param_1,undefined8 param_2,QString *param_3)

{
  QObject::QObject(param_1,(QObject *)0x0);
  *(undefined ***)param_1 = viewer::organization::engine::SessionMarker::vftable;
  *(code **)(param_1 + 0x10) = shared_null_exref;
  QString::QString((QString *)(param_1 + 0x18));
  QString::QString((QString *)(param_1 + 0x20));
  QString::QString((QString *)(param_1 + 0x28));
  param_1[0x30] = (QObject)0x0;
  QString::QString((QString *)(param_1 + 0x38));
  *(undefined8 *)(param_1 + 0x40) = param_2;
  QString::QString((QString *)(param_1 + 0x48),param_3);
  if (*(longlong *)(param_1 + 0x40) == 0) {
    qt_assert("_index",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-organization\\engine\\sessionMarker_55.cpp"
              ,0x24);
  }
  return param_1;
}

// =============================================================================
// VIOLATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x14036cd30
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

QString * FUN_14036cd30(longlong param_1,QString *param_2,longlong *param_3)

{
  bool bVar1;
  char cVar2;
  QUuid *this;
  QString *pQVar3;
  QMapNodeBase *this_00;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined1 auStack_88 [32];
  undefined4 local_68;
  QString local_60 [8];
  longlong *local_58;
  undefined8 local_50;
  QString *local_48;
  ulonglong local_30;
  
  local_50 = 0xfffffffffffffffe;
  local_30 = DAT_140e88880 ^ (ulonglong)auStack_88;
  local_68 = 0;
  local_58 = param_3;
  local_48 = param_2;
  if (param_3 == (longlong *)0x0) {
    qt_assert("nullptr != selector",
              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-violations\\engine\\manager.cpp"
              ,0x2e);
  }
  QString::QString(param_2);
  local_68 = 1;
  while( true ) {
    this = (QUuid *)QUuid::createUuid();
    pQVar3 = (QString *)QUuid::toString(this);
    QString::operator=(param_2,pQVar3);
    QString::~QString(local_60);
    lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x18) + 0x10);
    if (lVar5 == 0) break;
    lVar7 = 0;
    do {
      bVar1 = operator<((QString *)(lVar5 + 0x18),param_2);
      if (bVar1) {
        lVar6 = *(longlong *)(lVar5 + 0x10);
      }
      else {
        lVar6 = *(longlong *)(lVar5 + 8);
        lVar7 = lVar5;
      }
      lVar5 = lVar6;
    } while (lVar6 != 0);
    if ((lVar7 == 0) || (bVar1 = operator<(param_2,(QString *)(lVar7 + 0x18)), bVar1)) break;
  }
  thunk_FUN_14036ba00(param_1 + 0x18,local_60,param_2,&local_58);
  lVar5 = *(longlong *)(param_1 + 0x10);
  if (*(longlong *)(lVar5 + 0x10) == 0) {
    this_00 = (QMapNodeBase *)(lVar5 + 8);
  }
  else {
    this_00 = *(QMapNodeBase **)(lVar5 + 0x20);
  }
  do {
    if (this_00 == (QMapNodeBase *)(lVar5 + 8)) {
      uVar4 = 0;
LAB_14036ce76:
      (**(code **)(*param_3 + 8))(param_3,uVar4);
      return param_2;
    }
    cVar2 = thunk_FUN_1403780a0(*(undefined8 *)(this_00 + 0x20));
    if (cVar2 != '\0') {
      uVar4 = 1;
      goto LAB_14036ce76;
    }
    this_00 = QMapNodeBase::nextNode(this_00);
  } while( true );
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1403780b0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1403780b0(longlong param_1)

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
             thunk_FUN_14006fb70(&local_138,"violations connection closed for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1a0));
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar3 = thunk_FUN_14039be80(local_150);
      local_168 = 0x51;
      thunk_FUN_14039b070(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-violations\\engine\\loader.cpp"
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
    thunk_FUN_14037b010(param_1);
  }
  thunk_FUN_140190b30(param_1);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1403783b0
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_1403783b0(longlong param_1)

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
  thunk_FUN_140190bc0();
  if (*(char *)(param_1 + 0x1a8) == '\0') {
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
             thunk_FUN_14006fb70(&local_138,"violations connection opened for node #");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<
                (this,*(__int64 *)(param_1 + 0x1a0));
      uVar2 = thunk_FUN_1400876c0(&local_138,local_48);
      uVar3 = thunk_FUN_14039be80(local_150);
      local_168 = 0x47;
      thunk_FUN_14039b070(uVar3,20000,uVar2,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-violations\\engine\\loader.cpp"
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
    *(undefined1 *)(param_1 + 0x1a8) = 1;
    thunk_FUN_14037b010(param_1);
  }
  return;
}

// =============================================================================
// FERESTRE SI TRANSFER FISIERE
// =============================================================================

// -----------------------------------------------------------------------------
// Ghidra @ 0x140387f20
// -----------------------------------------------------------------------------
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_140387f20(QQuickWindow *param_1,QQmlComponent *param_2)

{
  char cVar1;
  QQmlContext *pQVar2;
  QObject *pQVar3;
  QObject *this;
  QVariant *pQVar4;
  QQuickItem *pQVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  char *pcVar8;
  ulonglong uVar9;
  undefined1 auStackY_198 [32];
  QQuickWindow *local_168;
  QVariant local_160 [16];
  undefined8 local_150;
  undefined *local_148;
  basic_streambuf<char,std::char_traits<char>_> local_140 [128];
  basic_ios<char,std::char_traits<char>_> local_c0 [104];
  ulonglong local_58 [3];
  ulonglong local_40;
  ulonglong local_38;
  
  if (param_2 != (QQmlComponent *)0x0) {
    local_150 = 0xfffffffffffffffe;
    local_38 = DAT_140e88880 ^ (ulonglong)auStackY_198;
    local_168 = (QQuickWindow *)((ulonglong)local_168 & 0xffffffff00000000);
    if (*(longlong *)(param_1 + 0x80) == 0) {
      pQVar2 = QQmlComponent::creationContext(param_2);
      pQVar3 = (QObject *)(**(code **)(*(longlong *)param_2 + 0x60))(param_2,pQVar2);
      this = QMetaObject::cast((QMetaObject *)staticMetaObject_exref,pQVar3);
      if (this == (QObject *)0x0) {
        uVar6 = thunk_FUN_14039be80(local_160);
        cVar1 = thunk_FUN_14039ba40(uVar6,40000);
        thunk_FUN_14039a8a0(local_160);
        if (cVar1 != '\0') {
          local_148 = &DAT_1405a3878;
          std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                    (local_c0);
          local_168 = (QQuickWindow *)CONCAT44(local_168._4_4_,1);
          std::basic_ostream<char,std::char_traits<char>_>::
          basic_ostream<char,std::char_traits<char>_>
                    ((basic_ostream<char,std::char_traits<char>_> *)&local_148,local_140,false);
          *(undefined ***)(local_140 + (longlong)*(int *)(local_148 + 4) + -8) =
               std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
          *(int *)((longlong)&local_150 + (longlong)*(int *)(local_148 + 4) + 4) =
               *(int *)(local_148 + 4) + -0x88;
          thunk_FUN_140076630(local_140,2);
          thunk_FUN_14006fb70(&local_148,"unable to create window content item from delegate");
          uVar6 = thunk_FUN_1400876c0(&local_148,local_58);
          uVar7 = thunk_FUN_14039be80(local_160);
          thunk_FUN_14039b070(uVar7,40000,uVar6,
                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-windows\\window.cpp"
                             );
          thunk_FUN_14039a8a0(local_160);
          if (0xf < local_40) {
            uVar9 = local_58[0];
            if (0xfff < local_40 + 1) {
              if ((local_58[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              uVar9 = *(ulonglong *)(local_58[0] - 8);
              if (local_58[0] <= uVar9) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (local_58[0] - uVar9 < 8) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
              if (0x27 < local_58[0] - uVar9) {
                    /* WARNING: Subroutine does not return */
                _invalid_parameter_noinfo_noreturn();
              }
            }
            FUN_140002d6f(uVar9);
          }
          thunk_FUN_140077af0(local_c0);
          std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                    (local_c0);
        }
        if (pQVar3 != (QObject *)0x0) {
          QObject::deleteLater(pQVar3);
        }
      }
      else {
        local_168 = param_1;
        pQVar4 = (QVariant *)thunk_FUN_1403874d0(local_160,&local_168);
        QObject::setProperty(this,"window",pQVar4);
        QVariant::~QVariant(local_160);
        pQVar5 = QQuickWindow::contentItem(param_1);
        QQuickItem::setParentItem((QQuickItem *)this,pQVar5);
      }
      (**(code **)(*(longlong *)param_2 + 0x68))(param_2);
      *(QObject **)(param_1 + 0x80) = this;
      thunk_FUN_1403869c0(param_1);
      pcVar8 = qFlagLocation("1onContentMinimumWidthChanged()");
      pQVar3 = (QObject *)qFlagLocation("2minimumWidthChanged()");
      QObject::connect((QObject *)&local_168,(char *)this,pQVar3,(char *)param_1,
                       (ConnectionType)pcVar8);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      pcVar8 = qFlagLocation("1onContentMinimumHeightChanged()");
      pQVar3 = (QObject *)qFlagLocation("2minimumHeightChanged()");
      QObject::connect((QObject *)&local_168,(char *)this,pQVar3,(char *)param_1,
                       (ConnectionType)pcVar8);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      pcVar8 = qFlagLocation("1onContentWishfulWidthChanged()");
      pQVar3 = (QObject *)qFlagLocation("2wishfulWidthChanged()");
      QObject::connect((QObject *)&local_168,(char *)this,pQVar3,(char *)param_1,
                       (ConnectionType)pcVar8);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      pcVar8 = qFlagLocation("1onContentWishfulHeightChanged()");
      pQVar3 = (QObject *)qFlagLocation("2wishfulHeightChanged()");
      QObject::connect((QObject *)&local_168,(char *)this,pQVar3,(char *)param_1,
                       (ConnectionType)pcVar8);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      pcVar8 = qFlagLocation("1onContentWishfulXChanged()");
      pQVar3 = (QObject *)qFlagLocation("2wishfulXChanged()");
      QObject::connect((QObject *)&local_168,(char *)this,pQVar3,(char *)param_1,
                       (ConnectionType)pcVar8);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      pcVar8 = qFlagLocation("1onContentWishfulYChanged()");
      pQVar3 = (QObject *)qFlagLocation("2wishfulYChanged()");
      QObject::connect((QObject *)&local_168,(char *)this,pQVar3,(char *)param_1,
                       (ConnectionType)pcVar8);
      QMetaObject::Connection::~Connection((Connection *)&local_168);
      thunk_FUN_14038a520(param_1);
      thunk_FUN_14038a460(param_1);
      thunk_FUN_14038a5e0(param_1);
      thunk_FUN_14038a690(param_1);
      thunk_FUN_14038a390(param_1);
      thunk_FUN_14038a2c0(param_1);
    }
  }
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x14038ea80
// -----------------------------------------------------------------------------
void FUN_14038ea80(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  void *pvVar2;
  QObject *this;
  Data *local_30;
  void **local_28;
  void **local_20;
  undefined4 local_18;
  
  thunk_FUN_14038d320(&local_30,param_1 + 0x70,param_3,param_4,0xfffffffffffffffe);
  local_28 = QListData::begin((QListData *)&local_30);
  local_20 = QListData::end((QListData *)&local_30);
  for (; local_18 = 1, local_28 != local_20; local_28 = local_28 + 1) {
    pvVar2 = *local_28;
    thunk_FUN_1403884d0(pvVar2);
    this = (QObject *)thunk_FUN_1403888d0(pvVar2);
    if (this == (QObject *)0x0) {
      qt_assert("windowConfig",
                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-windows\\manager.cpp",0xb5
               );
    }
    thunk_FUN_14038bf50(pvVar2);
    thunk_FUN_14038c320(pvVar2);
    QObject::deleteLater(this);
  }
  local_18 = 1;
  if (*(int *)local_30 != 0) {
    if (*(int *)local_30 == -1) {
      return;
    }
    LOCK();
    iVar1 = *(int *)local_30;
    *(int *)local_30 = *(int *)local_30 + -1;
    UNLOCK();
    if (iVar1 != 1) {
      return;
    }
  }
  QListData::dispose(local_30);
  return;
}

// -----------------------------------------------------------------------------
// Ghidra @ 0x1404ea190
// -----------------------------------------------------------------------------
/* WARNING: Function: _alloca_probe replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

uint FUN_1404ea190(undefined ***param_1,HANDLE param_2,int param_3,int param_4,void *param_5,
                  size_t param_6)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  DWORD DVar4;
  BOOL BVar5;
  DWORD DVar6;
  QString *pQVar7;
  undefined8 uVar8;
  basic_ostream<char,std::char_traits<char>_> *pbVar9;
  undefined8 uVar10;
  char *pcVar11;
  QByteArray *pQVar12;
  QFileInfo *pQVar13;
  undefined8 *dwItem1;
  basic_ostream<char,struct_std::char_traits<char>_> *pbVar14;
  ulonglong uVar15;
  longlong lVar16;
  uint uVar17;
  uint uVar18;
  undefined1 auStackY_2348 [32];
  QString local_22d8 [16];
  undefined8 local_22c8;
  undefined8 uStack_22c0;
  QString *local_22b8;
  char *pcStack_22b0;
  DWORD local_22a8 [4];
  undefined8 local_2298;
  undefined8 uStack_2290;
  longlong local_2288;
  QString local_2280 [8];
  undefined8 local_2278;
  undefined8 uStack_2270;
  QString local_2268 [8];
  undefined ***local_2260;
  QFile local_2258 [12];
  int iStack_224c;
  undefined *local_2248;
  undefined **local_2240;
  basic_ostream<char,std::char_traits<char>_> local_2238 [96];
  undefined8 local_21d8;
  undefined4 local_21d0;
  basic_ios<char,std::char_traits<char>_> local_21c0 [104];
  QDir local_2158 [8];
  _OVERLAPPED local_2150;
  HANDLE local_2130;
  HANDLE local_2128;
  undefined8 local_2120;
  undefined8 local_2118;
  undefined8 uStack_2110;
  undefined8 local_2108;
  undefined8 uStack_2100;
  undefined8 local_20f8;
  undefined8 uStack_20f0;
  undefined8 local_20e8;
  undefined8 uStack_20e0;
  undefined8 local_20d8;
  undefined8 uStack_20d0;
  ulonglong local_20c8 [7];
  ulonglong local_2090;
  undefined1 local_2084 [8188];
  ulonglong local_88;
  undefined8 local_48;
  
  local_48 = 0x1404ea1af;
  local_2120 = 0xfffffffffffffffe;
  local_88 = DAT_140e88880 ^ (ulonglong)auStackY_2348;
  uVar17 = 0;
  local_22c8 = local_22c8 & 0xffffffff00000000;
  local_2260 = param_1;
  QString::fromWCharArray((wchar_t *)local_2268,param_4);
  pQVar7 = (QString *)QString::fromWCharArray((wchar_t *)&local_2278,param_3);
  QDir::QDir(local_2158,pQVar7);
  QString::~QString((QString *)&local_2278);
  local_2150.Internal = 0;
  local_2150.InternalHigh = 0;
  local_2150.u.Pointer = (PVOID)0x0;
  local_2150.hEvent = (HANDLE)0x0;
  local_2150.hEvent = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
  if ((longlong)local_2150.hEvent - 1U < 0xfffffffffffffffe) {
    local_2130 = param_2;
    local_2128 = local_2150.hEvent;
    ResetEvent(local_2150.hEvent);
    iVar3 = ReadDirectoryChangesW
                      (param_2,local_2084,0x1ffc,1,0x10,(LPDWORD)0x0,&local_2150,
                       (LPOVERLAPPED_COMPLETION_ROUTINE)0x0);
    uVar18 = uVar17;
    while (iVar3 != 0) {
      DVar4 = WaitForMultipleObjects(2,&local_2130,0,0xffffffff);
      if (DVar4 != 0x102) {
        if (DVar4 == 0) goto LAB_1404eb091;
        if (DVar4 != 1) {
          if (DVar4 == 0xffffffff) {
            DVar4 = GetLastError();
            if (DVar4 == 6) goto LAB_1404eb091;
            uVar8 = thunk_FUN_14039be80(local_20c8);
            cVar1 = thunk_FUN_14039ba40(uVar8,30000);
            thunk_FUN_14039a8a0(local_20c8);
            if (cVar1 == '\0') goto LAB_1404eb091;
            local_2248 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_21c0);
            local_22c8 = CONCAT44(local_22c8._4_4_,uVar18) | 0x20;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,false);
            *(undefined ***)
             ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4))
                 = std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
            *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
                 *(int *)(local_2248 + 4) + -0x88;
            thunk_FUN_140076630(&local_2240,2);
            DVar4 = GetLastError();
            pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                         "waiter failed: ");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar4);
            uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                        local_20c8 + 4);
            uVar10 = thunk_FUN_14039be80(local_20c8);
            thunk_FUN_14039b070(uVar10,30000,uVar8,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control-utils\\pasteUrlsHandler\\implWin.cpp"
                               );
            thunk_FUN_14039a8a0(local_20c8);
            if (0xf < local_2090) {
              uVar15 = local_20c8[4];
              if (0xfff < local_2090 + 1) {
                if ((local_20c8[4] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar15 = *(ulonglong *)(local_20c8[4] - 8);
                if (local_20c8[4] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_20c8[4] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_20c8[4] - uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(uVar15);
            }
            thunk_FUN_140077af0(local_21c0);
          }
          else {
            uVar8 = thunk_FUN_14039be80(local_20c8);
            cVar1 = thunk_FUN_14039ba40(uVar8,30000);
            thunk_FUN_14039a8a0(local_20c8);
            if (cVar1 == '\0') goto LAB_1404eb091;
            local_2248 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_21c0);
            local_22c8 = CONCAT44(local_22c8._4_4_,uVar18) | 0x40;
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,false);
            *(undefined ***)
             ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4))
                 = std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
            *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
                 *(int *)(local_2248 + 4) + -0x88;
            thunk_FUN_140076630(&local_2240,2);
            DVar6 = GetLastError();
            pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                         "WaitForMultipleObjects: ");
            pbVar14 = std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar4);
            pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70(pbVar14,&DAT_1405a3ec4);
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar6);
            uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                        local_20c8 + 4);
            uVar10 = thunk_FUN_14039be80(local_20c8);
            thunk_FUN_14039b070(uVar10,30000,uVar8,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control-utils\\pasteUrlsHandler\\implWin.cpp"
                               );
            thunk_FUN_14039a8a0(local_20c8);
            if (0xf < local_2090) {
              uVar15 = local_20c8[4];
              if (0xfff < local_2090 + 1) {
                if ((local_20c8[4] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar15 = *(ulonglong *)(local_20c8[4] - 8);
                if (local_20c8[4] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_20c8[4] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_20c8[4] - uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(uVar15);
            }
            thunk_FUN_140077af0(local_21c0);
          }
          goto LAB_1404eb084;
        }
        local_22a8[0] = 0;
        BVar5 = GetOverlappedResult(param_2,&local_2150,local_22a8,0);
        if (BVar5 == 0) {
          uVar8 = thunk_FUN_14039be80(&local_22b8);
          cVar1 = thunk_FUN_14039ba40(uVar8,40000);
          thunk_FUN_14039a8a0(&local_22b8);
          if (cVar1 != '\0') {
            local_2248 = &DAT_1405a3878;
            std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                      (local_21c0);
            uVar18 = 4;
            local_22c8 = CONCAT44(local_22c8._4_4_,4);
            std::basic_ostream<char,std::char_traits<char>_>::
            basic_ostream<char,std::char_traits<char>_>
                      ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                       (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,false);
            *(undefined ***)
             ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4))
                 = std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::
                   vftable;
            *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
                 *(int *)(local_2248 + 4) + -0x88;
            thunk_FUN_140076630(&local_2240,2);
            DVar4 = GetLastError();
            pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                     thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                         "GetOverlappedResult: ");
            std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar4);
            uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                        local_20c8);
            uVar10 = thunk_FUN_14039be80(&local_2298);
            thunk_FUN_14039b070(uVar10,40000,uVar8,
                                "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control-utils\\pasteUrlsHandler\\implWin.cpp"
                               );
            thunk_FUN_14039a8a0(&local_2298);
            if (0xf < local_20c8[3]) {
              uVar15 = local_20c8[0];
              if (0xfff < local_20c8[3] + 1) {
                if ((local_20c8[0] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                uVar15 = *(ulonglong *)(local_20c8[0] - 8);
                if (local_20c8[0] <= uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (local_20c8[0] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
                if (0x27 < local_20c8[0] - uVar15) {
                    /* WARNING: Subroutine does not return */
                  _invalid_parameter_noinfo_noreturn();
                }
              }
              FUN_140002d6f(uVar15);
            }
            thunk_FUN_140077af0(local_21c0);
            std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                      (local_21c0);
          }
        }
        else {
          CancelIo(param_2);
          if (local_22a8[0] != 0) {
            QString::fromWCharArray((wchar_t *)local_2280,(int)local_2084 + 0xc);
            bVar2 = QString::endsWith(local_2280,local_2268,1);
            if (bVar2) {
              QDir::absoluteFilePath(local_2158,local_22d8);
              lVar16 = 4;
              do {
                QFile::QFile(local_2258,local_22d8);
                bVar2 = QFile::open(local_2258,1);
                if (bVar2) {
                  QIODevice::read((QIODevice *)local_2258,(__int64)&local_2288);
                  QFileDevice::close((QFileDevice *)local_2258);
                  if (*(int *)(local_2288 + 4) == (int)param_6) {
                    pcVar11 = QByteArray::constData((QByteArray *)&local_2288);
                    iVar3 = memcmp(pcVar11,param_5,param_6);
                    if (iVar3 == 0) {
                      CloseHandle(local_2150.hEvent);
                      bVar2 = QFile::setPermissions(local_22d8,0x2222);
                      if (!bVar2) {
                        uVar8 = thunk_FUN_14039be80(&local_2298);
                        cVar1 = thunk_FUN_14039ba40(uVar8,30000);
                        thunk_FUN_14039a8a0(&local_2298);
                        if (cVar1 != '\0') {
                          local_2248 = &DAT_1405a3878;
                          std::basic_ios<char,std::char_traits<char>_>::
                          basic_ios<char,std::char_traits<char>_>(local_21c0);
                          local_22c8 = CONCAT44(local_22c8._4_4_,uVar18) | 8;
                          std::basic_ostream<char,std::char_traits<char>_>::
                          basic_ostream<char,std::char_traits<char>_>
                                    ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                     (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,
                                     false);
                          *(undefined ***)
                           ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 +
                           *(int *)(local_2248 + 4)) =
                               std::
                               basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>
                               ::vftable;
                          *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
                               *(int *)(local_2248 + 4) + -0x88;
                          thunk_FUN_140076630(&local_2240,2);
                          uVar8 = thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *
                                                      )&local_2248,
                                                      "Unable to set permission victim file: ");
                          pQVar12 = (QByteArray *)QString::toLocal8Bit(local_22d8);
                          pcVar11 = QByteArray::data(pQVar12);
                          thunk_FUN_14006fb70(uVar8,pcVar11);
                          QByteArray::~QByteArray((QByteArray *)&local_22c8);
                          thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)
                                              &local_2248,local_20c8);
                          uVar8 = thunk_FUN_14039be80(&local_2298);
                          thunk_FUN_14039b070(uVar8,30000);
                          thunk_FUN_14039a8a0(&local_2298);
                          thunk_FUN_140077b90(local_20c8);
                          thunk_FUN_140077af0(local_21c0);
                          std::basic_ios<char,std::char_traits<char>_>::
                          ~basic_ios<char,std::char_traits<char>_>(local_21c0);
                          uVar18 = uVar18 | 8;
                        }
                      }
                      pQVar13 = (QFileInfo *)
                                QFileInfo::QFileInfo((QFileInfo *)&local_22c8,local_22d8);
                      QFileInfo::path(pQVar13);
                      QFileInfo::~QFileInfo((QFileInfo *)&local_22c8);
                      bVar2 = QFile::remove(local_22d8);
                      if (bVar2) {
                        pQVar7 = (QString *)QDir::toNativeSeparators((QString *)&local_22b8);
                        dwItem1 = (undefined8 *)QString::toStdWString(pQVar7);
                        if (7 < (ulonglong)dwItem1[3]) {
                          dwItem1 = (undefined8 *)*dwItem1;
                        }
                        SHChangeNotify(0x1000,5,dwItem1,(LPCVOID)0x0);
                        if (7 < local_20c8[3]) {
                          thunk_FUN_1402a7f50(local_20c8,local_20c8[0],local_20c8[3] + 1);
                        }
                        local_20c8[3] = 7;
                        local_20c8[2] = 0;
                        local_20c8[0] = local_20c8[0] & 0xffffffffffff0000;
                        QString::~QString((QString *)&local_22b8);
                      }
                      else {
                        uVar8 = thunk_FUN_14039be80(&local_2298);
                        cVar1 = thunk_FUN_14039ba40(uVar8,30000);
                        thunk_FUN_14039a8a0(&local_2298);
                        if (cVar1 != '\0') {
                          local_2248 = &DAT_1405a3878;
                          std::basic_ios<char,std::char_traits<char>_>::
                          basic_ios<char,std::char_traits<char>_>(local_21c0);
                          local_22c8 = CONCAT44(local_22c8._4_4_,uVar18) | 0x10;
                          std::basic_ostream<char,std::char_traits<char>_>::
                          basic_ostream<char,std::char_traits<char>_>
                                    ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                     (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,
                                     false);
                          *(undefined ***)
                           ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 +
                           *(int *)(local_2248 + 4)) =
                               std::
                               basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>
                               ::vftable;
                          *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
                               *(int *)(local_2248 + 4) + -0x88;
                          thunk_FUN_140076630(&local_2240,2);
                          uVar8 = thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *
                                                      )&local_2248,"unable to delete victim file: ")
                          ;
                          pQVar12 = (QByteArray *)QString::toLocal8Bit(local_22d8);
                          pcVar11 = QByteArray::data(pQVar12);
                          thunk_FUN_14006fb70(uVar8,pcVar11);
                          QByteArray::~QByteArray((QByteArray *)&local_22c8);
                          uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *
                                                      )&local_2248,local_20c8);
                          uVar10 = thunk_FUN_14039be80(&local_2298);
                          thunk_FUN_14039b070(uVar10,30000,uVar8,
                                              "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control-utils\\pasteUrlsHandler\\implWin.cpp"
                                             );
                          thunk_FUN_14039a8a0(&local_2298);
                          thunk_FUN_140077b90(local_20c8);
                          thunk_FUN_140077af0(local_21c0);
                          std::basic_ios<char,std::char_traits<char>_>::
                          ~basic_ios<char,std::char_traits<char>_>(local_21c0);
                        }
                      }
                      QString::~QString((QString *)&local_2278);
                      pQVar7 = (QString *)QString::left(local_22d8,(int)&local_22b8);
                      QString::operator=(local_22d8,pQVar7);
                      QString::~QString((QString *)&local_22b8);
                      pQVar13 = (QFileInfo *)
                                QFileInfo::QFileInfo((QFileInfo *)&local_2278,local_22d8);
                      pQVar7 = (QString *)QFileInfo::canonicalFilePath(pQVar13);
                      QString::operator=(local_22d8,pQVar7);
                      QString::~QString((QString *)&local_22b8);
                      QFileInfo::~QFileInfo((QFileInfo *)&local_2278);
                      local_22b8 = local_22d8;
                      local_2298 = 0;
                      uStack_2290 = 0;
                      local_2278 = 0;
                      uStack_2270 = 0;
                      local_22c8 = 0;
                      uStack_22c0 = 0;
                      local_2118 = 0;
                      uStack_2110 = 0;
                      local_2108 = 0;
                      uStack_2100 = 0;
                      local_20f8 = 0;
                      uStack_20f0 = 0;
                      local_20e8 = 0;
                      uStack_20e0 = 0;
                      local_20d8 = 0;
                      uStack_20d0 = 0;
                      local_20c8[4] = 0;
                      local_20c8[5] = 0;
                      pcStack_22b0 = "QString";
                      local_20c8[0] = 0;
                      local_20c8[1] = 0;
                      QMetaObject::invokeMethod(local_2260,"onFoundAt",2,local_20c8);
                      QByteArray::~QByteArray((QByteArray *)&local_2288);
                      QFile::~QFile(local_2258);
                      QString::~QString(local_22d8);
                      QString::~QString(local_2280);
                      goto LAB_1404eb279;
                    }
                  }
                  QByteArray::~QByteArray((QByteArray *)&local_2288);
                  QFile::~QFile(local_2258);
                  break;
                }
                DVar4 = GetLastError();
                if (DVar4 == 0x20) {
                  Sleep(1);
                }
                QFile::~QFile(local_2258);
                lVar16 = lVar16 + -1;
              } while (lVar16 != 0);
              QString::~QString(local_22d8);
            }
            QString::~QString(local_2280);
          }
        }
      }
      ResetEvent(local_2150.hEvent);
      iVar3 = ReadDirectoryChangesW
                        (param_2,local_2084,0x1ffc,1,0x10,(LPDWORD)0x0,&local_2150,
                         (LPOVERLAPPED_COMPLETION_ROUTINE)0x0);
    }
    DVar4 = GetLastError();
    if (DVar4 != 6) {
      uVar8 = thunk_FUN_14039be80(local_20c8);
      cVar1 = thunk_FUN_14039ba40(uVar8,30000);
      thunk_FUN_14039a8a0(local_20c8);
      if (cVar1 != '\0') {
        local_2248 = &DAT_1405a3878;
        std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                  (local_21c0);
        local_22c8 = CONCAT44(local_22c8._4_4_,uVar18) | 2;
        std::basic_ostream<char,std::char_traits<char>_>::
        basic_ostream<char,std::char_traits<char>_>
                  ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                   (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,false);
        *(undefined ***)
         ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
             *(int *)(local_2248 + 4) + -0x88;
        local_2260 = &local_2240;
        std::basic_streambuf<char,std::char_traits<char>_>::
        basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2240);
        local_2240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
        local_21d8 = 0;
        local_21d0 = 4;
        DVar4 = GetLastError();
        pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
                 thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                     "ReadDirectoryChangesW: ");
        std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar4);
        uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                    local_20c8 + 4);
        uVar10 = thunk_FUN_14039be80(local_20c8);
        thunk_FUN_14039b070(uVar10,30000,uVar8,
                            "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control-utils\\pasteUrlsHandler\\implWin.cpp"
                           );
        thunk_FUN_14039a8a0(local_20c8);
        if (0xf < local_2090) {
          uVar15 = local_20c8[4];
          if (0xfff < local_2090 + 1) {
            if ((local_20c8[4] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            uVar15 = *(ulonglong *)(local_20c8[4] - 8);
            if (local_20c8[4] <= uVar15) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (local_20c8[4] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
            if (0x27 < local_20c8[4] - uVar15) {
                    /* WARNING: Subroutine does not return */
              _invalid_parameter_noinfo_noreturn();
            }
          }
          FUN_140002d6f(uVar15);
        }
        *(undefined ***)
         ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4)) =
             std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
        *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
             *(int *)(local_2248 + 4) + -0x88;
        local_2240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::
                     vftable;
        thunk_FUN_14007bb20(&local_2240);
        std::basic_streambuf<char,std::char_traits<char>_>::
        ~basic_streambuf<char,std::char_traits<char>_>
                  ((basic_streambuf<char,std::char_traits<char>_> *)&local_2240);
        std::basic_ostream<char,std::char_traits<char>_>::
        ~basic_ostream<char,std::char_traits<char>_>(local_2238);
LAB_1404eb084:
        std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                  (local_21c0);
      }
    }
LAB_1404eb091:
    CloseHandle(local_2150.hEvent);
  }
  else {
    uVar8 = thunk_FUN_14039be80(local_20c8);
    cVar1 = thunk_FUN_14039ba40(uVar8,40000);
    thunk_FUN_14039a8a0(local_20c8);
    if (cVar1 != '\0') {
      local_2248 = &DAT_1405a3878;
      std::basic_ios<char,std::char_traits<char>_>::basic_ios<char,std::char_traits<char>_>
                (local_21c0);
      local_22c8 = CONCAT44(local_22c8._4_4_,1);
      std::basic_ostream<char,std::char_traits<char>_>::basic_ostream<char,std::char_traits<char>_>
                ((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                 (basic_streambuf<char,std::char_traits<char>_> *)&local_2240,false);
      *(undefined ***)
       ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
           *(int *)(local_2248 + 4) + -0x88;
      local_2260 = &local_2240;
      std::basic_streambuf<char,std::char_traits<char>_>::
      basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_2240);
      local_2240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      local_21d8 = 0;
      local_21d0 = 4;
      DVar4 = GetLastError();
      pbVar9 = (basic_ostream<char,std::char_traits<char>_> *)
               thunk_FUN_14006fb70((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                   "CreateEventW: ");
      std::basic_ostream<char,std::char_traits<char>_>::operator<<(pbVar9,DVar4);
      uVar8 = thunk_FUN_1400876c0((basic_ostream<char,std::char_traits<char>_> *)&local_2248,
                                  local_20c8 + 4);
      uVar10 = thunk_FUN_14039be80(local_20c8);
      thunk_FUN_14039b070(uVar10,40000,uVar8,
                          "R:\\bbuild-7\\KICK-EVPROD-WIN64\\components\\impl-viewer-control-utils\\pasteUrlsHandler\\implWin.cpp"
                         );
      thunk_FUN_14039a8a0(local_20c8);
      if (0xf < local_2090) {
        uVar15 = local_20c8[4];
        if (0xfff < local_2090 + 1) {
          if ((local_20c8[4] & 0x1f) != 0) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          uVar15 = *(ulonglong *)(local_20c8[4] - 8);
          if (local_20c8[4] <= uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (local_20c8[4] - uVar15 < 8) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
          if (0x27 < local_20c8[4] - uVar15) {
                    /* WARNING: Subroutine does not return */
            _invalid_parameter_noinfo_noreturn();
          }
        }
        FUN_140002d6f(uVar15);
      }
      *(undefined ***)
       ((basic_ostream<char,std::char_traits<char>_> *)&local_2248 + *(int *)(local_2248 + 4)) =
           std::basic_ostringstream<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      *(int *)((longlong)&iStack_224c + (longlong)*(int *)(local_2248 + 4)) =
           *(int *)(local_2248 + 4) + -0x88;
      local_2240 = std::basic_stringbuf<char,std::char_traits<char>,std::allocator<char>_>::vftable;
      thunk_FUN_14007bb20(&local_2240);
      std::basic_streambuf<char,std::char_traits<char>_>::
      ~basic_streambuf<char,std::char_traits<char>_>
                ((basic_streambuf<char,std::char_traits<char>_> *)&local_2240);
      std::basic_ostream<char,std::char_traits<char>_>::~basic_ostream<char,std::char_traits<char>_>
                (local_2238);
      std::basic_ios<char,std::char_traits<char>_>::~basic_ios<char,std::char_traits<char>_>
                (local_21c0);
    }
    uVar17 = 0xffffffff;
  }
LAB_1404eb279:
  QDir::~QDir(local_2158);
  QString::~QString(local_2268);
  return uVar17;
}

