#pragma once

#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QVBoxLayout>

// Layers/SaveDialog.qml fixFileName: lower-case on Windows, and every
// character a file name cannot (or should not) carry replaced by '_'.
QString fixFileName(QString name);

// utils/DatetimePrinter.dateToStr for the English UI.
QString dateToStr(const QDateTime &moment);

// Layers/SaveDialog.qml: the extra content (session, format, ...), the
// directory (remembered per recentDirCookie) and the file name; Ok checks
// the path, cleans the name, adds the extension and asks before
// overwriting (Yes / No = name_1, name_2... / Cancel).
class SaveDialog final : public QDialog
{
public:
    SaveDialog(const QString &title, const QString &extension, const QString &recentDirCookie,
                      const QString &initialName, QWidget *parent)
        : QDialog(parent)
        , m_extension(extension)
        , m_cookie(recentDirCookie)
    {
        setWindowTitle(title);
        setModal(true);
        auto *layout = new QVBoxLayout(this);
        m_form = new QFormLayout;
        layout->addLayout(m_form);
        const QString recent = QSettings().value(QStringLiteral("saveDialog/") + m_cookie).toString();
        m_dirEdit = new QLineEdit(recent.isEmpty() || !QFileInfo(recent).isDir()
                                      ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                                      : recent,
                                  this);
        m_dirEdit->setReadOnly(true);
        auto *browse = new QPushButton(QStringLiteral("..."), this);
        browse->setFixedWidth(32);
        connect(browse, &QPushButton::clicked, this, [this] {
            const QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("Directories Tree:"),
                                                                  m_dirEdit->text());
            if (!dir.isEmpty()) {
                m_dirEdit->setText(QDir::toNativeSeparators(dir));
            }
        });
        auto *dirRow = new QHBoxLayout;
        dirRow->addWidget(m_dirEdit, 1);
        dirRow->addWidget(browse);
        m_form->addRow(QStringLiteral("Current dir:"), dirRow);
        // The File field shows fixedFilename: fixFileName(initialFilename).
        m_fileEdit = new QLineEdit(fixFileName(initialName), this);
        m_fileEdit->setMinimumWidth(360);
        m_form->addRow(QStringLiteral("File:"), m_fileEdit);
        auto *buttons = new QDialogButtonBox(this);
        buttons->addButton(QStringLiteral("Cancel"), QDialogButtonBox::RejectRole);
        buttons->addButton(QStringLiteral("Ok"), QDialogButtonBox::AcceptRole);
        connect(buttons, &QDialogButtonBox::accepted, this, &SaveDialog::tryAccept);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

    // The extra content goes above the directory and the file name.
    void addRow(const QString &label, QWidget *field) { m_form->insertRow(m_extraRows++, label, field); }
    void addRow(const QString &label, const QString &value) { addRow(label, new QLabel(value, this)); }
    void addWidget(QWidget *widget) { m_form->insertRow(m_extraRows++, widget); }
    void setExtension(const QString &extension) { m_extension = extension; }
    // initialFilename is a binding (KeyloggerExport.qml puts the extension in
    // it): a new one shows in the File field until the user has edited it.
    void setInitialName(const QString &initialName)
    {
        if (!m_fileEdit->isModified()) {
            m_fileEdit->setText(fixFileName(initialName));
        }
    }
    QString path() const { return m_path; }

private:
    void tryAccept()
    {
        const QString directory = m_dirEdit->text().trimmed();
        if (directory.isEmpty() || !QFileInfo(directory).isDir()) {
            QMessageBox::warning(this, QStringLiteral("Warning"), QStringLiteral("Path ok file name is not set"));
            return;
        }
        QString file = m_fileEdit->text().trimmed();
        if (file.isEmpty()) {
            file = QStringLiteral("No name");
        }
        file = fixFileName(file);
        if (!file.endsWith(m_extension)) {
            file += QLatin1Char('.') + m_extension;
        }
        QString path = QDir(directory).filePath(file);
        if (QFileInfo::exists(path)) {
            QMessageBox box(QMessageBox::Question, QStringLiteral("Rewrite confirmation"),
                            QStringLiteral("File \"%1\" already exists. Press Yes to rewrite, No to save with different name.")
                                .arg(file),
                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
            const int answer = box.exec();
            if (answer == QMessageBox::Cancel) {
                return;
            }
            if (answer == QMessageBox::No) {
                const QString base = file.left(file.size() - m_extension.size() - 1);
                for (int n = 1;; ++n) {
                    path = QDir(directory).filePath(QStringLiteral("%1_%2.%3").arg(base).arg(n).arg(m_extension));
                    if (!QFileInfo::exists(path)) {
                        break;
                    }
                }
            }
        }
        QSettings().setValue(QStringLiteral("saveDialog/") + m_cookie, directory);
        m_path = path;
        accept();
    }

    QFormLayout *m_form = nullptr;
    int m_extraRows = 0;
    QLineEdit *m_dirEdit = nullptr;
    QLineEdit *m_fileEdit = nullptr;
    QString m_extension;
    QString m_cookie;
    QString m_path;
};

