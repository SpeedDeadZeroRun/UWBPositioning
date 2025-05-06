<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\DB;

use App\Models\Entities;
use App\Models\MeasureLogs;
use App\Models\UwbIps;

class MeasureLogsController extends Controller
{   
    public function AddMeasureLogs(Request $request) {
        $ofUser = $request->user('sanctum');
        if (!$ofUser) {
            return response()->json([
                'status' => 'Token Does not exist.'
            ], 404);
        }
        $ofEntity = $ofUser->Entities;
        if (!$ofEntity) {
            return response()->json([
                'status' => 'Entity Does not exist.'
            ], 404);
        }

        if ($ofEntity->EntityTypes['type'] == 'controller') {
            $ControllerHaveAnchors = [];
            foreach ($ofEntity->ControllerHaveAnchors as $CHA) {
                $ControllerHaveAnchors[] = $CHA['have_anchor'];
            }
            
            foreach ($request->data as $data) {
                $ofAnchor = UwbIps::where('ip', $data['anchor'])->first();
                $ofTag = UwbIps::where('ip', $data['tag'])->first();
                if(!$ofAnchor || !$ofTag) continue;
                $data['anchor'] = $ofAnchor['uwb'];
                $data['tag'] = $ofTag['uwb'];
                if (in_array($data['anchor'], $ControllerHaveAnchors)) {
                    if ($data['timestamp'] == '') {
                        $data['timestamp'] = now();
                    }
                    MeasureLogs::insert($data);
                }
            }

            return response()->json([
                'Status' => 'ok',
            ], 200);
        }
        elseif ($ofEntity->EntityTypes['type'] == 'anchor') {
            $data = $request->data;
            foreach ($request->data as $data) {
                $ofTag = UwbIps::where('ip', $data['tag'])->first();
                if(!$ofTag) continue;
                $data['tag'] = $ofTag['uwb'];
                if ($data['timestamp'] == '') {
                    $data['timestamp'] = now();
                }
                $data['anchor'] = $ofEntity['id'];
                MeasureLogs::insert($data);
            }
            
            return response()->json([
                'Status' => 'ok',
            ], 200);
        }

        return response()->json([
            'status' => 'Unauthorized.'
        ], 401);
    }

    public function GetMeasureLogs() {
        $log = MeasureLogs::get();
        return response()->json([
            'Status' => 'ok',
            'data' => $log,
        ], 200);
    }

    public function GetTagMeasureLogs(int $id, Request $request) {
        $ofUser = $request->user('sanctum');
        if (!$ofUser) {
            return response()->json([
                'status' => 'Token Does not exist.'
            ], 404);
        }
        $ofEntity = $ofUser->Entities;
        if (!$ofEntity) {
            return response()->json([
                'status' => 'Entity Does not exist.'
            ], 404);
        }

        $tag = Entities::find($id);
        if ($tag) {
            $EntityAccessZones = [];
            foreach ($ofEntity->AccessZones as $EAZ) {
                $EntityAccessZones[] = $EAZ['access_zone'];
            }
            if (
                $tag->EntityTypes['type'] == 'tag' &&
                in_array($tag->AccessZones[0]['access_zone'], $EntityAccessZones)
            ) {
                $row_num = 1;
                $log = MeasureLogs::select(DB::raw('* ,row_number() OVER(PARTITION BY anchor ORDER BY timestamp DESC) AS row_num'))
                                    ->where('tag', $tag['id'])
                                    ->orderBy('row_num', 'ASC')
                                    ->limit(30)
                                    ->get();
                $LastLog = $log->where('row_num', $row_num)->makeHidden(['id', 'tag', 'row_num'])->values();
                return response()->json([
                    'Status' => 'ok',
                    'tag_id' => $tag['id'],
                    'row_num' => $row_num,
                    'data' => $LastLog,
                ], 200);
            }
        }

        return response()->json([
            'status' => 'Tag Does not exist.'
        ], 404);
    }
}
