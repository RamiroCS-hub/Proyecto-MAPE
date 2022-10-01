from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import smtplib

def emailSender(message,email):
    # date = datetime.now()
    # actualDate = str(date.strftime("%d/%m/%Y %H:%M:%S"))
    msg = MIMEMultipart()

    server = smtplib.SMTP(host = "smtp.gmail.com", port = 587)
    
    server.ehlo()
    msg["Subject"] = "ALARMA ACTIVADA!"
    mesage = "Hora de activacion" + message
    msg.attach(MIMEText(message, "plain"))
    # Saco los espacios dentro del email
    email = email.strip()
    #Inicio la conexión
    server.starttls()
    #logeo el email con el cual mando el main y mando el mail hacia la direccion del usuario
    server.login(user="pythonnexample@gmail.com", password="rjvjltqrzdatisvu")
    try:
        server.sendmail("pythonnexample@gmail.com", 
        email, msg.as_string())
    except:
        print("Ocurrio un error al mandar el mail")    
        server.quit()
        return False
    
    print("Se ha enviado el email con sus datos")
    return True



