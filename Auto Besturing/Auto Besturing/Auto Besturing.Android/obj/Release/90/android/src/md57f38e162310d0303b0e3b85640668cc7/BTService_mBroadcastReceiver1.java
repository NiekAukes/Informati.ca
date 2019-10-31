package md57f38e162310d0303b0e3b85640668cc7;


public class BTService_mBroadcastReceiver1
	extends android.content.BroadcastReceiver
	implements
		mono.android.IGCUserPeer
{
/** @hide */
	public static final String __md_methods;
	static {
		__md_methods = 
			"n_onReceive:(Landroid/content/Context;Landroid/content/Intent;)V:GetOnReceive_Landroid_content_Context_Landroid_content_Intent_Handler\n" +
			"";
		mono.android.Runtime.register ("Auto_Besturing.Droid.BTService+mBroadcastReceiver1, Auto Besturing.Android", BTService_mBroadcastReceiver1.class, __md_methods);
	}


	public BTService_mBroadcastReceiver1 ()
	{
		super ();
		if (getClass () == BTService_mBroadcastReceiver1.class)
			mono.android.TypeManager.Activate ("Auto_Besturing.Droid.BTService+mBroadcastReceiver1, Auto Besturing.Android", "", this, new java.lang.Object[] {  });
	}


	public void onReceive (android.content.Context p0, android.content.Intent p1)
	{
		n_onReceive (p0, p1);
	}

	private native void n_onReceive (android.content.Context p0, android.content.Intent p1);

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
