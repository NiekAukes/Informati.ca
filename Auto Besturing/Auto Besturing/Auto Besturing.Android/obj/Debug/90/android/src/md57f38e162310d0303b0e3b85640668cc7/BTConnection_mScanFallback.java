package md57f38e162310d0303b0e3b85640668cc7;


public class BTConnection_mScanFallback
	extends android.bluetooth.le.ScanCallback
	implements
		mono.android.IGCUserPeer
{
/** @hide */
	public static final String __md_methods;
	static {
		__md_methods = 
			"";
		mono.android.Runtime.register ("Auto_Besturing.Droid.BTConnection+mScanFallback, Auto Besturing.Android", BTConnection_mScanFallback.class, __md_methods);
	}


	public BTConnection_mScanFallback ()
	{
		super ();
		if (getClass () == BTConnection_mScanFallback.class)
			mono.android.TypeManager.Activate ("Auto_Besturing.Droid.BTConnection+mScanFallback, Auto Besturing.Android", "", this, new java.lang.Object[] {  });
	}

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
