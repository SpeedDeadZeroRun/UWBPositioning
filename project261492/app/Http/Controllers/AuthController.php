<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Hash;

use App\Models\Accesses;

class AuthController extends Controller
{
    public function register(Request $request) {
        if($request->validate([
            'name' => ['required','string','max:32','unique:accesses'],
            'secret' => ['required','string','min:8']
        ])){
            $access = Accesses::create([
                'name' => $request['name'],
                'secret' => Hash::make($request['secret'])
            ]);    
        }

        return response()->json([
            'name' => $request->name
        ], 200);
    }

    public function login(Request $request) {
        $access = Accesses::where('name',  $request->name)->first();
        if (!$access || !Hash::check($request->secret, $access->secret)) {
            return response()->json([
                'status' => 'Name or secret incorrect'
            ], 404);
        }

        $access->tokens()->delete();

        $newToken = $access->createToken('auth_token');

        return response()->json([
            'name' => $access->name,
            'token' => $newToken->plainTextToken
        ], 200);
    }

    public function logout(Request $request) {
        $access = $request->user('sanctum');
        if($access != null) $access->currentAccessToken()->delete();
        return response()->json([
            'status' => 'logged out successfully'
        ], 200);
    }
}
