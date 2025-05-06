<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;
use Illuminate\Support\Facades\DB;

use App\Http\Controllers\AuthController;
use App\Http\Controllers\MeasureLogsController;
use App\Http\Middleware\EnsureAPIJsonHeaders;

Route::get('/connect', function () {
    return response()->json([
        'Status' => 'ok',
        'Message' => 'Response',
    ], 200);
});

Route::get('/greeting', function () {
    return response()->json([
        'Status' => 'ok',
        'Message' => 'Hello World',
    ], 200);
});

Route::middleware([EnsureAPIJsonHeaders::class])->group(function(){
    Route::post('/register', [AuthController::class, 'register']);
    Route::post('/login', [AuthController::class, 'login']);
    Route::post('/logout', [AuthController::class, 'logout']);
    Route::post('/AddMeasureLogs', [MeasureLogsController::class, 'AddMeasureLogs']);
    Route::get('/GetMeasureLogs', [MeasureLogsController::class, 'GetMeasureLogs']);
    Route::get('/GetTagMeasureLogs/{id}', [MeasureLogsController::class, 'GetTagMeasureLogs']);
});
