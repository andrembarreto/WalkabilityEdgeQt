package br.usp.esalq.caminhabilidade.qt;

import android.Manifest;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.os.Build;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import androidx.core.app.ServiceCompat;
import androidx.core.content.ContextCompat;

/*
 * Foreground service do tipo location que mantem o processo em prioridade de
 * primeiro plano enquanto uma jornada esta ativa.
 *
 * O servico nao faz nenhum trabalho proprio: GPS, cronometro e cache continuam
 * no C++ (JourneyTracker), que roda no mesmo processo. Ele so existe para que o
 * Android continue entregando posicoes com o app minimizado, para exibir a
 * notificacao obrigatoria e para segurar um wake lock que impede a CPU de
 * suspender (o Stopwatch usa steady_clock, que para durante a suspensao).
 *
 * Controlado pelo C++ via JNI em src/infra/background-tracking/.
 */
public class JourneyTrackingService extends Service
{
    private static final String TAG = "JourneyTrackingService";
    private static final String CHANNEL_ID = "journey_tracking";
    private static final int NOTIFICATION_ID = 1;
    private static final int NOTIFICATION_PERMISSION_REQUEST = 1001;

    private PowerManager.WakeLock m_wakeLock;

    public static void start(Context context)
    {
        context.startForegroundService(new Intent(context, JourneyTrackingService.class));
    }

    public static void stop(Context context)
    {
        context.stopService(new Intent(context, JourneyTrackingService.class));
    }

    // A partir do Android 13 a notificacao do servico so aparece se o usuario
    // conceder POST_NOTIFICATIONS. Sem ela o servico roda do mesmo jeito.
    public static void requestNotificationPermission(Context context)
    {
        if(Build.VERSION.SDK_INT < Build.VERSION_CODES.TIRAMISU || !(context instanceof Activity))
            return;

        if(ContextCompat.checkSelfPermission(context, Manifest.permission.POST_NOTIFICATIONS)
                == PackageManager.PERMISSION_GRANTED)
            return;

        final Activity activity = (Activity) context;
        activity.runOnUiThread(() -> ActivityCompat.requestPermissions(
            activity,
            new String[] { Manifest.permission.POST_NOTIFICATIONS },
            NOTIFICATION_PERMISSION_REQUEST
        ));
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        createNotificationChannel();

        // No Android 14 startForeground lanca SecurityException se a permissao
        // de localizacao ainda nao foi concedida.
        try {
            ServiceCompat.startForeground(
                this,
                NOTIFICATION_ID,
                buildNotification(),
                ServiceInfo.FOREGROUND_SERVICE_TYPE_LOCATION
            );
        } catch(Exception e) {
            Log.e(TAG, "failed to start foreground service", e);
            stopSelf();
            return START_NOT_STICKY;
        }

        acquireWakeLock();

        // Se o sistema matar o processo o servico nao deve voltar sozinho, sem o
        // Qt. A jornada e retomada do cache no proximo launch do app.
        return START_NOT_STICKY;
    }

    @Override
    public void onDestroy()
    {
        releaseWakeLock();
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }

    private void createNotificationChannel()
    {
        NotificationChannel channel = new NotificationChannel(
            CHANNEL_ID,
            "Jornada em andamento",
            NotificationManager.IMPORTANCE_LOW
        );
        getSystemService(NotificationManager.class).createNotificationChannel(channel);
    }

    private Notification buildNotification()
    {
        // A QtActivity e singleTop, entao tocar na notificacao volta para a
        // instancia que ja esta rodando.
        Intent launchIntent = getPackageManager().getLaunchIntentForPackage(getPackageName());
        PendingIntent contentIntent = PendingIntent.getActivity(
            this, 0, launchIntent, PendingIntent.FLAG_IMMUTABLE
        );

        return new NotificationCompat.Builder(this, CHANNEL_ID)
            .setContentTitle("Jornada em andamento")
            .setContentText("Registrando a rota")
            .setSmallIcon(android.R.drawable.ic_menu_mylocation)
            .setContentIntent(contentIntent)
            .setOngoing(true)
            .setCategory(NotificationCompat.CATEGORY_WORKOUT)
            .build();
    }

    private void acquireWakeLock()
    {
        if(m_wakeLock == null)
        {
            PowerManager powerManager = getSystemService(PowerManager.class);
            m_wakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "Caminhabilidade:journey");
            m_wakeLock.setReferenceCounted(false);
        }

        if(!m_wakeLock.isHeld())
            m_wakeLock.acquire();
    }

    private void releaseWakeLock()
    {
        if(m_wakeLock != null && m_wakeLock.isHeld())
            m_wakeLock.release();
    }
}
