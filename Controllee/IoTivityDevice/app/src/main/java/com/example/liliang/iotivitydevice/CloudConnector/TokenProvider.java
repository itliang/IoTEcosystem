package com.example.liliang.iotivitydevice.CloudConnector;

/**
 * Created by liliang on 17-8-17.
 */
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.scalars.ScalarsConverterFactory;
import retrofit2.http.Field;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.POST;

public class TokenProvider {
    public static final String TAG = TokenProvider.class.getName();
    private static String GRANT_TYPE = "authorization_code";


    public String accessToken;
    public String refreshToken;
    public String userId;

    public TokenService tokenService;

    private AccessTokenListener accessTokenListener = null;

    public TokenProvider(String authURL, AccessTokenListener accessTokenListener){

        this.accessTokenListener = accessTokenListener;

        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(authURL)
                .addConverterFactory(ScalarsConverterFactory.create())
                .build();
        tokenService = retrofit.create(TokenService.class);
    }

    public void getAccessToken(final String authURL, String authCode, String clientId, String deviceId){
        Call<String> call = tokenService.getAccessToken(GRANT_TYPE, clientId, authCode, deviceId);
        Log.e(TAG, "clientId:"+clientId+",deviceId:"+deviceId);
        call.enqueue(new Callback<String>() {
            @Override
            public void onResponse(Call<String> call, Response<String> response) {
                Log.e(TAG, "accessToken : "+response.body());
                try {
                    accessToken = getValFromJson(response.body(), "access_token");
                    userId = getValFromJson(response.body(), "userId");
                    refreshToken = getValFromJson(response.body(), "refresh_token");
                    accessTokenListener.onAccessTokenReceived(accessToken, refreshToken, userId, authURL);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<String> call, Throwable t) {
                Log.e(TAG, "accessToken failed");
                accessTokenListener.onAccessTokenFailed();
            }
        });
    }

    private interface TokenService{
        @FormUrlEncoded
        @POST("/auth/oauth2/token")
        Call<String> getAccessToken(@Field("grant_type") String grantType, @Field("client_id") String clientId,
                                    @Field("code") String authCode, @Field("physical_address_text") String deviceId);



        /*
        * HTTP Request, 3 properties (method, path, hasBody)
        * method (GET, POST, PUT)
        * path ()
        * hasBody (request body ?)
        * @HTTP(method="GET", path = "blog/{id}", hasBody = false)
        * Call<ResponseBody> getBlog(@path("id") int id);
        *
        * Parameter type
        * Field, FieldMap for Form
        * Part, PartMap for file upload, PartMap default accept data type Map<String, RequestBody>
        * @POST("/form")
        * @FormUrlEncoded
        * Call<ResponseBody> testFormUrlEncoded(@Field("username") String name, @Field("age") int age);
        * Call<ResponseBody> testFormUrlEncoded2(@FieldMap Map<String, Object> map);
        * Call<ResponseBody> testFileUpload1(@Part("name") RequestBody name, @Part("age") RequestBody age, @Part MultipartBody.Part file);

        *
        * */

    }


    public interface AccessTokenListener{
        void onAccessTokenReceived(String accessToken, String refreshToken, String userId, String authProviderURL);
        void onAccessTokenFailed();
    }

    private String getValFromJson(String json, String inputKey) throws JSONException {
        String val = "";
        JSONObject jsonObject = new JSONObject(json);
        Iterator<?> keyIterator = jsonObject.keys();
        while(keyIterator.hasNext()){
            String key = (String) keyIterator.next();
            if(key.equalsIgnoreCase(inputKey)){
                val = jsonObject.getString(key);
                break;
            }
        }
        return val;
    }
}
