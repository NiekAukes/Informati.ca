package md57f38e162310d0303b0e3b85640668cc7;


public class BTService_mAdvertiseCallback
	extends android.bluetooth.le.AdvertiseCallback
	implements
		mono.android.IGCUserPeer
{
/** @hide */
	public static final String __md_methods;
	static {
		__md_methods = 
			"n_onStartSuccess:(Landroid/bluetooth/le/AdvertiseSettings;)V:GetOnStartSuccess_Landroid_bluetooth_le_AdvertiseSettings_Handler\n" +
			"";
		mono.android.Runtime.register ("Auto_Besturing.Droid.BTService+mAdvertiseCallback, Auto Besturing.Android", BTService_mAdvertiseCallback.class, __md_methods);
	}


	public BTService_mAdvertiseCallback ()
	{
		super ();
		if (getClass () == BTService_mAdvertiseCallback.class)
			mono.android.TypeManager.Activate ("Auto_Besturing.Droid.BTService+mAdvertiseCallback, Auto Besturing.Android", "", this, new java.lang.Object[] {  });
	}


	public void onStartSuccess (android.bluetooth.le.AdvertiseSettings p0)
	{
		n_onStartSuccess (p0);
	}

	private native void n_onStartSuccess (android.bluetooth.le.AdvertiseSettings p0);

	private java.util.ArrayList refList;
	public void monodroidAddReference (java.lang.Object obj)
	{
		if (refList == null)
			refList = new java.util.ArrayList ();
		refList.add (obj);
	}

	public void monodroidClearReferences ()
	{
		if (refList != null)
			refList.clear ();
	}
}
