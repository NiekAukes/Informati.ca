package md57f38e162310d0303b0e3b85640668cc7;


public class BTService_mGattServerCallback
	extends android.bluetooth.BluetoothGattServerCallback
	implements
		mono.android.IGCUserPeer
{
/** @hide */
	public static final String __md_methods;
	static {
		__md_methods = 
			"n_onCharacteristicReadRequest:(Landroid/bluetooth/BluetoothDevice;IILandroid/bluetooth/BluetoothGattCharacteristic;)V:GetOnCharacteristicReadRequest_Landroid_bluetooth_BluetoothDevice_IILandroid_bluetooth_BluetoothGattCharacteristic_Handler\n" +
			"";
		mono.android.Runtime.register ("Auto_Besturing.Droid.BTService+mGattServerCallback, Auto Besturing.Android", BTService_mGattServerCallback.class, __md_methods);
	}


	public BTService_mGattServerCallback ()
	{
		super ();
		if (getClass () == BTService_mGattServerCallback.class)
			mono.android.TypeManager.Activate ("Auto_Besturing.Droid.BTService+mGattServerCallback, Auto Besturing.Android", "", this, new java.lang.Object[] {  });
	}


	public void onCharacteristicReadRequest (android.bluetooth.BluetoothDevice p0, int p1, int p2, android.bluetooth.BluetoothGattCharacteristic p3)
	{
		n_onCharacteristicReadRequest (p0, p1, p2, p3);
	}

	private native void n_onCharacteristicReadRequest (android.bluetooth.BluetoothDevice p0, int p1, int p2, android.bluetooth.BluetoothGattCharacteristic p3);

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
