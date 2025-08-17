#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include "connectionparameters.h"

using namespace Connection;

ConnectionDialog::ConnectionDialog(ConnectionParameters *parametrs,
                                   QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionDialog)
    , m_parameters(parametrs)
{
    ui->setupUi(this);

    updateUiFromParameters();

    connect(ui->pushButtonSave,
            &QPushButton::clicked,
            this,
            &ConnectionDialog::onSaveClicked);

    connect(ui->pushButtonCancel,
            &QPushButton::clicked,
            this,
            &ConnectionDialog::onCancelClicked);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::updateUiFromParameters()
{
    ui->lineEditIP->setText(m_parameters->url().host());
    ui->comboBoxIPv->setCurrentIndex(m_parameters->protocol());
    ui->lineEditPort->setText(QString::number(m_parameters->url().port()));
    ui->lineEditConnectTimeout->setText(QString::number(m_parameters->connectTimeout()));
    ui->lineEditDelayTimout->setText(QString::number(m_parameters->delayBetweenPolls()));
    ui->lineEditResponseTimeout->setText(QString::number(m_parameters->responseTimeout()));
}

void ConnectionDialog::onSaveClicked()
{
    QUrl url;
    url.setHost(ui->lineEditIP->text());
    url.setPort(ui->lineEditPort->text().toInt());

    if (!url.isValid()) {
        qWarning() << "Invalid URL:" << url.errorString();
    }

    m_parameters->setUrl(url);

    m_parameters->setConnectTimeout(ui->lineEditConnectTimeout->text().toInt());
    m_parameters->setDelayBetweenPolls(ui->lineEditDelayTimout->text().toInt());
    m_parameters->setResponseTimeout(ui->lineEditResponseTimeout->text().toInt());
    m_parameters->setProtocol(protocolMap.value(ui->comboBoxIPv->currentIndex()));

    accept();
}

void ConnectionDialog::onCancelClicked()
{
    updateUiFromParameters();
    reject();
}
